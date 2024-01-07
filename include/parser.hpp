/**
 * @file parser.hpp
 * @brief Parser for the S-Lang Compiler
 *
 * This file contains the definition of the Parser class responsible for parsing
 * source code. The parser processes the input code, passes it through the
 * Lexer, iterates through the tokens, and constructs an Abstract Syntax Tree
 * (AST) that represents the structure of the program.
 *
 * The Parser class handles the conversion of source code into a structured
 * format, including parsing literals, expressions, statements, declarations,
 * and function definitions.
 *
 * @author Sagar Patel
 * @date 12-21-2023
 *
 * Project: S-Lang Compiler
 */

#ifndef PARSER_HPP
#define PARSER_HPP
#pragma once

#include "ast.hpp"
#include "lexer.hpp"

/**
 * @brief The Parser class for the S-Lang compiler.
 *
 * Parser is responsible for processing the tokens code into an AST (Abstract Syntax
 * Tree).
 */
class Parser {
  private:
    Lexer lexer;         // Handles the lexer aspect of the frontend, converting text it
                         // into tokens.
    Token current_token; // Stores the current token being processed by the
                         // parser.
    int get_op_precedence() const; // Returns precedence of current op token.
  public:
    /**
     * @brief Constructor for the Parser class.
     *
     * Initializes the Parser with the given source code for parsing.
     *
     * @param code A string containing the source code to be parsed.
     */
    Parser(std::string code);

    /**
     * @brief Parses an integer literal from the source code.
     *
     * This method identifies and extracts an integer literal, converting it
     * into a Literal<int> object.
     *
     * @return A Literal<int> representing the parsed integer value.
     */
    Literal<int> parse_int();

    /**
     * @brief Parses a floating-point literal from the source code.
     *
     * This method identifies and extracts a floating-point literal, converting
     * it into a Literal<double> object.
     *
     * @return A Literal<double> representing the parsed floating-point value.
     */
    Literal<double> parse_float();

    /**
     * @brief Parses a boolean literal from the source code.
     *
     * This method identifies and extracts a boolean literal ('true' or
     * 'false'), converting it into a Literal<bool> object.
     *
     * @return A Literal<bool> representing the parsed boolean value.
     */
    Literal<bool> parse_bool();

    /**
     * @brief Parses a character literal from the source code.
     *
     * This method identifies and extracts a character literal, converting it
     * into a Literal<char> object.
     *
     * @return A Literal<char> representing the parsed character.
     */
    Literal<char> parse_char();

    /**
     * @brief Parses a string literal from the source code.
     *
     * This method identifies and extracts a string literal, converting it into
     * a Literal<std::string> object.
     *
     * @return A Literal<std::string> representing the parsed string.
     */
    Literal<std::string> parse_string();

    /**
     * @brief Parses a general expression from the source code.
     *
     * This method handles the parsing of an expression, which could be a
     * combination of literals, identifiers, function calls, or operators. It
     * can parse any data type in the expression variant.
     *
     * @return An Expression object representing the parsed expression.
     */
    Expression parse_expression();

    /**
     * @brief Parses an expression within parentheses from the source code.
     *
     * This method deals with expressions enclosed within parentheses. Essentially
     * calls parse_expression() on the content enclosed in parentheses and ensures that
     * the expression is enclosed within parentheses.
     *
     * @return An Expression object representing the parsed expression within
     * parentheses.
     */
    Expression parse_parentheses_expression();

    /**
     * @brief Parses an atomic type expression from the source code.
     *
     * Atomic type expressions are the simplest og expressions, which are either
     * literals, identifiers or complex characters (eg. "("). The method is responsible
     * for parsing these basic tokens.
     *
     * @return An Expression object representing the parsed atomic type expression.
     */
    Expression parse_atomic_type();

    /**
     * @brief Parses a unary expression from the source code.
     *
     * Unary expressions involve a single operand and an operator (like
     * negation, such as (!2). This method handles the parsing of such
     * expressions, identifying the operator and its operand and constructing
     * the expression for the AST.
     *
     * @return An Expression object representing the parsed unary expression.
     */
    Expression parse_unary_expression();

    /**
     * @brief Parses the right-hand side of a binary operation.
     * This method is used for parsing binary expressions, handling the
     * right-hand side based on the provided operator precedence and the
     * left-hand side expression.
     *
     * @param expression_precedence The precedence of the current operator to
     * guide the parsing.
     * @param lhs The left-hand side Expression already parsed.
     *
     * @return An Expression object representing the right-hand side of the
     * binary operation.
     */
    Expression parse_binary_op_rhs(int expression_precedence, Expression lhs);

    /**
     * @brief Parses a list of arguments in a function call.
     *
     * This method deals with parsing the arguments passed in a function call,
     * handling comma-separated expressions within parentheses.
     *
     * @return A vector of Expression objects, each representing a parsed
     * argument in the call list.
     */
    std::vector<Expression> parse_argument_call_list();

    /**
     * @brief Parses either an identifier or a pluh (function) call.
     *
     * This method distinguishes between a simple identifier and a function
     * call.
     *
     * @return An Expression object representing either an identifier or a
     * parsed function call.
     */
    Expression parse_identifier_or_pluh_call();

