#include "parser.hpp"

Parser::Parser(std::string code) : lexer(code) {
    // Fetch the first token from the lexer to start parsing.
    current_token = lexer.get_token();
}

int Parser::get_op_precedence() const {
    // Determine the precedence of the current operator token.
    
    // Define precedence for comparison operators.
    if (current_token.second == "<" || current_token.second == "<=" ||
        current_token.second == ">" || current_token.second == ">=" ||
        current_token.second == "==" || current_token.second == "!=") {
        return 10;
    } 
    // Define precedence for addition and subtraction.
    else if (current_token.second == "+" || current_token.second == "-") {
        return 20;
    } 
    // Define precedence for multiplication, division, and modulo.
    else if (current_token.second == "*" || current_token.second == "/" ||
             current_token.second == "%") {
        return 40;
    } 
    // Return a low precedence for other cases.
    else {
        return -1;
    }
}

Literal<int> Parser::parse_int() {
    debug << "[DEBUG] Parsing int!" << std::endl;
    try {
        // Convert the current token to an integer value.
        int value = std::stoi(current_token.second);

        // Create a Literal node with the integer value.
        Literal<int> node(value);

        // Move to the next token.
        current_token = lexer.get_token();

        return node;
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing int: " << current_token.second
                  << std::endl;
        exit(1);
    }
}

Literal<double> Parser::parse_float() {
    debug << "[DEBUG] Parsing float!" << std::endl;
    try {
        // Convert the current token to a double value.
        double value = std::stod(current_token.second);

        // Create a Literal node with the float value.
        Literal<double> node(value);

        // Move to the next token.
        current_token = lexer.get_token();

        return node;
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing float: " << current_token.second
                  << std::endl;
        exit(1);
    }
}

Literal<bool> Parser::parse_bool() {
    debug << "[DEBUG] Parsing bool!" << std::endl;
    try {
        // Convert the current token to a boolean value.
        bool value = current_token.second == "true";

        // Create a Literal node with the boolean value.
        Literal<bool> node(value);

        // Move to the next token.
        current_token = lexer.get_token();

        return node;
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing bool: " << current_token.second
                  << std::endl;
        exit(1);
    }
}

Literal<char> Parser::parse_char() {
    debug << "[DEBUG] Parsing char!" << std::endl;
    try {
        // Convert the current token to a char value.
        char value = current_token.second[0];

        // Create a Literal node with the char value.
        Literal<char> node(value);

        // Move to the next token.
        current_token = lexer.get_token();

        return node;
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing char: " << current_token.second
                  << std::endl;
        exit(1);
    }
}

Literal<std::string> Parser::parse_string() {
    debug << "[DEBUG] Parsing string!" << std::endl;
    try {
        // Store the current token as a string value.
        std::string value = current_token.second;

        // Create a Literal node with the string value.
        Literal<std::string> node(value);

        // Move to the next token.
        current_token = lexer.get_token();

        return node;
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing string: " << current_token.second
                  << std::endl;
        exit(1);
    }
}

Expression Parser::parse_expression() {
    debug << "[DEBUG] Parsing expression!" << std::endl;
    try {
        // Parse the left-hand side as a unary expression.
        auto lhs = parse_unary_expression();

        // Parse the right-hand side as a binary operation (if applicable).
        return parse_binary_op_rhs(0, std::move(lhs));
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing expression!" << std::endl;
        exit(1);
    }
}

Expression Parser::parse_parentheses_expression() {
    debug << "[DEBUG] Parsing parentheses expression!" << std::endl;
    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Parse the expression inside the parentheses.
        auto node = parse_expression();

        // Ensure the closing parenthesis is present.
        if (current_token.second != ")") {
            throw parse_logic_error("Expected ) parsing parentheses expression, got: " +
                                    current_token.second);
        }

        // Move past the closing parenthesis.
        current_token = lexer.get_token();

        return node;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing parentheses expression!"
                  << std::endl;
        exit(1);
    }
}

