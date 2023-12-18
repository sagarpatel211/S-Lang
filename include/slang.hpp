/**
 * @file slang.hpp
 * @brief Main Interface for the S-Lang Compiler
 *
 * This file defines the Slang class, which serves as the main interface for the S-Lang compiler.
 * The Slang class integrates different components of the compiler, including the lexer, parser,
 * abstract syntax tree (AST), and code generation. It provides methods to process the source code,
 * generate intermediate representation (IR), and output the results to a file.
 *
 * @author Sagar Patel
 * @date 12-10-2023
 *
 * Project: S-Lang Compiler
 */

#ifndef SLANG_HPP
#define SLANG_HPP
#pragma once

#include "ast.hpp"
#include "codegen.hpp"
#include "debug_stream.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <sstream>

/**
 * @brief The main compiler class for the S-Lang language.
 *
 * The Slang class encapsulates the entire process of compiling S-Lang source code.
 * It integrates the lexer, parser, AST handling, and code generation phases.
 * It provides an interface to input source code, generate IR, and output the results.
 */
class Slang {
  private:
	std::string code;	 // Source code to be compiled.
	Parser parser;	  	 // Lexer for tokenizing the source code.
	Codegen irgen;	  	 // Codegen for generating IR from the AST.
	std::string llvm_ir; // The generated IR in string form.
  public:
	/**
	 * @brief Construct a new Slang instance with given source code.
	 *
	 * @param code The source code as a string.
	 */
	Slang(const std::string& code);

	/**
	 * @brief Print the Intermediate Representation (IR) of the compiled source code.
	 */
	void print_IR() const;

	/**
	 * @brief Write the compiled output to a file.
	 *
	 * @param filename The name of the file to write the output to.
	 */
	void write_to_file(const std::string& filename);

	/**
	 * @brief Default destructor.
	 */
	~Slang() = default;
};

#endif
