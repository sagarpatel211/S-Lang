/**
 * @file codegen.hpp
 * @brief Code Generator for the S-Lang Compiler
 *
 * This file contains the definition of the Codegen class responsible for
 * the code generation phase in the S-Lang Compiler. The CodeGenerator takes the
 * Abstract Syntax Tree (AST) produced by the Parser and translates it into
 * LLVM intermediate representation (IR).
 *
 * @author Sagar Patel
 * @date 1-6-2024
 *
 * Project: S-Lang Compiler
 */

#ifndef CODEGEN_HPP
#define CODEGEN_HPP
#pragma once

#include "ast.hpp"
#include "exceptions.hpp"
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_os_ostream.h>
#include <sstream>

/**
 * @brief Code Generator class for the S-Lang Compiler.
 *
 * Codegen is responsible for translating the abstract syntax tree (AST)
 * into LLVM Intermediate Representation (IR).
 */
class Codegen {
  private:
    std::unique_ptr<llvm::LLVMContext>
        context; // Manages the LLVM context for LLVM IR generation.
    std::unique_ptr<llvm::IRBuilder<>> builder; // Helps in constructing the LLVM IR.
    std::unique_ptr<llvm::Module> module; // Represents the LLVM module which holds the
                                          // functions and global variables.
    std::unordered_map<std::string, llvm::AllocaInst*>
        current_scope_symbols; // Maps variable names to their memory allocations in the
                               // current scope.
    llvm::BasicBlock* current_loop_condition; // Tracks the BasicBlock for the current
                                              // loop's condition check.
    llvm::BasicBlock* current_loop_merge;     // Tracks the BasicBlock where the control
                                              // should merge after a loop.