Expression Parser::parse_atomic_type() {
    debug << "[DEBUG] Parsing atomic type!" << std::endl;
    try {
        // Use a switch-case to determine the type of atomic expression to parse.
        switch (current_token.first) {
            case TokenType::IDENTIFIER:
                // Parse an identifier or function call.
                return parse_identifier_or_pluh_call();
            case TokenType::INT:
                // Parse an integer literal.
                return parse_int();
            case TokenType::FLOAT:
                // Parse a float literal.
                return parse_float();
            case TokenType::BOOL:
                // Parse a boolean literal.
                return parse_bool();
            case TokenType::CHAR:
                // Parse a char literal.
                return parse_char();
            case TokenType::STRING:
                // Parse a string literal.
                return parse_string();
            case TokenType::COMPLEX:
                // Parse a complex type, mostly involving parentheses.
                if (current_token.second == "(") {
                    return parse_parentheses_expression();
                } else {
                    throw parse_logic_error("Expected ( parsing a complex type, got: " +
                                            current_token.second);
                }
            default:
                throw parse_logic_error("Unknown token parsing atomic type, got: " +
                                        current_token.second);
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing atomic type!" << std::endl;
        exit(1);
    }
}


Expression Parser::parse_unary_expression() {
    debug << "[DEBUG] Parsing unary expression!" << std::endl;
    try {
        // Check if the current token is a unary operator (+, -, !).
        if (current_token.second == "+" || current_token.second == "-" ||
            current_token.second == "!") {
            
            // Store the operator.
            auto op = current_token;
            current_token = lexer.get_token(); // Move to the next token.

            // Check for invalid application of unary operators to char or string.
            if (current_token.first == TokenType::CHAR ||
                current_token.first == TokenType::STRING) {
                throw parse_logic_error(
                    "Unary operator cannot be applied to char or string: " +
                    current_token.second);
            }

            // Recursively parse the right-hand side as a unary expression.
            auto rhs = parse_unary_expression();

            // Construct and return a UnaryExpression.
            return std::make_unique<UnaryExpression>(op.second, std::move(rhs));
        }

        // If not a unary operator, parse as an atomic type.
        return parse_atomic_type();

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing unary expression!" << std::endl;
        exit(1);
    }
}


Expression Parser::parse_binary_op_rhs(int expression_precedence, Expression lhs) {
    debug << "[DEBUG] Parsing right-hand side of binary operation!" << std::endl;
    try {
        // Continuously parse the binary operation's right-hand side.
        while (true) {
            // Determine the precedence of the current token.
            int token_precedence = get_op_precedence();

            // If the token's precedence is lower than the expression's precedence, return lhs.
            if (token_precedence < expression_precedence) {
                return lhs;
            }

            // Store the operator.
            auto op = current_token;
            current_token = lexer.get_token(); // Move to the next token.

            // Parse the right-hand side as a unary expression.
            auto rhs = std::move(parse_unary_expression());

            // Get the precedence of the next operator.
            int next_precedence = get_op_precedence();

            // If the current token's precedence is lower than the next, recursively parse the rhs.
            if (token_precedence < next_precedence) {
                rhs = parse_binary_op_rhs(token_precedence + 1, std::move(rhs));
            }

            // Combine lhs and rhs into a new lhs as a BinaryExpression.
            lhs = std::make_unique<BinaryExpression>(op.second, std::move(lhs), std::move(rhs));
        }
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception parsing right-hand side of binary operation!"
                  << std::endl;
        exit(1);
    }
}


std::vector<Expression> Parser::parse_argument_call_list() {
    // Debug message for starting the parsing of an argument call list.
    debug << "[DEBUG] Parsing argument call list!" << std::endl;

    try {
        // Initialize an empty vector to store the arguments.
        std::vector<Expression> args = {};

        // Check if the current token is not a closing parenthesis.
        if (current_token.second != ")") {
            while (true) {
                // Parse each argument as an expression and add it to the args vector.
                auto arg = parse_expression();
                args.push_back(std::move(arg));

                // Check for the end of the argument list or a comma for another argument.
                if (current_token.second == ")") {
                    break;
                } else if (current_token.second != ",") {
                    throw parse_logic_error(
                        "Expected , or ) parsing argument call list, got: " +
                        current_token.second);
                }
                // Move to the next token.
                current_token = lexer.get_token();
            }
        }

        // Move past the closing parenthesis of the argument list.
        current_token = lexer.get_token();

        // Return the parsed list of arguments.
        return args;

    } catch (const std::exception& e) {
        // Catch and log standard exceptions, then exit.
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        // Catch any other types of exceptions and handle them similarly.
        std::cerr << "[ERROR] Unknown exception parsing argument call list!" << std::endl;
        exit(1);
    }
}


Expression Parser::parse_identifier_or_pluh_call() {
    // Debug message for parsing either an identifier or a function call.
    debug << "[DEBUG] Parsing identifier or function call!" << std::endl;

    try {
        // Store the identifier from the current token.
        std::string identifier = current_token.second;

        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Check if the current token is not an opening parenthesis.
        if (current_token.second != "(") {
            // If it's not, treat the identifier as a variable and return a VariableExpression.
            return std::make_unique<VariableExpression>(identifier);
        }

        // Otherwise, parse the argument list for the function call.
        current_token = lexer.get_token();
        return std::make_unique<CallExpression>(
            identifier, std::move(parse_argument_call_list()));

    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing identifier or function call!"
                  << std::endl;
        exit(1);
    }
}

Statement Parser::parse_statement() {
    // Debug message for starting the parsing of a statement.
    debug << "[DEBUG] Parsing statement!" << std::endl;

    try {
        // Define a default return type for statements that require it.
        std::string default_returntype = "void";

        // Switch based on the type of the current token to determine the kind of statement to parse.
        switch (current_token.first) {
            case TokenType::LET:
                // Parse a cookup or cookup assignment statement.
                return parse_cookup_or_cookupassign();
            case TokenType::IDENTIFIER:
                // Parse an assignment statement or a function call.
                return parse_assignment_or_call();
            case TokenType::IF:
                // Parse an 'if' statement (fr, ong, just-like-that statement).
                return parse_fr_ong_justlikethat();
            case TokenType::WHILE:
                // Parse a 'while' loop statement (holdup).
                return parse_holdup();
            case TokenType::BREAK:
                // Parse a 'break' statement (ghost).
                return parse_ghost();
            case TokenType::CONTINUE:
                // Parse a 'continue' statement (rizz).
                return parse_rizz();
            case TokenType::RETURN:
                // Parse a 'return' statement (yeet).
                return parse_yeet();
            case TokenType::COMPLEX:
                // Parse a compound statement enclosed in curly braces.
                return parse_curly_compound(default_returntype);
            default:
                // If the token does not match any known statement type, throw an error.
                throw parse_logic_error("Unknown token parsing statement, got: " +
                                        current_token.second);
        }
    } catch (const std::exception& e) {
        // Catch and log standard exceptions, then exit.
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        // Catch any other types of exceptions and handle them similarly.
        std::cerr << "[ERROR] Unknown exception parsing statement!" << std::endl;
        exit(1);
    }
}


Statement Parser::parse_cookup_or_cookupassign() {
    // Debug message indicating the start of parsing a 'let' statement to declare or assign a variable.
    debug << "[DEBUG] Parsing to let him cook up a variable!" << std::endl;

    try {
        // Move to the next token to get the variable name.
        current_token = lexer.get_token();
        auto var_name = current_token.second;

        // Move to the next tokens to get the type name of the variable.
        current_token = lexer.get_token();
        current_token = lexer.get_token();
        auto type_name = current_token.second;

        // Check if the next token is an equals sign, indicating an assignment.
        current_token = lexer.get_token();
        if (current_token.second == "=") {
            // If it's an assignment, parse the right-hand side expression.
            current_token = lexer.get_token();
            auto expr = parse_expression();

            // Create and return a CookedUpAssignmentStatement with variable name, type, and expression.
            return std::make_unique<CookedUpAssignmentStatement>(var_name, type_name, std::move(expr));
        }

        // If there's no assignment, return a CookedUpStatement with just variable name and type.
        return std::make_unique<CookedUpStatement>(var_name, type_name);
    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing to let him cook up a variable!"
                  << std::endl;
        exit(1);
    }
}


Statement Parser::parse_assignment_or_call() {
    // Debug message for parsing an assignment or a pluh call.
    debug << "[DEBUG] Parsing assignment or pluh call!" << std::endl;

    try {
        // Store the variable name from the current token.
        auto var_name = current_token.second;

        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Check if the next token indicates an assignment.
        if (current_token.second == "=") {
            // Parse the right-hand side expression of the assignment.
            current_token = lexer.get_token();
            auto expr = parse_expression();

            // Return an AssignmentStatement with the variable and expression.
            return std::make_unique<AssignmentStatement>(var_name, std::move(expr));
        } 
        // Check if the next token indicates a function call.
        else if (current_token.second == "(") {
            // Parse the argument list for the function call.
            current_token = lexer.get_token();
            return std::make_unique<AssignmentStatement>(
                "@", std::make_unique<CallExpression>(
                         var_name, std::move(parse_argument_call_list())));
        }

        // Throw an error if neither assignment nor function call syntax is found.
        throw parse_logic_error("Expected = or ( parsing assignment or call, got: " +
                                current_token.second);

    } catch (const std::exception& e) {
        // Catch standard exceptions, log them, and exit.
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        // Catch any other types of exceptions and handle them similarly.
        std::cerr << "[ERROR] Unknown exception parsing assignment or call!" << std::endl;
        exit(1);
    }
}


Statement Parser::parse_fr_ong_justlikethat() {
    // Debug message for parsing a 'fr, ong, just-like-that' statement.
    debug << "[DEBUG] Parsing a fr, ong, just-like-that statement!" << std::endl;

    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Parse the condition for the statement.
        auto condition = parse_expression();

        // Define a default return type for the 'then' part of the statement.
        std::string default_returntype = "void";

        // Parse the 'then' part of the statement.
        auto then_stmt = parse_curly_compound(default_returntype);

        // Check for the presence of an 'else' part.
        if (current_token.first != TokenType::ELSE) {
            // If 'else' part is absent, return the statement with an empty 'else' part.
            return std::make_unique<FrOngJustLikeThatStatement>(
                std::move(condition), std::move(then_stmt),
                std::make_unique<CompoundStatement>(std::vector<Statement>()));
        } else {
            // If 'else' part is present, parse it.
            current_token = lexer.get_token();
            auto else_stmt = parse_statement();

            // Return the full statement with both 'then' and 'else' parts.
            return std::make_unique<FrOngJustLikeThatStatement>(
                std::move(condition), std::move(then_stmt), std::move(else_stmt));
        }
    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing fr, ong, just-like-that statement!"
                  << std::endl;
        exit(1);
    }
}

