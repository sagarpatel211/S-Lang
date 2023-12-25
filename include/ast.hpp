/**
 * @file ast.hpp
 * @brief Abstract Syntax Tree for the S-Lang Compiler
 *
 * This file contains the definitions of  classes that make up the abstract
 * abstract syntax tree (AST) used in the S-Lang compiler. The AST is
 * essentially the last step of the frontend for a compiler, representing the
 * structure of the parsed code in a way that can be processed for code
 * generation.
 *
 * The AST includes classes for literals, expressions, statements, function
 * declarations, and so on. Each class is designed to encapsulate a specific
 * element of the syntax of the S-Lang programming language.
 *
 * Usage of these classes involves constructing instances representing different
 * parts of the source code and linking them together, which is done in the
 * parser, to form the complete tree structure of a program.
 *
 * @author Sagar Patel
 * @date 12-17-2023
 *
 * Project: S-Lang Compiler
 */

#ifndef AST_HPP
#define AST_HPP
#pragma once

#include "debug_stream.hpp"
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

// Forward declarations for classes that are used in the AST
template <typename T>
class Literal;
class VariableExpression;
class UnaryExpression;
class BinaryExpression;
class CallExpression;
class CookedUpStatement;
class AssignmentStatement;
class CookedUpAssignmentStatement;
class FrOngJustLikeThatStatement;
class HoldUpStatement;
class CompoundStatement;
class YeetStatement;
class RizzStatement;
class GhostStatement;
class PluhDeclaration;
class Prototype;
class TeaSpill;

/**
 * @brief Compares two std::unique_ptr objects for equality.
 *
 * This function template provides an overloaded equality operator (==) for
 * comparing two std::unique_ptr objects. It checks if the pointers are
 * non-null and then compares the objects they point to. If both pointers are
 * non-null, it dereferences them and compares the objects. If either or both
 * are null, it directly compares the pointer values.
 *
 * @param p1 The first std::unique_ptr to compare.
 * @param p2 The second std::unique_ptr to compare.
 *
 * @return true if both pointers are null, or if they are non-null and the
 *         objects they point to are equal; false otherwise.
 */
template <typename T>
inline bool operator==(const std::unique_ptr<T>& p1, const std::unique_ptr<T>& p2) {
    if (p1 != nullptr && p2 != nullptr) {
        return *p1 == *p2;
    }
    return p1 == p2;
}

/**
 * @brief Represents a generic literal value of a specified type.
 * This template class encapsulates a literal value of type T. It provides
 * functionality to store, retrieve, and compare literal values. The class
 * supports move semantics.
 *
 * @tparam T The type of the literal value this class will hold.
 */
template <typename T>
class Literal {
  private:
    T value; // The encapsulated literal value of type T
  public:
    /**
     * @brief Default move constructor.
     *
     * Allows moving of a Literal object. This is defaulted (C++ 20).
     */
    Literal(Literal<T>&&) = default;

    /**
     * @brief Constructor that initializes the Literal with a value.
     *
     * @param value The value to initialize the Literal with.
     */
    explicit Literal(T value);

    /**
     * @brief Gets the encapsulated literal value.
     *
     * @return The literal value of type T.
     */
    T get_value() const;