    /**
     * @brief Generates LLVM IR for a positive unary operation.
     *
     * @param rhs_value The right-hand side value of the unary operation.
     *
     * @return llvm::Value* -> The LLVM IR value of applying the positive unary operation.
     */
    llvm::Value* positive_unary_op(llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a negative unary operation.
     *
     * @param rhs_value The right-hand side value of the unary operation.
     *
     * @return llvm::Value* -> The LLVM IR value of applying the negative unary operation.
     */
    llvm::Value* negative_unary_op(llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a bitwise not unary operation.
     *
     * @param rhs_value The right-hand side value of the unary operation.
     *
     * @return llvm::Value* -> The resultant LLVM IR value of applying the bitwise not
     * unary operation.
     */
    llvm::Value* negate_unary_op(llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for an addition binary operation.
     *
     * This function takes two LLVM IR values, representing the left-hand side
     * and right-hand side of a binary addition, and generates the corresponding
     * LLVM IR code to perform the addition.
     *
     * @param lhs_value The left-hand side value of the binary addition.
     * @param rhs_value The right-hand side value of the binary addition.
     *
     * @return llvm::Value* -> The LLVM IR value after performing the addition.
     */
    llvm::Value* add_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a subtraction binary operation.
     *
     * This function creates the LLVM IR code for subtracting the right-hand side value
     * from the left-hand side value.
     *
     * @param lhs_value The left-hand side value of the binary subtraction.
     * @param rhs_value The right-hand side value of the binary subtraction.
     *
     * @return llvm::Value* -> The LLVM IR value after performing the subtraction.
     */
    llvm::Value* sub_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a multiplication binary operation.
     *
     * This function creates the LLVM IR code for multiplying the left-hand side value
     * with the right-hand side value.
     *
     * @param lhs_value The left-hand side value of the binary multiplication.
     * @param rhs_value The right-hand side value of the binary multiplication.
     *
     * @return llvm::Value* -> The LLVM IR value after performing the multiplication.
     */
    llvm::Value* mult_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a division binary operation.
     *
     * This function creates the LLVM IR code for dividing the left-hand side value
     * by the right-hand side value.
     *
     * @param lhs_value The left-hand side value of the binary division.
     * @param rhs_value The right-hand side value of the binary division.
     *
     * @return llvm::Value* -> The LLVM IR value after performing the division.
     */
    llvm::Value* div_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a modulus binary operation.
     *
     * This function creates the LLVM IR code for calculating the modulus of the
     * left-hand side value by the right-hand side value.
     *
     * @param lhs_value The left-hand side value of the binary modulus.
     * @param rhs_value The right-hand side value of the binary modulus.
     *
     * @return llvm::Value* -> The LLVM IR value after performing the modulus.
     */
    llvm::Value* modulus_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for an equality comparison binary operation.
     *
     * This function creates LLVM IR code to compare two values for equality.
     * It takes two LLVM IR values (left-hand side and right-hand side) and
     * generates code to check if they are equal.
     *
     * @param lhs_value The left-hand side value of the equality comparison.
     * @param rhs_value The right-hand side value of the equality comparison.
     *
     * @return llvm::Value* -> The resultant LLVM IR boolean value indicating true or
     * false.
     */
    llvm::Value* eq_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for an inequality comparison binary operation.
     *
     * This function creates LLVM IR code to compare two values for inequality.
     * It takes two LLVM IR values (left-hand side and right-hand side) and
     * generates code to check if they are not equal.
     *
     * @param lhs_value The left-hand side value of the inequality comparison.
     * @param rhs_value The right-hand side value of the inequality comparison.
     *
     * @return llvm::Value* -> The resultant LLVM IR boolean value indicating true or
     * false.
     */
    llvm::Value* neq_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a less-than comparison binary operation.
     *
     * This function creates LLVM IR code to compare two values for less-than.
     * It takes two LLVM IR values (left-hand side and right-hand side) and
     * generates code to check if the left-hand side value is less than the
     * right-hand side value.
     *
     * @param lhs_value The left-hand side value of the less-than comparison.
     * @param rhs_value The right-hand side value of the less-than comparison.
     *
     * @return llvm::Value* -> The resultant LLVM IR boolean value indicating true or
     * false.
     */
    llvm::Value* lessthan_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a greater-than comparison binary operation.
     *
     * This function creates LLVM IR code to compare two values for greater-than.
     * It takes two LLVM IR values (left-hand side and right-hand side) and
     * generates code to check if the left-hand side value is greater than the
     * right-hand side value.
     *
     * @param lhs_value The left-hand side value of the greater-than comparison.
     * @param rhs_value The right-hand side value of the greater-than comparison.
     *
     * @return llvm::Value* -> The resultant LLVM IR boolean value indicating true or
     * false.
     */
    llvm::Value* greaterthan_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a less-than-or-equal-to comparison binary operation.
     *
     * This function creates LLVM IR code to compare two values for less-than-or-equal-to.
     * It takes two LLVM IR values (left-hand side and right-hand side) and
     * generates code to check if the left-hand side value is less than or equal to the
     * right-hand side value.
     *
     * @param lhs_value The left-hand side value of the less-than-or-equal-to comparison.
     * @param rhs_value The right-hand side value of the less-than-or-equal-to comparison.
     *
     * @return llvm::Value* -> The resultant LLVM IR boolean value indicating true or
     * false.
     */
    llvm::Value* leq_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Generates LLVM IR for a greater-than-or-equal-to comparison binary
     * operation.
     *
     * This function creates LLVM IR code to compare two values for
     * greater-than-or-equal-to. It takes two LLVM IR values (left-hand side and
     * right-hand side) and generates code to check if the left-hand side value is greater
     * than or equal to the right-hand side value.
     *
     * @param lhs_value The left-hand side value of the greater-than-or-equal-to
     * comparison.
     * @param rhs_value The right-hand side value of the greater-than-or-equal-to
     * comparison.
     *
     * @return llvm::Value* -> The resultant LLVM IR boolean value indicating true or
     * false.
     */
    llvm::Value* geq_binary_op(llvm::Value* lhs_value, llvm::Value* rhs_value);

    /**
     * @brief Retrieves the LLVM Type corresponding to a given type name.
     *
     * This method is used to convert a type name represented as a string into
     * its corresponding LLVM Type. It is essential for handling different data
     * types during code generation (eg. char => builder->getInt8Ty())
     *
     * @param name The name of the type as a string.
     *
     * @return llvm::Type* The corresponding LLVM Type for the given type name, if it
     * exists.
     */
    llvm::Type* get_type_from_typename(const std::string& name) const;
  public:
    /**
     * @brief Overloaded function call operator for handling integer literals.
     *
     * This operator generates LLVM IR code for an integer literal. It takes a
     * Literal<int> node representing the integer literal and converts it into
     * an LLVM IR value.
     *
     * @param node The Literal<int> node representing the integer literal.
     *
     * @return llvm::Value* -> LLVM IR representation of the integer literal.
     */
    llvm::Value* operator()(const Literal<int>& node);

    /**
     * @brief Overloaded function call operator for handling double literals.
     *
     * This operator generates LLVM IR code for a double literal. It takes a
     * Literal<double> node representing the double literal and converts it into
     * an LLVM IR value.
     *
     * @param node The Literal<double> node representing the double literal.
     *
     * @return llvm::Value* -> LLVM IR representation of the double literal.
     */
    llvm::Value* operator()(const Literal<double>& node);

    /**
     * @brief Overloaded function call operator for handling boolean literals.
     *
     * This operator generates LLVM IR code for a boolean literal. It takes a
     * Literal<bool> node representing the boolean literal and converts it into
     * an LLVM IR value.
     *
     * @param node The Literal<bool> node representing the boolean literal.
     *
     * @return llvm::Value* -> LLVM IR representation of the boolean literal.
     */
    llvm::Value* operator()(const Literal<bool>& node);

    /**
     * @brief Overloaded function call operator for handling character literals.
     *
     * This operator generates LLVM IR code for a character literal. It takes a
     * Literal<char> node representing the character literal and converts it into
     * an LLVM IR value.
     *
     * @param node The Literal<char> node representing the character literal.
     *
     * @return llvm::Value* -> LLVM IR representation of the character literal.
     */
    llvm::Value* operator()(const Literal<char>& node);

    /**
     * @brief Overloaded function call operator for handling string literals.
     *
     * This operator generates LLVM IR code for a string literal. It takes a
     * Literal<std::string> node representing the string literal and converts it into
     * an LLVM IR value.
     *
     * @param node The Literal<std::string> node representing the string literal.
     *
     * @return llvm::Value* -> LLVM IR representation of the string literal.
     */
    llvm::Value* operator()(const Literal<std::string>& node);

    /**
     * @brief Overloaded function call operator for handling variable expressions.
     *
     * This operator generates LLVM IR code for a variable expression. It takes
     * a unique pointer to a VariableExpression node representing the variable
     * and retrieves the corresponding value from the symbol table or generates
     * the necessary IR code to access it.
     *
     * @param node A unique pointer to the VariableExpression node.
     *
     * @return llvm::Value* -> LLVM IR representation of the variable expression.
     */
    llvm::Value* operator()(std::unique_ptr<VariableExpression>& node);

    /**
     * @brief Overloaded function call operator for handling unary expressions.
     *
     * This operator generates LLVM IR code for a unary expression. It takes a
     * unique pointer to a UnaryExpression node, which contains an operator and
     * a single operand, and generates the appropriate LLVM IR code to perform
     * the unary operation.
     *
     * @param node A unique pointer to the UnaryExpression node.
     *
     * @return llvm::Value* -> LLVM IR representation of the unary expression.
     */
    llvm::Value* operator()(std::unique_ptr<UnaryExpression>& node);

    /**
     * @brief Overloaded function call operator for handling binary expressions.
     *
     * This operator generates LLVM IR code for a binary expression. It takes a
     * unique pointer to a BinaryExpression node, which contains an operator and
     * two operands, and generates the appropriate LLVM IR code to perform the
     * binary operation.
     *
     * @param node A unique pointer to the BinaryExpression node.
     *
     * @return llvm::Value* -> LLVM IR representation of the binary expression.
     */
    llvm::Value* operator()(std::unique_ptr<BinaryExpression>& node);

    /**
     * @brief Overloaded function call operator for handling call expressions.
     *
     * This operator generates LLVM IR code for a function call expression. It
     * takes a unique pointer to a CallExpression node, representing a function
     * call, and generates the LLVM IR code to handle the function invocation,
     * including argument passing and return value handling.
     *
     * @param node A unique pointer to the CallExpression node.
     *
     * @return llvm::Value* -> LLVM IR representation of the function call expression.
     */
    llvm::Value* operator()(std::unique_ptr<CallExpression>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<CookedUpStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<CookedUpAssignmentStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<AssignmentStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<FrOngJustLikeThatStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<HoldUpStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<GhostStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<RizzStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<YeetStatement>& node);

    // TO DO: Documentation
    void operator()(std::unique_ptr<CompoundStatement>& node);

    // TO DO: Documentation
    void operator()(PluhDeclaration& node);

    /**
     * @brief Constructor for the Codegen class.
     *
     * Initializes an instance of the Codegen class. This constructor sets up
     * the necessary environment for generating LLVM Intermediate Representation (IR),
     * including initializing LLVM context, builders, and modules by processing the AST.
     */
    Codegen();

    /**
     * @brief Generates the LLVM IR from the given AST.
     *
     * This method takes the root node of an abstract syntax tree (represented by
     * TeaSpill) and traverses it to generate the corresponding LLVM IR for the entire
     * program. The TeaSpill node encapsulates the entire program structure.
     *
     * @param module_node The root of the AST (TeaSpill) representing the entire program.
     *
     * @return True if the IR generation was successful, False otherwise.
     */
    bool generate_ir(TeaSpill& module_node);

    /**
     * @brief Outputs the generated LLVM IR as a string.
     *
     * Once the LLVM IR has been generated by traversing the AST, this method
     * can be used to retrieve the IR as a string. This is used for writing the IR to a
     * file.
     *
     * @return std::string A string representation of the generated LLVM IR.
     */
    std::string output_ir();
};

#endif