Statement Parser::parse_holdup() {
    // Debug message for parsing a holdup statement.
    debug << "[DEBUG] Parsing a holdup statement!" << std::endl;
    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Parse the condition for the holdup statement.
        auto condition = parse_expression();

        // Define a default return type for the holdup's body.
        std::string default_returntype = "void";

        // Parse the body of the holdup statement.
        auto body = parse_curly_compound(default_returntype);

        // Return a unique pointer to the constructed HoldUpStatement.
        return std::make_unique<HoldUpStatement>(std::move(condition), std::move(body));
    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing holdup statement!" << std::endl;
        exit(1);
    }
}

Statement Parser::parse_ghost() {
    // Debug message for parsing a ghost statement.
    debug << "[DEBUG] Parsing getting ghosted!" << std::endl;
    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Return a unique pointer to a new GhostStatement.
        return std::make_unique<GhostStatement>();
    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing ghost statement!" << std::endl;
        exit(1);
    }
}

Statement Parser::parse_rizz() {
    // Debug message for parsing a rizz statement.
    debug << "[DEBUG] Parsing having rizz!" << std::endl;
    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Return a unique pointer to a new RizzStatement.
        return std::make_unique<RizzStatement>();
    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing rizz statement!" << std::endl;
        exit(1);
    }
}