    /**
     * @brief Default move assignment operator.
     *
     * Allows move assignment of a Literal object. This is defaulted (C++ 20).
     *
     * @return A reference to the modified Literal object.
     */
    Literal<T>& operator=(Literal<T>&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this Literal object with another for equality. This
     * is defaulted (C++ 20).
     *
     * @param rhs The right-hand side Literal object to compare with.
     *
     * @return true if both Literal objects are equal, false otherwise.
     */
    bool operator==(const Literal<T>& rhs) const = default;
};

/**
 * @brief Represents a variant type for different kinds of expressions.
 * This alias defines 'Expression' as a std::variant that holds
 * different types of Expression objects. Each type represents a different
 * kind of expression in SLang.
 *
 * Types:
 * - Literal<int>: Represents an integer literal.
 * - Literal<double>: Represents a floating-point literal.
 * - Literal<bool>: Represents a boolean literal.
 * - Literal<char>: Represents a character literal.
 * - Literal<std::string>: Represents a string literal.
 * - std::unique_ptr<VariableExpression>: Represents an expression dealing with a
 * variable.
 * - std::unique_ptr<UnaryExpression>: Represents an expression with a single operand.
 * - std::unique_ptr<BinaryExpression>: Represents an expression with two operands.
 * - std::unique_ptr<CallExpression>: Represents a function call expression.
 *
 * We use std::variant for type-safe expression handling :) ASTs can
 * get tricky with raw pointers!
 */
using Expression =
    std::variant<Literal<int>, Literal<double>, Literal<bool>, Literal<char>,
                 Literal<std::string>, std::unique_ptr<VariableExpression>,
                 std::unique_ptr<UnaryExpression>, std::unique_ptr<BinaryExpression>,
                 std::unique_ptr<CallExpression>>;

/**
 * @brief Represents an expression that encapsulates a variable.
 *
 * This class is designed to represent a variable in an AST tree. It only holds
 * the name of the variable. The class supports move semantics. It provides
 * functionality to store, retrieve, and compare instances.
 */
class VariableExpression {
  private:
    std::string name; // The name of the variable
  public:
    /**
     * @brief Default move constructor.
     *
     * Allows moving of a VariableExpression object. The operation is defaulted
     * (C++ 20).
     */
    VariableExpression(VariableExpression&&) = default;

    /**
     * @brief Constructor that initializes the VariableExpression with a name.
     *
     * @param name The name of the variable to initialize the VariableExpression
     * with.
     */
    explicit VariableExpression(const std::string& name);

    /**
     * @brief Gets the name of the variable.
     *
     * @return A std::string representing the name of the variable.
     */
    std::string get_name() const;

    /**
     * @brief Default move assignment operator.
     *
     * Allows move assignment of a VariableExpression object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified VariableExpression object.
     */
    VariableExpression& operator=(VariableExpression&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this VariableExpression object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side VariableExpression object to compare with.
     *
     * @return true if both VariableExpression objects are equal, false
     * otherwise.
     */
    bool operator==(const VariableExpression& rhs) const = default;
};

/**
 * @brief Represents a unary expression in an abstract syntax tree
 *
 * This class stores a unary expression, which consists of a single operator
 * and a right-hand side expression (rhs). A common unary operation is negation.
 */
class UnaryExpression {
  private:
    std::string op; // The unary operator represented as a string.
    Expression rhs; // The right-hand side expression of the unary operation.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving a UnaryExpression object. The operation is defaulted (C++
     * 20).
     */
    UnaryExpression(UnaryExpression&&) = default;

    /**
     * @brief Constructor that initializes the UnaryExpression with an operator
     * and an expression.
     *
     * @param op The unary operator as a string.
     * @param rhs The right-hand side expression for the unary operation.
     */
    explicit UnaryExpression(const std::string& op, Expression rhs);

    /**
     * @brief Gets the unary operator.
     *
     * @return A std::string representing the unary operator.
     */
    std::string get_op() const;

    /**
     * @brief Gets a reference to the right-hand side expression.
     *
     * @return A reference to the right-hand side Expression object.
     */
    Expression& get_rhs();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a UnaryExpression object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified UnaryExpression object.
     */
    UnaryExpression& operator=(UnaryExpression&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this UnaryExpression object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side UnaryExpression object to compare with.
     *
     * @return true if both UnaryExpression objects are equal, false otherwise.
     */
    bool operator==(const UnaryExpression& rhs) const = default;
};

/**
 * @brief Represents a binary expression in an abstract syntax tree
 *
 * This class encapsulates a binary expression, which consists of an operator
 * and two expressions (left-hand side and right-hand side). A common binary
 * is addition.
 */
class BinaryExpression {
  private:
    std::string op; // The binary operator represented as a string.
    Expression lhs; // The left-hand side expression of the binary operation.
    Expression rhs; // The right-hand side expression of the binary operation.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables the moving of a BinaryExpression object. The operation is
     * defaulted (C++ 20).
     */
    BinaryExpression(BinaryExpression&&) = default;

