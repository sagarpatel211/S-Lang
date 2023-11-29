#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/* == Usage == */
void usage() {
    std::cout << "Usage: program [file] [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h  Show this help message" << std::endl;
    std::cout << "  -r  Rename outputted binary executable" << std::endl;
    std::cout << "  -e  Emit IR code" << std::endl;
    exit(1);
}

/* == File Processing == */
std::string process_file(const std::string& file_path) {
    try {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << file_path << std::endl;
            exit(1);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string file_content = buffer.str();

        return file_content;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "ERROR: Unknown error occurred" << std::endl;
        exit(1);
    }
}

/* == Flags == */
void processFlags(const std::string& flags, bool& emit_IR, bool& rename_executable) {
    for (const char flag : flags) {
        switch (flag) {
            case 'h':
                usage();
                break;
            case 'e':
                emit_IR = true;
                break;
            case 'r':
                rename_executable = true;
                break;
            default:
                usage();
        }
    }
}

/* == Main Code == */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage();
    }

    std::string file_path = "";
    std::string content = "";
    bool emit_IR = false;
    bool rename_executable = false;

    for (int i = 1; i < argc; ++i) {
        std::cout << "Processing argument: " << argv[i] << std::endl;
        std::string arg = argv[i];
        if (arg[0] == '-') {
            processFlags(arg.substr(1), emit_IR, rename_executable);
        } else if (file_path.empty()) {
            file_path = arg;
        } else {
            usage();
        }
    }

    if (!file_path.empty()) {
        content = process_file(file_path);
    }
    
    // TO DO: Add LLVM IR code generation

    return 0;
}