Statement Parser::parse_yeet() {
    // Debug message for parsing a yeet statement.
    debug << "[DEBUG] Parsing what ur yeeting!" << std::endl;
    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Parse the expression to be yeeted.
        auto expr = parse_expression();

        // Return a unique pointer to a new YeetStatement with the parsed expression.
        return std::make_unique<YeetStatement>(std::move(expr));
    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing yeet statement!" << std::endl;
        exit(1);
    }
}


Statement Parser::parse_curly_compound(std::string& returntype) {
    // Debug message for starting parsing of a compound statement enclosed in curly braces.
    debug << "[DEBUG] Parsing curly brace compound statement!" << std::endl;

    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Initialize a vector to store the parsed statements.
        std::vector<Statement> statements = {};

        // Continue parsing statements until a closing curly brace is encountered.
        while (current_token.second != "}") {
            statements.push_back(parse_statement());
        }

        // Determine the return type based on the nature of the last statement.
        if ((!statements.empty()) &&
            (std::holds_alternative<std::unique_ptr<YeetStatement>>(statements.back()))) {
            // Set return type to "nonnpc" if the last statement is a unique pointer to a YeetStatement.
            returntype = "nonnpc";
        } else {
            // Otherwise, set return type to "npc".
            returntype = "npc";
        }

        // Fetch the next token after parsing the compound statement.
        current_token = lexer.get_token();

        // Return a unique pointer to the constructed CompoundStatement.
        return std::make_unique<CompoundStatement>(std::move(statements));

    } catch (...) {
        // Catch any exception, log an error message, and exit.
        std::cerr << "[ERROR] Unknown exception parsing compound statement!" << std::endl;
        exit(1);
    }
}