    /**
     * @brief Constructor that initializes the BinaryExpression with an operator
     * and two expressions.
     *
     * @param op The binary operator as a string.
     * @param lhs The left-hand side expression for the binary operation.
     * @param rhs The right-hand side expression for the binary operation.
     */
    explicit BinaryExpression(const std::string& op, Expression lhs, Expression rhs);

    /**
     * @brief Gets the binary operator.
     *
     * @return A std::string representing the binary operator.
     */
    std::string get_op() const;

    /**
     * @brief Gets a reference to the left-hand side expression.
     *
     * @return A reference to the left-hand side Expression object.
     */
    Expression& get_lhs();

    /**
     * @brief Gets a reference to the right-hand side expression.
     *
     * @return A reference to the right-hand side Expression object.
     */
    Expression& get_rhs();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a BinaryExpression object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified BinaryExpression object.
     */
    BinaryExpression& operator=(BinaryExpression&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this BinaryExpression object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side BinaryExpression object to compare with.\
     *
     * @return true if both BinaryExpression objects are equal, false otherwise.
     */
    bool operator==(const BinaryExpression& rhs) const = default;
};

/**
 * @brief Represents a function call expression in an abstract syntax tree
 *
 * This class encapsulates a call expression, which consists of a callee (the
 * function being called) and a list of arguments.
 */
class CallExpression {
  private:
    std::string callee;                // The name of the callee or function being called.
    std::vector<Expression> arguments; // list of args passed to function.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a CallExpression object. The operation is defaulted
     * (C++ 20).
     */
    CallExpression(CallExpression&&) = default;

    /**
     * @brief Constructor that initializes the CallExpression with a callee and
     * a list of arguments.
     *
     * @param callee The name of the callee or function.
     * @param arguments The list of arguments to be passed to the function.
     */
    explicit CallExpression(const std::string& callee, std::vector<Expression> arguments);

    /**
     * @brief Gets the callee's name.
     *
     * @return A std::string representing the name of the callee or function.
     */
    std::string get_callee() const;

    /**
     * @brief Gets a reference to the list of arguments.
     *
     * @return A reference to a std::vector containing the Expression objects
     * representing the arguments.
     */
    std::vector<Expression>& get_arguments();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a CallExpression object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified CallExpression object.
     */
    CallExpression& operator=(CallExpression&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this CallExpression object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side CallExpression object to compare with.
     *
     * @return true if both CallExpression objects are equal, false otherwise.
     */
    bool operator==(const CallExpression& rhs) const = default;
};

/**
 * @brief Type alias representing an argument as a pair of strings.
 *
 * This type alias defines 'Argument' as a std::pair, where the first element
 * represents the name of the argument and the second element represents its
 * value. Both the name and the value are stored as std::string.
 */
using Argument = std::pair<std::string, std::string>;

/**
 * @brief Represents the prototype of a function in an abstract syntax tree.
 *
 * This class encapsulates the prototype of a function, which includes the
 * function's name, a list of its arguments, and its return type. Each argument
 * is represented as an 'Argument', which is a pair of strings specifying the
 * argument's name and type.
 */
class Prototype {
  private:
    std::string name;                // The name of the function or method.
    std::vector<Argument> arguments; // The list of arguments of the function.
    std::string return_type;         // The return type of the function.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables the moving of a Prototype object.
     * The operation is defaulted (C++ 20).
     */
    Prototype(Prototype&&) = default;