    /**
     * @brief Parses a general statement from the source code.
     *
     * This method is a key part of the parsing process, capable of handling
     * different types of statements like assignments, control flow constructs,
     * declarations, etc.
     *
     * @return A Statement object representing the parsed statement.
     */
    Statement parse_statement();

    /**
     * @brief Parses a 'cookup' or 'cookupassign' statement.
     *
     * This method handles the parsing of variable declarations (cookup) or
     * variable declarations and assignments (cookup and assign). It identifies and
     * processes these statements.
     *
     * @return A Statement object representing the parsed cookup or cookupassign
     * statement.
     */
    Statement parse_cookup_or_cookupassign();

    /**
     * @brief Parses an assignment or function call statement.
     *
     * This method is responsible for parsing statements that involve either
     * variable assignments (eg. x = ...) or function calls (eg. x(...)). It
     * differentiates between these cases and processes a Statement to return accordingly.
     *
     * @return A Statement object representing the parsed assignment or function call
     * statement.
     */
    Statement parse_assignment_or_call();

    /**
     * @brief Parses an 'fr_ong_justlikethat' control flow statement.
     *
     * This method handles the parsing of control flow statements that encompass
     * if, elseif, and else constructs found in popular progamming languages.
     *
     * @return A Statement object representing the parsed fr_ong_justlikethat statement.
     */
    Statement parse_fr_ong_justlikethat();

    /**
     * @brief Parses a 'holdUp' statement.
     *
     * This method is responsible for parsing loop statements (holdUp equivalent to while
     * loops in C++).
     *
     * @return A Statement object representing the parsed holdUp statement.
     */
    Statement parse_holdup();

    /**
     * @brief Parses a 'ghost' statement, which is equivalent to a 'break' statement.
     *
     * This method handles the parsing of 'ghost' statements, which are used to exit from
     * loops.
     *
     * @return A Statement object representing the parsed 'ghost' statement.
     */
    Statement parse_ghost();

    /**
     * @brief Parses a 'rizz' statement, which is equivalent to a 'continue' statement.
     *
     * This method is responsible for parsing 'rizz' statements, which are used to skip
     * the current iteration of a loop and continue with the next iteration.
     *
     * @return A Statement object representing the parsed 'rizz' statement.
     */
    Statement parse_rizz();

    /**
     * @brief Parses a 'yeet' statement, which is equivalent to a 'return' statement.
     *
     * This method handles the parsing of 'yeet' statements, which
     * are used to return from a function and return a value.
     *
     * @return A Statement object representing the parsed 'yeet' statement.
     */
    Statement parse_yeet();

    /**
     * @brief Parses a block of code enclosed within curly braces.
     * This method is responsible for parsing a block of code enclosed within
     * curly braces ('{}'). It handles the code contained within the braces and
     * is used for functions or compound statements like loops.
     *
     * @param returntype A reference to a string representing the return type of
     * the code block (if any) to identify possible missing return statements.
     *
     * @return A Statement object representing the parsed curly compound block.
     */
    Statement parse_curly_compound(std::string& returntype);

    /**
     * @brief Parses a function prototype, which defines the function's signature.
     *
     * This method handles the parsing of a function prototype, which
     * includes the function's name, parameters, and return type. It identifies
     * and processes the prototype based on the provided code. This is used by the
     * parse_pluh() and parse_plug() methods.
     *
     * @return A Prototype object representing the parsed function prototype.
     */
    Prototype parse_prototype();

    /**
     * @brief Parses a function declaration (pluh).
     *
     * This method is responsible for parsing function declarations (pluh),
     * which defines the behavior of a function. It identifies and processes
     * these declarations, including the function name, parameters, return type,
     * and code block statement.
     *
     * @return A PluhDeclaration object representing the parsed function
     * declaration.
     */
    PluhDeclaration parse_pluh();

    /**
     * @brief Parses an extern function declaration (plug).
     *
     * This method handles the parsing of extern function declarations (plug),
     * which declare a function without defining its behavior as the definition may come
     * from another file! It identifies and processes these declarations, including the
     * function name, parameters, and return type.
     *
     * @return A PluhDeclaration object representing the parsed extern function
     * declaration.
     */
    PluhDeclaration parse_plug();

    /**
     * @brief Parses all declarations in the code.
     *
     * This method is responsible for parsing different kinds of declarations
     * (like pluh's and plugs) represented by the 'PluhDeclaration'
     * variant. It handles multiple declarations and organizes them into a
     * vector.
     *
     * @return A vector of std::variant<PluhDeclaration>, each element
     * representing a parsed declaration.
     */
    std::vector<std::variant<PluhDeclaration>> parse_declarations();

    /**
     * @brief Parses the entire program, forming the root of the Abstract Syntax Tree.
     *
     * The 'TeaSpill' class represents the entire parsed program, having all
     * elements such as declarations, statements, expressions, etc. as children. This
     * method has all declarations as direct childrens to this instance's root node.
     *
     * @return A TeaSpill object that represents the entire parsed program.
     */
    TeaSpill parse_tea();
};

#endif