Prototype Parser::parse_prototype() {
    // Debug message indicating the start of prototype parsing.
    debug << "[DEBUG] Parsing prototype!" << std::endl;

    try {
        // Ensure the current token is an identifier, as expected at the start of a prototype.
        if (current_token.first != TokenType::IDENTIFIER) {
            throw parse_logic_error("Expected identifier in prototype, instead got: " +
                                    current_token.second);
        }
        // Store the function name from the current token.
        std::string func_name = current_token.second;
        current_token = lexer.get_token();

        // Check for an opening parenthesis after the function name.
        if (current_token.second != "(") {
            throw parse_logic_error("Expected ( in prototype, instead got: " +
                                    current_token.second);
        }

        // Parse and store arguments of the function.
        std::vector<Argument> argument_names = {};
        while ((current_token = lexer.get_token()).first == TokenType::IDENTIFIER) {
            auto var_name = current_token.second;
            if ((current_token = lexer.get_token()).second != ":") {
                throw parse_logic_error(
                    "Expected : after argument name in prototype, instead got: " +
                    current_token.second);
            }
            auto type_name = (current_token = lexer.get_token()).second;
            argument_names.push_back({var_name, type_name});
            if ((current_token = lexer.get_token()).second != ",") {
                break;
            }
        }

        // Check for a closing parenthesis after arguments.
        if (current_token.second != ")") {
            throw parse_logic_error("Expected ) in prototype, instead got: " +
                                    current_token.second);
        }

        // Check for a colon after the closing parenthesis, followed by return type.
        if ((current_token = lexer.get_token()).second != ":") {
            throw parse_logic_error("Expected : after args in prototype, instead got: " +
                                    current_token.second);
        }
        auto return_type = (current_token = lexer.get_token()).second;
        current_token = lexer.get_token();

        // Construct and return a Prototype object.
        return Prototype(func_name, std::move(argument_names), return_type);

    } catch (const std::exception& e) {
        // Catch and log standard exceptions, then exit.
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        // Catch and handle any other exceptions in a similar way.
        std::cerr << "[ERROR] Unknown exception parsing prototype!" << std::endl;
        exit(1);
    }
}


PluhDeclaration Parser::parse_pluh() {
    // Debug message for starting to parse a pluh.
    debug << "[DEBUG] Parsing a pluh!" << std::endl;

    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Parse the prototype for the pluh.
        auto proto = parse_prototype();

        // Retrieve the expected return type from the parsed prototype.
        std::string expected_returntype = proto.get_return_type();
        std::string actual_returntype = "";

        // Parse the compound statement with actual return type being updated.
        auto stmt = parse_curly_compound(actual_returntype);

        // Check if return types mismatch and throw an error if they do.
        if ((expected_returntype != "npc" && actual_returntype == "npc") ||
            (expected_returntype == "npc" && actual_returntype == "nonnpc")) {
            throw parse_logic_error("Expected return type " + expected_returntype +
                                    " for pluh: " + proto.get_name());
        }

        // Construct and return a PluhDeclaration object.
        return PluhDeclaration(std::move(proto), std::move(stmt));

    } catch (const std::exception& e) {
        // Catch standard exceptions, log them, and exit.
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        // Catch any other types of exceptions and handle them similarly.
        std::cerr << "[ERROR] Unknown exception parsing function declaration"
                  << std::endl;
        exit(1);
    }
}


