#include "slang.hpp"

Slang::Slang(const std::string& code) : code(std::move(code)), parser(this->code) {
    debug << "[DEBUG] Slang initialized." << std::endl;
    TeaSpill slang_program = parser.parse_tea();
    debug << "[DEBUG] Tea parsed." << std::endl;
    Codegen irgen;
    debug << "[DEBUG] Codegen initialized." << std::endl;
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
    return;
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