    /**
     * @brief Constructor that initializes the Prototype with a name, arguments,
     * and a return type.
     *
     * @param name The name of the function or method.
     * @param arguments The list of arguments of the function, each represented
     * as an 'Argument'.
     * @param return_type The return type of the function as a string.
     */
    explicit Prototype(const std::string& name, std::vector<Argument> arguments,
                       const std::string& return_type);

    /**
     * @brief Gets the name of the function or method.
     *
     * @return A std::string representing the name of the function or method.
     */
    std::string get_name() const;

    /**
     * @brief Gets a reference to the list of arguments.
     *
     * @return A reference to a std::vector containing the Argument objects
     * representing the function's arguments.
     */
    std::vector<Argument>& get_arguments();

    /**
     * @brief Gets the return type of the function or method.
     *
     * @return A std::string representing the return type of the function or
     * method.
     */
    std::string get_return_type() const;

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a Prototype object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified Prototype object.
     */
    Prototype& operator=(Prototype&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this Prototype object with another for equality. The operation
     * is defaulted (C++ 20).
     *
     * @param rhs The right-hand side Prototype object to compare with.
     *
     * @return true if both Prototype objects are equal, false otherwise.
     */
    bool operator==(const Prototype& rhs) const = default;
};

/**
 * @brief Represents a statement used for variable declaration and
 * initialization.
 *
 * This class encapsulates a variable declaration statement, which includes the variable's
 * name and type.
 */
class CookedUpStatement {
  private:
    std::string var_name; // The name of the variable being declared.
    std::string var_type; // The type of the variable being declared.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a CookedUpStatement object. The operation is defaulted (C++ 20).
     */
    CookedUpStatement(CookedUpStatement&&) = default;

    /**
     * @brief Constructor that initializes the CookedUpStatement with a variable
     * name and type.
     *
     * @param var_name The name of the variable being declared.
     * @param var_type The type of the variable being declared.
     */
    explicit CookedUpStatement(const std::string& var_name, const std::string& var_type);

    /**
     * @brief Gets the name of the variable.
     *
     * @return A std::string representing the name of the variable.
     */
    std::string get_var_name() const;

    /**
     * @brief Gets the type of the variable.
     *
     * @return A std::string representing the type of the variable.
     */
    std::string get_var_type() const;

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a CookedUpStatement object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified CookedUpStatement object.
     */
    CookedUpStatement& operator=(CookedUpStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this CookedUpStatement object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side CookedUpStatement object to compare with.
     *
     * @return true if both CookedUpStatement objects are equal, false
     * otherwise.
     */
    bool operator==(const CookedUpStatement& rhs) const = default;
};

/**
 * @brief Represents an assignment statement in an abstract syntax tree.
 *
 * This class encapsulates an assignment statement, which involves assigning
 * a value or expression to a variable. It consists of the variable's
 * name and the expression being assigned to it.
 */
class AssignmentStatement {
  private:
    std::string var_name;             // The name of the variable being assigned to.
    Expression assignment_expression; // The expression being assigned to the variable.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of an AssignmentStatement object. The operation is
     * defaulted (C++ 20).
     */
    AssignmentStatement(AssignmentStatement&&) = default;

    /**
     * @brief Constructor that initializes the AssignmentStatement with a
     * variable name and an expression.
     *
     * @param var_name The name of the variable being assigned to.
     *
     * @param assignment_expression The expression being assigned to the
     * variable.
     */
    explicit AssignmentStatement(const std::string& var_name,
                                 Expression assignment_expression);

    /**
     * @brief Gets the name of the variable.
     *
     * @return A std::string representing the name of the variable being
     * assigned to.
     */
    std::string get_var_name() const;

