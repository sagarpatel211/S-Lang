/**
 * @file main.cpp
 * @brief Entry point for a C++ program with file processing and command-line interface.
 *
 * This file contains the main function for a program that processes text files and
 * handles various command-line arguments. It includes functionality for reading files,
 * processing command-line flags, and initializing various components for the language.
 * This program is part of a larger project that involves parsing and code generation.
 *
 * @author Sagar Patel
 * @date 12-3-2023
 *
 * Project: S-Lang Compiler
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "slang.hpp"


// Flag to check if verbose mode is enabled
bool debug_mode = false;
DebugStream debug;


/**
 * @brief Prints the application's logo to the console.
 *
 * This function outputs a pre-defined, text-based logo of the application, 
 * including the application name and developer credit ('Developed by Sagar Patel'). 
 *
 * @note This function only performs console output.
 */
void print_logo() {
    std::cout << " _____     __    _____ _____ _____ " << std::endl;
    std::cout << "|   __|___|  |  |  _  |   | |   __|" << std::endl;
    std::cout << "|__   |___|  |__|     | | | |  |  |" << std::endl;
    std::cout << "|_____|   |_____|__|__|_|___|_____|" << std::endl;
    std::cout << "|    Developed by Sagar Patel     |" << std::endl;
    std::cout << "|_________________________________|" << std::endl;
    std::cout << std::endl;
}


/**
 * @brief Displays the usage instructions for the program.
 *
 * This function prints out the command-line usage instructions for the program.
 * It lists all available options and their descriptions, providing help on how
 * to properly use the compiler. This function is typically called when the user
 * passes invalid arguments or explicitly requests help with the '-h' flag. 
 *
 * The options include:
 *  - '-h': Display help message.
 *  - '-r': Rename outputted Intermediate Representation (IR) file. Default is 'output.ll'.
 *  - '-e': Print IR code.
 *  - '-v': Enable verbose mode for detailed output.
 *
 * @note This function terminates the program after displaying the help message.
 */
void usage() {
    print_logo();
    std::cout << "Usage: ./slang [options] [file]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h  Show this help message" << std::endl;
    std::cout << "  -r  Rename outputted IR file [Default: output.ll]" << std::endl;
    std::cout << "  -e  Print IR code" << std::endl;
    std::cout << "  -v  Enable verbose mode" << std::endl;
    exit(1);
}


/**
 * @brief Reads the contents of a file and returns it as a string.
 *
 * This function opens a file specified by 'file_path' and reads its contents into a string.
 * If the file cannot be opened or read, it throws a 'process_file_error' exception. The function
 * uses a standard input file stream (`std::ifstream`) to read data, and a string stream
 * (`std::stringstream`) to collect the file's contents.
 *
 * @param file_path A constant reference to a string containing the path to the file to be processed.
 * @return A string containing the contents of the file.
 *
 * @throws process_file_error If the file cannot be opened or another file reading error occurs.
 *
 * @note If an exception is thrown, the program terminates with an exit status of 1.
 *       This includes handling of both specific (`std::exception`) and unknown exceptions.
 */
std::string process_file(const std::string& file_path) {
    try {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw process_file_error("Error opening file!");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string file_content = buffer.str();
        return file_content;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "ERROR: Unknown error occurred!" << std::endl;
        exit(1);
    }
}


/**
 * @brief Processes individual flag characters from a command-line argument string.
 *
 * This function iterates through each character in the 'flags' string, treating each
 * character as a separate command-line flag. It supports several flags:
 *  - 'h': Calls the 'usage()' function to display help information.
 *  - 'e': Enables IR code printing by setting 'print_IR' to true.
 *  - 'v': Enables verbose mode by setting the global 'debug_mode' to true.
 * If an unrecognized flag is encountered, it calls 'usage()' to display help information.
 *
 * @param flags A constant reference to a string containing the flag characters.
 * @param print_IR A reference to a boolean that is set to true if the 'e' flag is present.
 */
void process_single_flags(const std::string& flags, bool& print_IR) {
    for (const char flag : flags) {
        switch (flag) {
            case 'h':
                usage();
                break;
            case 'v':
                debug_mode = true;
                debug << "[DEBUG] Verbose output mode enabled." << std::endl;
                break;
            case 'e':
                print_IR = true;
                debug << "[DEBUG] IR code will be printed." << std::endl;
                break;
            default:
                debug << "[DEBUG] Invalid flag: " << flag << std::endl;
                usage();
        }
    }
}


/**
 * @brief Main entry point of the program.
 *
 * This function handles command-line arguments and initializes the processing of the program.
 * It requires at least one argument (file path). The function supports several options:
 *  - '-r': Specify a custom name for the output file.
 *  - Other flags processed by the 'process_single_flags' function.
 * If the required file path is not provided or multiple file paths are given, it throws an exception.
 * After processing arguments, it proceeds to read the file content and initializes Slang for processing.
 *
 * @param argc The number ocf command-line arguments.
 * @param argv The array of command-line argument strings.
 * @return Returns 0 on successful execution, or exits with a 1 status in the case of errors.
 *
 * @throws std::invalid_argument If command-line arguments are not as expected.
 * @note The program exits if the 'usage()' function is called or if an unhandled exception occurs.
 */
int main(int argc, char* argv[]) {
    if (argc < 2) usage();
    
    std::string file_path = "";           // Path to file to be processed
    std::string content = "";             // Content of file to be processed
    bool emit_IR = false;                 // Flag to check if IR code should be printed
    std::string filename = "output.ll";   // Name of output file
    bool file_path_set = false;           // Flag to check if file path has been set

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            debug << "[DEBUG] Processing argument: " << arg << std::endl;
            if (arg[0] == '-') {
                if (arg == "-r") {
                    if (i + 1 < argc) {
                        filename = argv[++i];
                    } else {
                        throw std::invalid_argument("No filename specified for -r option.");
                    }
                } else {
                    process_single_flags(arg.substr(1), emit_IR);
                }
            } else if (!file_path_set) {
                file_path = arg;
                file_path_set = true;
            } else {
                throw std::invalid_argument("Multiple file paths provided.");
            }
        }

        if (file_path.empty()) throw std::invalid_argument("No file path/content provided.");

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        usage();
    } catch (...) {
        std::cerr << "ERROR: Unknown error occurred!" << std::endl;
        usage();
    }

    print_logo();
    debug << "[DEBUG] File path: " << file_path << std::endl;
    debug << "[DEBUG] Output file name: " << filename << std::endl;
    debug << "[DEBUG] Processing file..." << std::endl;
    content = process_file(file_path);
    debug << "[DEBUG] File processed." << std::endl;

    Slang slang(content);
    if (emit_IR) slang.printIR();
    slang.write_to_file(filename);

    return 0;
}