PluhDeclaration Parser::parse_plug() {
    // Debug message indicating the start of parsing a plug.
    debug << "[DEBUG] Parsing a plug!" << std::endl;

    try {
        // Fetch the next token using the lexer.
        current_token = lexer.get_token();

        // Parse the prototype for the plug and store it in 'proto'.
        auto proto = parse_prototype();

        // Create a PluhDeclaration object using the parsed prototype.
        auto func = PluhDeclaration(std::move(proto));

        // Return the constructed PluhDeclaration object.
        return func;
    } catch (...) {
        // Catch any exception and print an error message, then exit.
        std::cerr << "[ERROR] Unknown exception parsing a plug!" << std::endl;
        exit(1);
    }
}

std::vector<std::variant<PluhDeclaration>> Parser::parse_declarations() {
    // Debug message for the start of parsing declarations.
    debug << "[DEBUG] Parsing all pluh's and plugs!" << std::endl;

    try {
        // Initialize an empty vector to store the declarations.
        std::vector<std::variant<PluhDeclaration>> declarations = {};

        while (true) {
            // Parsing based on the type of the current token.
            if (current_token.first == TokenType::DEF) {
                // If it's a 'DEF' token, parse a pluh and add it to declarations.
                declarations.push_back(std::move(parse_pluh()));
            } else if (current_token.first == TokenType::EXTERN) {
                // If it's an 'EXTERN' token, parse a plug and add it to declarations.
                declarations.push_back(std::move(parse_plug()));
            } else if (current_token.first == TokenType::END_OF_FILE) {
                // If it's the end of file token, return all parsed declarations.
                return declarations;
            } else {
                // If an unexpected token is encountered, throw an error.
                throw parse_logic_error("Expected pluh or plug parsing all declarations!");
            }
        }
    } catch (const std::exception& e) {
        // Handle standard exceptions by logging the error and exiting.
        std::cerr << "[ERROR] " << e.what() << std::endl;
        exit(1);
    } catch (...) {
        // Handle non-standard exceptions similarly.
        std::cerr << "[ERROR] Unknown exception parsing all pluh's!" << std::endl;
        exit(1);
    }
}

TeaSpill Parser::parse_tea() {
    // Output a debug message indicating the start of the tea parsing process.
    debug << "[DEBUG] Parsing the Tea!" << std::endl;

    try {
        // Check if the current token is of type PROGRAM, which is expected at the start.
        if (current_token.first != TokenType::PROGRAM) {
            // If not, throw a parse logic error with a descriptive message.
            throw parse_logic_error("Expected program for parsing Tea!");
        }

        // Retrieve the next token from the lexer.
        current_token = lexer.get_token();

        // Store the second element of the current token, which is expected to be the name.
        std::string name = current_token.second;

        // Retrieve the next token after obtaining the name.
        current_token = lexer.get_token();

        // Parse the declarations in the tea spill and store them in a vector of variants.
        std::vector<std::variant<PluhDeclaration>> decls = parse_declarations();

        // Construct a TeaSpill object with the parsed name and declarations, and return it.
        return TeaSpill(name, std::move(decls));

    } catch (const std::exception& e) {
        // If a standard exception is caught, output an error message with the exception details.
        std::cerr << "[ERROR] " << e.what() << std::endl;
        // Exit the program with an error status.
        exit(1);
    } catch (...) {
        // Catch any other types of exceptions that are not covered by std::exception.
        std::cerr << "[ERROR] Unknown exception parsing Tea!" << std::endl;
        // Exit the program with an error status.
        exit(1);
    }
}