    /**
     * @brief Gets a reference to the assignment expression.
     *
     * @return A reference to the Expression object representing the assignment
     * expression.
     */
    Expression& get_assignment_expression();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of an AssignmentStatement object. The operation
     * is defaulted (C++ 20).
     *
     * @return A reference to the modified AssignmentStatement object.
     */
    AssignmentStatement& operator=(AssignmentStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this AssignmentStatement object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side AssignmentStatement object to compare with.
     *
     * @return true if both AssignmentStatement objects are equal, false otherwise.
     */
    bool operator==(const AssignmentStatement& rhs) const = default;
};

/**
 * @brief Represents a combined declaration and assignment statement in an abstract syntax
 * tree.
 *
 * This class encapsulates a 'cookUp-and-assign' statement, which includes the
 * declaration of a variable with its type, followed by an immediate assignment
 * of an expression to that variable. It consists of the variable's name, its
 * type, and the expression being assigned.
 */
class CookedUpAssignmentStatement {
  private:
    std::string var_name; // The name of the variable being declared and assigned to.
    std::string var_type; // The type of the variable being declared.
    Expression assignment_expression; // The expression being assigned to the variable.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a CookedUpAssignmentStatement object. The operation is
     * defaulted (C++ 20).
     */
    CookedUpAssignmentStatement(CookedUpAssignmentStatement&&) = default;

    /**
     * @brief Constructor that initializes the CookedUpAssignmentStatement with
     * a variable name, type, and assignment expression.
     *
     * @param var_name The name of the variable being declared and assigned to.
     * @param var_type The type of the variable being declared.
     * @param assignment_expression The expression being assigned to the
     * variable.
     */
    explicit CookedUpAssignmentStatement(const std::string& var_name,
                                         const std::string& var_type,
                                         Expression assignment_expression);

    /**
     * @brief Gets the name of the variable.
     *
     * @return A std::string representing the name of the variable.
     */
    std::string get_var_name() const;

    /**
     * @brief Gets the type of the variable.
     *
     * @return A std::string representing the type of the variable.
     */
    std::string get_var_type() const;

    /**
     * @brief Gets a reference to the assignment expression.
     *
     * @return A reference to the Expression object representing the assignment
     * expression.
     */
    Expression& get_assignment_expression();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a CookedUpAssignmentStatement object. The
     * operation is defaulted (C++ 20).
     *
     * @return A reference to the modified CookedUpAssignmentStatement object.
     */
    CookedUpAssignmentStatement& operator=(CookedUpAssignmentStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this CookedUpAssignmentStatement object with another for
     * equality. The operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side CookedUpAssignmentStatement object to
     * compare with.
     *
     * @return true if both CookedUpAssignmentStatement objects are equal, false
     * otherwise.
     */
    bool operator==(const CookedUpAssignmentStatement& rhs) const = default;
};

/**
 * @brief Represents a return value statement in an abstract syntax tree.
 *
 * This class encapsulates a return value statement, which is used to exit a function
 * and return a value. The return value is represented by an Expression, referred to here
 * as 'yeet_expr'.
 */
class YeetStatement {
  private:
    Expression yeet_expr; // The expression being returned by the statement.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a YeetStatement object. The operation is defaulted (C++ 20).
     */
    YeetStatement(YeetStatement&&) = default;

    /**
     * @brief Constructor that initializes the YeetStatement with a return
     * expression.
     *
     * @param yeet_expr The expression to be returned by the statement.
     */
    explicit YeetStatement(Expression yeet_expr);

    /**
     * @brief Gets a reference to the return expression.
     *
     * @return A reference to the Expression object representing the return
     * expression.
     */
    Expression& get_yeet_expr();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a YeetStatement object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified YeetStatement object.
     */
    YeetStatement& operator=(YeetStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this YeetStatement object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side YeetStatement object to compare with.
     *
     * @return true if both YeetStatement objects are equal, false otherwise.
     */
    bool operator==(const YeetStatement& rhs) const = default;
};

/**
 * @brief Represents a breaking loop statement in a loop context in an abstract syntax
 * tree.
 *
 * This class encapsulates a breaking loop statement, which is typically used to exit a
 * loop prematurely.
 */
class GhostStatement {
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a GhostStatement object. The operation is defaulted (C++ 20).
     */
    GhostStatement(GhostStatement&&) = default;

