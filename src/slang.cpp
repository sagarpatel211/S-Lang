#include "slang.hpp"

Slang::Slang(const std::string& code) : code(std::move(code)), parser(this->code) {
    debug << "[DEBUG] Slang initialized." << std::endl;
    Module slang_program = parser.parse_module();
    Codegen irgen;
    if (irgen.generate_ir(slang_program)) {
        std::cerr << "[INFO] IR generated successfully." << std::endl;
    } else {
        std::cerr << "[ERROR] IR generation failed." << std::endl;
        exit(1);
    }
    llvm_ir = irgen.output_ir();
}

void Slang::print_IR() const {
    std::cout << llvm_ir << std::endl;
}

void Slang::write_to_file(const std::string& filename) {
    try {
        std::ofstream out(filename);
        out << llvm_ir;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    }
}
