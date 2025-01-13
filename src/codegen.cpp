#include "codegen.hpp"

Codegen::Codegen()
    : context(new llvm::LLVMContext()),
      builder(new llvm::IRBuilder<>(*context)),
      module(new llvm::Module("", *context)),
      current_scope_symbols({}),
      current_loop_condition(nullptr),
      current_loop_merge(nullptr) {}

bool Codegen::generate_ir(TeaSpill& module_node) {
    try {
        module->setModuleIdentifier(module_node.get_name());

        // Handling declarations in the module
        for (auto& decl : module_node.get_declarations()) {
            std::visit(*this, decl);
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

std::string Codegen::output_ir() {
    std::ostringstream string_stream;
    llvm::raw_os_ostream raw_stream(string_stream);
    module->print(raw_stream, nullptr);
    raw_stream.flush();
    return string_stream.str();
}

llvm::Value* Codegen::positive_unary_op(llvm::Value* rhs_value) { // eg. (+ 1) -> 1
    if (rhs_value->getType()->isDoubleTy()) {
        return rhs_value;
    } else if (rhs_value->getType()->isIntegerTy()) {
        return rhs_value;
    } else {
        throw codegen_error("Unknown type signature for unary operator +");
    }
}

llvm::Value* Codegen::negative_unary_op(llvm::Value* rhs_value) { // eg. (- 1) -> -1
    try {
        if (rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFNeg(rhs_value);
        } else if (rhs_value->getType()->isIntegerTy()) {
            return builder->CreateNeg(rhs_value);
        } else {
            throw codegen_error("Unknown type signature for unary operator -");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for unary operator -" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::negate_unary_op(llvm::Value* rhs_value) { // eg. (! true) -> false
    try {
        if (rhs_value->getType()->isIntegerTy()) {
            return builder->CreateNot(rhs_value);
        } else {
            throw codegen_error("Unknown type signature for unary operator !");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for unary operator !" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::add_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value) {
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFAdd(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy(32) &&
                   rhs_value->getType()->isIntegerTy(32)) {
            return builder->CreateAdd(lhs_value, rhs_value);
            // } else if (lhs_value->getType()->isPointerTy() &&
            // lhs_value->getType()->getPointerElementType()->isIntegerTy(8) &&
            //            rhs_value->getType()->isPointerTy() &&
            //            rhs_value->getType()->getPointerElementType()->isIntegerTy(8)) {
            // // Find the end of the first string (lhs_value)
            // llvm::Value* lhs_end = builder->CreateGEP(lhs_value,
            // builder->CreateStrLen(lhs_value));

            // // Copy characters from the second string (rhs_value) to the end of the
            // first string llvm::Value* index = llvm::ConstantInt::get(*context,
            // llvm::APInt(32, 0)); llvm::Value* currentChar =
            // builder->CreateLoad(builder->CreateGEP(rhs_value, index)); while
            // (currentChar != llvm::ConstantInt::get(*context, llvm::APInt(8, 0))) {
            //     builder->CreateStore(currentChar, lhs_end);
            //     index = builder->CreateAdd(index, llvm::ConstantInt::get(*context,
            //     llvm::APInt(32, 1))); currentChar =
            //     builder->CreateLoad(builder->CreateGEP(rhs_value, index)); lhs_end =
            //     builder->CreateGEP(lhs_end, llvm::ConstantInt::get(*context,
            //     llvm::APInt(32, 1)));
            // }

            // return lhs_value;
        } else {
            throw std::runtime_error("Unknown type signature for binary operator +");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator +" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::sub_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value) {
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFSub(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateSub(lhs_value, rhs_value);
        } else {
            throw std::runtime_error("Unknown type signature for binary operator -");
        }
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator -" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::mult_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value) {
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFMul(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateMul(lhs_value, rhs_value);
        } else {
            throw std::runtime_error("Unknown type signature for binary operator *");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator *" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::div_binary_op(llvm::Value* lhs_value,
                                    llvm::Value* rhs_value) { // eg. (/ 1 2) -> 0
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFDiv(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateSDiv(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator /");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator /" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::modulus_binary_op(llvm::Value* lhs_value,
                                        llvm::Value* rhs_value) { // eg. (% 1 2) -> 1
    try {
        if (lhs_value->getType()->isIntegerTy() && rhs_value->getType()->isIntegerTy()) {
            return builder->CreateSRem(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator %");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator %" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::eq_binary_op(llvm::Value* lhs_value,
                                   llvm::Value* rhs_value) { // eg. (== 1 2) -> false
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFCmpOEQ(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateICmpEQ(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy(8) &&
                   rhs_value->getType()->isIntegerTy(8)) {
            return builder->CreateICmpEQ(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isPointerTy() &&
                   rhs_value->getType()->isPointerTy()) {
            return builder->CreateICmpEQ(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator ==");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator ==" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::neq_binary_op(llvm::Value* lhs_value,
                                    llvm::Value* rhs_value) { // eg. (!= 1 2) -> true
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFCmpONE(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateICmpNE(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isPointerTy() &&
                   rhs_value->getType()->isPointerTy()) {
            return builder->CreateICmpNE(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator !=");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator !=" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::lessthan_binary_op(llvm::Value* lhs_value,
                                         llvm::Value* rhs_value) { // eg. (< 1 2) -> true
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFCmpOLT(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateICmpSLT(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator <");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator <" << std::endl;
        exit(1);
    }
}

llvm::Value*
Codegen::greaterthan_binary_op(llvm::Value* lhs_value,
                               llvm::Value* rhs_value) { // eg. (> 1 2) -> false
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFCmpOGT(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateICmpSGT(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator >");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator >" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::leq_binary_op(llvm::Value* lhs_value,
                                    llvm::Value* rhs_value) { // eg. (<= 1 2) -> true
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFCmpOLE(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateICmpSLE(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator <=");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator <=" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::geq_binary_op(llvm::Value* lhs_value,
                                    llvm::Value* rhs_value) { // eg. (>= 1 2) -> false
    try {
        if (lhs_value->getType()->isDoubleTy() && rhs_value->getType()->isDoubleTy()) {
            return builder->CreateFCmpOGE(lhs_value, rhs_value);
        } else if (lhs_value->getType()->isIntegerTy() &&
                   rhs_value->getType()->isIntegerTy()) {
            return builder->CreateICmpSGE(lhs_value, rhs_value);
        } else {
            throw codegen_error("Unknown type signature for binary operator >=");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for binary operator >=" << std::endl;
        exit(1);
    }
}

llvm::AllocaInst* create_entry_block_alloc(llvm::Function* function, llvm::StringRef name,
                                           llvm::Type* type) {
    try {
        llvm::IRBuilder<> tmp_block(&function->getEntryBlock(),
                                    function->getEntryBlock().begin());
        return tmp_block.CreateAlloca(type, nullptr, name);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for creating entry block allocation"
                  << std::endl;
        exit(1);
    }
}

llvm::Type* Codegen::get_type_from_typename(const std::string& name) const {
    try {
        if (name == "bool") {
            return builder->getInt1Ty();
        } else if (name == "int") {
            return builder->getInt32Ty();
        } else if (name == "float") {
            return builder->getDoubleTy();
        } else if (name == "char") {
            return builder->getInt8Ty();
        } else if (name == "string") {
            return builder->getInt8Ty()->getPointerTo();
        } else if (name == "npc") {
            return builder->getInt8Ty()->getPointerTo();
        } else {
            throw codegen_error("Unknown type when getting type from typename");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for getting type from typename" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(const Literal<int>& node) {
    try {
        return llvm::ConstantInt::get(
            *context, llvm::APInt(32, static_cast<uint64_t>(node.get_value()), true));
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for int literal" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(const Literal<double>& node) {
    try {
        return llvm::ConstantFP::get(*context, llvm::APFloat(node.get_value()));
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for float literal" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(const Literal<bool>& node) {
    try {
        return llvm::ConstantInt::getBool(*context, node.get_value());
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for bool literal" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(const Literal<char>& node) {
    try {
        return llvm::ConstantInt::get(
            *context, llvm::APInt(8, static_cast<uint64_t>(node.get_value()), true));
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for char literal" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(const Literal<std::string>& node) {
    try {
        return builder->CreateGlobalStringPtr(node.get_value());
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for string literal" << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(std::unique_ptr<VariableExpression>& node) {
    try {
        llvm::AllocaInst* alloc = current_scope_symbols.at(node->get_name());
        return builder->CreateLoad(alloc->getAllocatedType(), alloc);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for variable expression"
                  << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(std::unique_ptr<UnaryExpression>& node) {
    try {
        auto rhs_val = std::visit(*this, node->get_rhs());
        if (rhs_val == nullptr) {
            throw codegen_error(
                "Failed to codegen for Right Hand Side of Unary Expression");
        }
        std::string str_op = node->get_op();
        if (str_op == "+") {
            return positive_unary_op(rhs_val);
        } else if (str_op == "-") {
            return negative_unary_op(rhs_val);
        } else if (str_op == "!") {
            return negate_unary_op(rhs_val);
        } else {
            throw codegen_error("Unknown unary operator found");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for unary expression"
                  << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(std::unique_ptr<BinaryExpression>& node) {
    try {
        auto lhs_val = std::visit(*this, node->get_lhs());
        auto rhs_val = std::visit(*this, node->get_rhs());
        if (lhs_val == nullptr) {
            throw std::logic_error(
                "failed to codegen for Left Hand Side of Binary Expression");
        } else if (rhs_val == nullptr) {
            throw std::logic_error(
                "failed to codegen for Right Hand Side of Binary Expression");
        }
        std::string str_op = node->get_op();
        if (str_op == "+") {
            return add_binary_op(lhs_val, rhs_val);
        } else if (str_op == "-") {
            return sub_binary_op(lhs_val, rhs_val);
        } else if (str_op == "*") {
            return mult_binary_op(lhs_val, rhs_val);
        } else if (str_op == "/") {
            return div_binary_op(lhs_val, rhs_val);
        } else if (str_op == "%") {
            return modulus_binary_op(lhs_val, rhs_val);
        } else if (str_op == "==") {
            return eq_binary_op(lhs_val, rhs_val);
        } else if (str_op == "!=") {
            return neq_binary_op(lhs_val, rhs_val);
        } else if (str_op == "<") {
            return lessthan_binary_op(lhs_val, rhs_val);
        } else if (str_op == ">") {
            return greaterthan_binary_op(lhs_val, rhs_val);
        } else if (str_op == "<=") {
            return leq_binary_op(lhs_val, rhs_val);
        } else if (str_op == ">=") {
            return geq_binary_op(lhs_val, rhs_val);
        } else {
            throw codegen_error("Unknown binary operator found");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for binary expression"
                  << std::endl;
        exit(1);
    }
}

llvm::Value* Codegen::operator()(std::unique_ptr<CallExpression>& node) {
    auto callee_func = module->getFunction(node->get_callee());

    if (callee_func == nullptr) {
        if (node->get_callee() == "yap") {
            std::vector<llvm::Value*> argument_values;
            std::string formatStr;

            // Generate format string based on argument types
            for (auto& arg : node->get_arguments()) {
                auto arg_val = std::visit(*this, arg);

                if (arg_val->getType()->isIntegerTy()) {
                    formatStr += "%d ";
                } else if (arg_val->getType()->isPointerTy()) {
                    // Assuming pointer type is a string
                    formatStr += "%s ";
                } else {
                    // Handle other types or throw an error
                    throw codegen_error("Unsupported argument type for yap");
                }

                argument_values.push_back(arg_val);
            }
            formatStr += "\n";
            // Add format string as the first argument to printf
            llvm::Value* formatStrVal = builder->CreateGlobalStringPtr(formatStr.c_str());
            argument_values.insert(argument_values.begin(), formatStrVal);

            // Get or declare printf function
            llvm::Function* printfFunc = module->getFunction("printf");
            if (!printfFunc) {
                std::vector<llvm::Type*> printfArgs = {
                    builder->getInt8Ty()->getPointerTo()};
                auto printfType =
                    llvm::FunctionType::get(builder->getInt32Ty(), printfArgs, true);
                printfFunc = llvm::Function::Create(
                    printfType, llvm::Function::ExternalLinkage, "printf", module.get());
            }

            return builder->CreateCall(printfFunc, argument_values);
        }

        throw codegen_error("Unknown function found");
    } else if (callee_func->arg_size() != node->get_arguments().size()) {
        throw codegen_error("Mismatch of Arguments in function call");
    }

    std::vector<llvm::Value*> argument_values;
    for (auto& arg : node->get_arguments()) {
        auto arg_val = std::visit(*this, arg);
        argument_values.push_back(arg_val);
    }
    return builder->CreateCall(callee_func, argument_values);
}

void Codegen::operator()(std::unique_ptr<CookedUpStatement>& node) {
    try {
        llvm::Function* current_func = builder->GetInsertBlock()->getParent();
        llvm::Value* init_val;
        llvm::Type* var_type_ir;
        if (node->get_var_type() == "int") {
            init_val = llvm::ConstantInt::get(
                *context, llvm::APInt(32, static_cast<uint64_t>(0), true));
            var_type_ir = builder->getInt32Ty();
        } else if (node->get_var_type() == "float") {
            init_val = llvm::ConstantFP::get(*context, llvm::APFloat(0.0));
            var_type_ir = builder->getDoubleTy();
        } else if (node->get_var_type() == "char") {
            init_val = llvm::ConstantInt::get(
                *context, llvm::APInt(8, static_cast<uint64_t>(0), true));
            var_type_ir = builder->getInt8Ty();
        } else {
            throw codegen_error("Unknown type for let statement");
        }
        llvm::AllocaInst* alloc =
            create_entry_block_alloc(current_func, node->get_var_name(), var_type_ir);
        builder->CreateStore(init_val, alloc);
        current_scope_symbols[node->get_var_name()] = alloc;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for let statement" << std::endl;
        exit(1);
    }
}

void Codegen::operator()(std::unique_ptr<CookedUpAssignmentStatement>& node) {
    try {
        // Step 1: Variable Declaration (similar to LetStatement)
        llvm::Function* current_func = builder->GetInsertBlock()->getParent();
        llvm::Value* init_val;
        llvm::Type* var_type_ir;

        // Determine the variable type and initial value
        if (node->get_var_type() == "int") {
            init_val = llvm::ConstantInt::get(
                *context, llvm::APInt(32, static_cast<uint64_t>(0), true));
            var_type_ir = builder->getInt32Ty();
        } else if (node->get_var_type() == "float") {
            init_val = llvm::ConstantFP::get(*context, llvm::APFloat(0.0));
            var_type_ir = builder->getDoubleTy();
        } else if (node->get_var_type() == "char") {
            init_val = llvm::ConstantInt::get(
                *context, llvm::APInt(8, static_cast<uint64_t>(0), true));
            var_type_ir = builder->getInt8Ty();
        } else {
            throw codegen_error("Unknown type for let and assignment statement");
        }

        // Allocate memory for the variable
        llvm::AllocaInst* alloc =
            create_entry_block_alloc(current_func, node->get_var_name(), var_type_ir);

        // Step 2: Assign the initial value (similar to AssignmentStatement)
        llvm::Value* rhs_val = std::visit(*this, node->get_assignment_expression());

        // Store the rhs value into the allocated space
        builder->CreateStore(rhs_val, alloc);

        // Add the variable to the current scope
        current_scope_symbols[node->get_var_name()] = alloc;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for let and assignment statement"
                  << std::endl;
        exit(1);
    }
}

void Codegen::operator()(std::unique_ptr<AssignmentStatement>& node) {
    try {
        if (node->get_var_name() == "@") {
            // The expression is a function call
            auto& expr = node->get_assignment_expression();
            if (auto* call_expr_ptr =
                    std::get_if<std::unique_ptr<CallExpression>>(&expr)) {
                // Call the existing function to handle CallExpression
                this->operator()(*call_expr_ptr);
            } else {
                std::cerr << "[ERROR] Expression is not a call expression" << std::endl;
                exit(1);
            }
            return;
        }

        // Handle normal assignment
        llvm::Value* rhs_val = std::visit(*this, node->get_assignment_expression());
        llvm::Value* lhs_val = current_scope_symbols.at(node->get_var_name());
        builder->CreateStore(rhs_val, lhs_val);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for assignment statement"
                  << std::endl;
        exit(1);
    }
}

void Codegen::operator()(std::unique_ptr<FrOngJustLikeThatStatement>& node) {
    try {
        llvm::Value* cond_expr = std::visit(*this, node->get_condition());
        llvm::Function* current_func = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock* then_block =
            llvm::BasicBlock::Create(*context, "then", current_func);
        llvm::BasicBlock* else_block = llvm::BasicBlock::Create(*context, "else");
        llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(*context, "merge");
        builder->CreateCondBr(cond_expr, then_block, else_block);
        builder->SetInsertPoint(then_block);
        std::visit(*this, node->get_then_statement());
        if (builder->GetInsertBlock()->getTerminator() == nullptr) {
            builder->CreateBr(merge_block);
        }
        current_func->insert(current_func->end(), else_block);
        builder->SetInsertPoint(else_block);
        std::visit(*this, node->get_else_statement());
        if (builder->GetInsertBlock()->getTerminator() == nullptr) {
            builder->CreateBr(merge_block);
        }
        current_func->insert(current_func->end(), merge_block);
        builder->SetInsertPoint(merge_block);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for if statement" << std::endl;
        exit(1);
    }
}

void Codegen::operator()(std::unique_ptr<HoldUpStatement>& node) {
    try {
        llvm::Function* current_func = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock* cond_block =
            llvm::BasicBlock::Create(*context, "cond", current_func);
        llvm::BasicBlock* body_block = llvm::BasicBlock::Create(*context, "body");
        llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(*context, "merge");
        llvm::BasicBlock* cond_block_old = current_loop_condition;
        llvm::BasicBlock* merge_block_old = current_loop_merge;

        builder->CreateBr(cond_block);
        builder->SetInsertPoint(cond_block);
        llvm::Value* cond_expr = std::visit(*this, node->get_condition());
        builder->CreateCondBr(cond_expr, body_block, merge_block);

        current_func->insert(current_func->end(), body_block);
        builder->SetInsertPoint(body_block);

        current_loop_condition = cond_block;
        current_loop_merge = merge_block;

        std::visit(*this, node->get_body());
        if (builder->GetInsertBlock()->getTerminator() == nullptr) {
            builder->CreateBr(cond_block);
        }

        current_func->insert(current_func->end(), merge_block);
        builder->SetInsertPoint(merge_block);

        current_loop_condition = cond_block_old;
        current_loop_merge = merge_block_old;
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for while statement" << std::endl;
        exit(1);
    }
}

void Codegen::operator()(std::unique_ptr<GhostStatement>& node) {
    try {
        if (current_loop_merge == nullptr) {
            throw codegen_error("Break statement outside of loop");
        }
        builder->CreateBr(current_loop_merge);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for break statement" << std::endl;
    }
}

void Codegen::operator()(std::unique_ptr<RizzStatement>& node) {
    try {
        if (current_loop_condition == nullptr) {
            throw codegen_error("Continue statement outside of loop");
        }
        builder->CreateBr(current_loop_condition);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for continue statement"
                  << std::endl;
    }
}

void Codegen::operator()(std::unique_ptr<YeetStatement>& node) {
    try {
        llvm::Value* yeet_expr = std::visit(*this, node->get_yeet_expr());
        builder->CreateRet(yeet_expr);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for return statement"
                  << std::endl;
        exit(1);
    }
}

void Codegen::operator()(std::unique_ptr<CompoundStatement>& node) {
    try {
        for (auto& stmt : node->get_statements()) {
            std::visit(*this, stmt);
        }
    } catch (...) {
        std::cerr << "[ERROR] Unknown error for codegen for compound statement"
                  << std::endl;
        exit(1);
    }
}

void Codegen::operator()(PluhDeclaration& node) {
    try {
        std::vector<llvm::Type*> arg_types = {};
        for (auto& arg : node.get_prototype().get_arguments()) {
            const std::string& type_name = arg.second;
            llvm::Type* type = get_type_from_typename(type_name);
            if (type->isVoidTy()) {
                throw codegen_error("npc not allowed here");
            }
            arg_types.push_back(type);
        }
        llvm::Type* return_type =
            get_type_from_typename(node.get_prototype().get_return_type());

        std::string function_name = node.get_prototype().get_name();
        if (module->getFunction(function_name) && function_name != "yap") {
            throw codegen_error("A pluh has been re-defined: " + function_name);
        }

        llvm::FunctionType* func_type =
            llvm::FunctionType::get(return_type, arg_types, false);
        llvm::Function* func =
            llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                   node.get_prototype().get_name(), module.get());
        auto arg_name_it = node.get_prototype().get_arguments().begin();
        for (auto& arg : func->args()) {
            arg.setName(arg_name_it->first);
            arg_name_it++;
        }
        if (!(node.get_body().has_value())) {
            return;
        }

        llvm::BasicBlock* func_block = llvm::BasicBlock::Create(*context, "entry", func);
        builder->SetInsertPoint(func_block);
        current_scope_symbols.clear();
        for (auto& arg : func->args()) {
            llvm::AllocaInst* alloc =
                create_entry_block_alloc(func, arg.getName(), arg.getType());
            builder->CreateStore(&arg, alloc);
            current_scope_symbols[std::string(arg.getName())] = alloc;
        }
        std::visit(*this, node.get_body().value());
        if (builder->GetInsertBlock()->getTerminator() == nullptr) {
            builder->CreateRet(llvm::UndefValue::get(return_type));
        }
        llvm::verifyFunction(*func);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown error in codegen for function declaration"
                  << std::endl;
    }
}