    /**
     * @brief Default constructor.
     *
     * Constructs a GhostStatement object, representing a break statement.
     */
    explicit GhostStatement() = default;

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a GhostStatement object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified GhostStatement object.
     */
    GhostStatement& operator=(GhostStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this GhostStatement object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side GhostStatement object to compare with.
     *
     * @return true if both GhostStatement objects are equal, false otherwise.
     */
    bool operator==(const GhostStatement& rhs) const = default;
};

/**
 * @brief Represents a 'continue' statement in a loop context in an abstract syntax tree.
 *
 * This class encapsulates a 'continue' statement, used in looping to skip the remaining
 * portion of the body and continue with the next iteration.
 */
class RizzStatement {
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a RizzStatement object. The operation is defaulted (C++ 20).
     */
    RizzStatement(RizzStatement&&) = default;

    /**
     * @brief Default constructor.
     *
     * Constructs a RizzStatement object, representing a continue statement.
     */
    explicit RizzStatement() = default;

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a RizzStatement object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified RizzStatement object.
     */
    RizzStatement& operator=(RizzStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this RizzStatement object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side RizzStatement object to compare with.
     *
     * @return true if both RizzStatement objects are equal, false otherwise.
     */
    bool operator==(const RizzStatement& rhs) const = default;
};

/**
 * @brief Type alias representing a variant of different statement types in an
 * abstract syntax tree.
 *
 * This type alias defines 'Statement' as a std::variant that can hold different
 * types of statement objects. Each statement type represents a different
 * kind of statement in a programming language.
 *
 * Types:
 * - std::unique_ptr<CookedUpStatement>: Represents a variable declaration statement.
 * - std::unique_ptr<AssignmentStatement>: Represents a variable assignment statement.
 * - std::unique_ptr<CookedUpAssignmentStatement>: Represents a combined variable
 * declaration and assignment statement.
 * - std::unique_ptr<FrOngJustLikeThatStatement>: Represents a conditional branching
 * statement.
 * - std::unique_ptr<HoldUpStatement>: Represents a conditional loop statement.
 * - std::unique_ptr<GhostStatement>: Represents a breaking statement in a loop.
 * - std::unique_ptr<RizzStatement>: Represents a skipping iteration statement in a loop.
 * - std::unique_ptr<YeetStatement>: Represents a return value to caller statement.
 * - std::unique_ptr<CompoundStatement>: Represents a compound statement.
 */
using Statement =
    std::variant<std::unique_ptr<CookedUpStatement>, std::unique_ptr<AssignmentStatement>,
                 std::unique_ptr<CookedUpAssignmentStatement>,
                 std::unique_ptr<FrOngJustLikeThatStatement>,
                 std::unique_ptr<HoldUpStatement>, std::unique_ptr<GhostStatement>,
                 std::unique_ptr<RizzStatement>, std::unique_ptr<YeetStatement>,
                 std::unique_ptr<CompoundStatement>>;

/**
 * @brief Represents conditional branching statement in an abstract syntax tree.
 *
 * This class encapsulates an if-elseif-else statement, which is a fundamental
 * control flow construct in programming. It consists of a condition
 * (Expression), a 'then' statement (Statement) to execute if the condition is
 * true, and an 'else' statement (Statement) to execute if the condition is
 * false.
 */
class FrOngJustLikeThatStatement {
  private:
    Expression condition;     // The condition expression to evaluate.
    Statement then_statement; // The statement to execute if the condition is true.
    Statement else_statement; // The statement to execute if the condition is false.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a FrOngJustLikeThatStatement object. The operation is
     * defaulted (C++ 20).
     */
    FrOngJustLikeThatStatement(FrOngJustLikeThatStatement&&) = default;

    /**
     * @brief Constructor that initializes the FrOngJustLikeThatStatement with a
     * condition, then statement, and else statement.
     *
     * @param condition The condition Expression to evaluate.
     * @param then_statement The Statement to execute if the condition is true.
     * @param else_statement The Statement to execute if the condition is false.
     */
    explicit FrOngJustLikeThatStatement(Expression condition, Statement then_statement,
                                        Statement else_statement);

    /**
     * @brief Gets a reference to the condition expression.
     *
     * @return A reference to the Expression object representing the condition.
     */
    Expression& get_condition();

    /**
     * @brief Gets a reference to the 'then' statement.
     *
     * @return A reference to the Statement object to execute if the condition
     * is true.
     */
    Statement& get_then_statement();

    /**
     * @brief Gets a reference to the 'else' statement.
     *
     * @return A reference to the Statement object to execute if the condition
     * is false.
     */
    Statement& get_else_statement();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a FrOngJustLikeThatStatement object. The
     * operation is defaulted (C++ 20).
     *
     * @return A reference to the modified FrOngJustLikeThatStatement object.
     */
    FrOngJustLikeThatStatement& operator=(FrOngJustLikeThatStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this FrOngJustLikeThatStatement object with another for
     * equality. The operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side FrOngJustLikeThatStatement object to
     * compare with.
     *
     * @return true if both FrOngJustLikeThatStatement objects are equal, false
     * otherwise.
     */
    bool operator==(const FrOngJustLikeThatStatement& rhs) const = default;
};

/**
 * @brief Represents a loop statement in an abstract syntax tree.
 *
 * This class encapsulates a holdUp loop. It consists of a condition (Expression) and a
 * body (Statement) that is executed repeatedly as long as the condition evaluates to
 * true.
 */
class HoldUpStatement {
  private:
    Expression condition; // The condition expression to evaluate for the loop
                          // continuation.
    Statement body;       // The statement or block of statements to execute in the
                          // loop body.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a HoldUpStatement object. The operation is defaulted (C++ 20).
     */
    HoldUpStatement(HoldUpStatement&&) = default;

    /**
     * @brief Constructor that initializes the HoldUpStatement with a condition
     * and a body.
     *
     * @param condition The condition Expression to evaluate for loop continuation.
     * @param body The Statement that represents the loop body to be executed.
     */
    explicit HoldUpStatement(Expression condition, Statement body);

    /**
     * @brief Gets a reference to the condition expression.
     *
     * @return A reference to the Expression object representing the loop
     * condition.
     */
    Expression& get_condition();

    /**
     * @brief Gets a reference to the loop body.
     *
     * @return A reference to the Statement object representing the loop body.
     */
    Statement& get_body();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a HoldUpStatement object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified HoldUpStatement object.
     */
    HoldUpStatement& operator=(HoldUpStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this HoldUpStatement object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side HoldUpStatement object to compare with.
     *
     * @return true if both HoldUpStatement objects are equal, false otherwise.
     */
    bool operator==(const HoldUpStatement& rhs) const = default;
};

/**
 * @brief Represents a compound statement in an abstract syntax tree.
 *
 * This class encapsulates a compound statement, essentially a group of
 * statements. It is represented as a vector of 'Statement' objects, which are
 * encompassed by curly braces in the source code.
 */
class CompoundStatement {
  private:
    std::vector<Statement> statements; // A vector of statements that form the
                                       // compound statement.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a CompoundStatement object. The operation is defaulted (C++ 20).
     */
    CompoundStatement(CompoundStatement&&) = default;

    /**
     * @brief Constructor that initializes the CompoundStatement with a sequence
     * of statements.
     *
     * @param statements A vector of Statement objects that constitute the
     * compound statement.
     */
    explicit CompoundStatement(std::vector<Statement> statements);

    /**
     * @brief Gets a reference to the vector of statements.
     *
     * @return A reference to a vector of Statement objects representing the
     * individual statements in the compound statement.
     */
    std::vector<Statement>& get_statements();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a CompoundStatement object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified CompoundStatement object.
     */
    CompoundStatement& operator=(CompoundStatement&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this CompoundStatement object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side CompoundStatement object to compare with.
     *
     * @return true if both CompoundStatement objects are equal, false
     * otherwise.
     */
    bool operator==(const CompoundStatement& rhs) const = default;
};

/**
 * @brief Represents a function declaration in an abstract syntax tree.
 *
 * This class encapsulates a function declaration, including its prototype
 * (signature) and an optional function body. The prototype is represented by a
 * 'Prototype' object, which includes the function's name, its arguments, and
 * its return type. The function body, if present, is represented by the alias
 * 'Statement'. The function body is optional, so this class represents both function
 * prototypes (declarations without a body) and function definitions (with a body).
 */
class PluhDeclaration {
  private:
    Prototype p;                   // The prototype or signature of the function.
    std::optional<Statement> body; // Optional body of the function.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a PluhDeclaration object. The operation is defaulted (C++ 20).
     */
    PluhDeclaration(PluhDeclaration&&) = default;

    /**
     * @brief Constructor that initializes the PluhDeclaration with a prototype
     * and an optional function body.
     *
     * @param p The Prototype object representing the function's signature.
     * @param body The optional Statement representing the function's body. If
     * not provided, represents a function prototype.
     */
    explicit PluhDeclaration(Prototype p, std::optional<Statement> body = std::nullopt);

    /**
     * @brief Gets a reference to the function prototype.
     *
     * @return A reference to the Prototype object representing the function's
     * signature.
     */
    Prototype& get_prototype();

    /**
     * @brief Gets a reference to the optional function body.
     *
     * @return A reference to an optional Statement object representing the
     * function's body, if it exists.
     */
    std::optional<Statement>& get_body();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a PluhDeclaration object. The operation is
     * defaulted (C++ 20).
     *
     * @return A reference to the modified PluhDeclaration object.
     */
    PluhDeclaration& operator=(PluhDeclaration&&) = default;

    /**
     * @brief Default equality comparison operator.
     *
     * Compares this PluhDeclaration object with another for equality. The
     * operation is defaulted (C++ 20).
     *
     * @param rhs The right-hand side PluhDeclaration object to compare with.
     *
     * @return true if both PluhDeclaration objects are equal, false otherwise.
     */
    bool operator==(const PluhDeclaration& rhs) const = default;
};

/**
 * @brief Represents the root of an Abstract Syntax Tree (AST) for a program.
 *
 * This class represents the name of the program and all the Pluh's. The
 * declarations are stored as a vector of declarations that may exist in this file.
 */
class TeaSpill {
  private:
    std::string name; // The name of the program or module represented by the AST.
    std::vector<std::variant<PluhDeclaration>>
        declarations; // A collection of declarations in the program.
  public:
    /**
     * @brief Default move constructor.
     *
     * Enables moving of a TeaSpill object. The operation is defaulted (C++ 20).
     */
    TeaSpill(TeaSpill&&) = default;

    /**
     * @brief Constructor that initializes the TeaSpill with a name and a
     * collection of declarations.
     *
     * @param name The name of the program or module.
     * @param declarations A vector of variants, each holding a PluhDeclaration,
     * representing the program's declarations.
     */
    explicit TeaSpill(const std::string& name,
                      std::vector<std::variant<PluhDeclaration>> declarations);

    /**
     * @brief Gets the name of the program or module.
     *
     * @return A std::string representing the name of the program or module.
     */
    std::string get_name() const;

    /**
     * @brief Gets a reference to the collection of declarations.
     *
     * @return A reference to a vector of variants, each possibly holding a
     * PluhDeclaration, representing the program's declarations.
     */
    std::vector<std::variant<PluhDeclaration>>& get_declarations();

    /**
     * @brief Default move assignment operator.
     *
     * Enables move assignment of a TeaSpill object. The operation is defaulted (C++ 20).
     *
     * @return A reference to the modified TeaSpill object.
     */
    TeaSpill& operator=(TeaSpill&&) = default;
};

#endif
