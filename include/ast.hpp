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

template <typename T>
class Literal; // Done
class VariableExpression;
class UnaryExpression;
class BinaryExpression;
class CallExpression;
class CookedUpStatement; // Done
class AssignmentStatement; // Done
class CookedUpAssignmentStatement; // Done
class FrOngJustLikeThatStatement; // Done
class holdUpStatement; // Done
class RatioedStatement; // Done
class CompoundStatement;  // Done
class PeriodtStatement; // Done
class RizzStatement; // Done
class GhostStatement; // Done
class GangStatement; // Done
class PluhDeclaration;
class Prototype;
class TeaSpill;

template <typename T>
inline bool operator==(const std::unique_ptr<T>& p1, const std::unique_ptr<T>& p2) {
    if (p1 != nullptr && p2 != nullptr) {
        return *p1 == *p2;
    }
    return p1 == p2;
}

template <typename T>
class Literal {
  private:
    T value;
  public:
    Literal(Literal<T>&&) = default;
    explicit Literal(T value);
    T get_value() const;
    Literal<T>& operator=(Literal<T>&&) = default;
    bool operator==(const Literal<T>& rhs) const = default;
};

using Expression =
    std::variant<Literal<int>, Literal<double>, Literal<bool>, Literal<char>,
                 Literal<std::string>, std::unique_ptr<VariableExpression>, std::unique_ptr<UnaryExpression>,
                 std::unique_ptr<BinaryExpression>, std::unique_ptr<CallExpression>>;

class VariableExpression {
  private:
    std::string name;
  public:
    VariableExpression(VariableExpression&&) = default;
    explicit VariableExpression(const std::string& name);
    std::string get_name() const;
    VariableExpression& operator=(VariableExpression&&) = default;
    bool operator==(const VariableExpression& rhs) const = default;
};

class UnaryExpression {
  private:
    std::string op;
    Expression rhs;
  public:
    UnaryExpression(UnaryExpression&&) = default;
    explicit UnaryExpression(const std::string& op, Expression rhs);
    std::string get_op() const;
    Expression& get_rhs();
    UnaryExpression& operator=(UnaryExpression&&) = default;
    bool operator==(const UnaryExpression& rhs) const = default;
};

class BinaryExpression {
  private:
    std::string op;
    Expression lhs;
    Expression rhs;
  public:
    BinaryExpression(BinaryExpression&&) = default;
    explicit BinaryExpression(const std::string& op, Expression lhs, Expression rhs);
    std::string get_op() const;
    Expression& get_lhs();
    Expression& get_rhs();
    BinaryExpression& operator=(BinaryExpression&&) = default;
    bool operator==(const BinaryExpression& rhs) const = default;
};

class CallExpression {
  private:
    std::string callee;
    std::vector<Expression> arguments;
  public:
    CallExpression(CallExpression&&) = default;
    explicit CallExpression(const std::string& callee, std::vector<Expression> arguments);
    std::string get_callee() const;
    std::vector<Expression>& get_arguments();
    CallExpression& operator=(CallExpression&&) = default;
    bool operator==(const CallExpression& rhs) const = default;
};

using Argument = std::pair<std::string, std::string>;

class Prototype {
  private:
    std::string name;
    std::vector<Argument> arguments;
    std::string return_type;
  public:
    Prototype(Prototype&&) = default;
    explicit Prototype(const std::string& name, std::vector<Argument> arguments, const std::string& return_type);
    Prototype& operator=(Prototype&&) = default;
    std::string get_name() const;
    std::vector<Argument>& get_arguments();
    std::string get_return_type() const;
    bool operator==(const Prototype& rhs) const = default;
};

class CookedUpStatement {
  private:
    std::string var_name;
    std::string var_type;
  public:
    CookedUpStatement(CookedUpStatement&&) = default;
    explicit CookedUpStatement(const std::string& var_name, const std::string& var_type);
    std::string get_var_name() const;
    std::string get_var_type() const;
    CookedUpStatement& operator=(CookedUpStatement&&) = default;
    bool operator==(const CookedUpStatement& rhs) const = default;
};

class AssignmentStatement {
  private:
    std::string var_name;
    Expression assignment_expression;
  public:
    AssignmentStatement(AssignmentStatement&&) = default;
    explicit AssignmentStatement(const std::string& var_name, Expression assignment_expression);
    std::string get_var_name() const;
    Expression& get_assignment_expression();
    AssignmentStatement& operator=(AssignmentStatement&&) = default;
    bool operator==(const AssignmentStatement& rhs) const = default;
};

class CookedUpAssignmentStatement {
  private:
    std::string var_name;
    std::string var_type;
    Expression assignment_expression;
  public:
    CookedUpAssignmentStatement(CookedUpAssignmentStatement&&) = default;
    explicit CookedUpAssignmentStatement(const std::string& var_name, const std::string& var_type,
                                    Expression assignment_expression);
    std::string get_var_name() const;
    std::string get_var_type() const;
    Expression& get_assignment_expression();
    CookedUpAssignmentStatement& operator=(CookedUpAssignmentStatement&&) = default;
    bool operator==(const CookedUpAssignmentStatement& rhs) const = default;
};

class PeriodtStatement {
  private:
    Expression return_expr;
  public:
    PeriodtStatement(PeriodtStatement&&) = default;
    explicit PeriodtStatement(Expression return_expr);
    Expression& get_return_expr();
    PeriodtStatement& operator=(PeriodtStatement&&) = default;
    bool operator==(const PeriodtStatement& rhs) const = default;
};

class GhostStatement {
  public:
    GhostStatement(GhostStatement&&) = default;
    explicit GhostStatement() = default;
    GhostStatement& operator=(GhostStatement&&) = default;
    bool operator==(const GhostStatement& rhs) const = default;
};

class RizzStatement {
  public:
    RizzStatement(RizzStatement&&) = default;
    explicit RizzStatement() = default;
    RizzStatement& operator=(RizzStatement&&) = default;
    bool operator==(const RizzStatement& rhs) const = default;
};

class GangStatement {
  private:
    std::string name;
    std::string type;
    std::vector<Expression> values;
  public:
    GangStatement(GangStatement&&) = default;
    explicit GangStatement(const std::string& name, const std::string& type, std::vector<Expression> values);
    std::string get_name() const;
    std::string get_type() const;
    std::vector<Expression>& get_values();
    GangStatement& operator=(GangStatement&&) = default;
    bool operator==(const GangStatement& rhs) const = default;
};

using Statement =
    std::variant<std::unique_ptr<CookedUpStatement>, std::unique_ptr<AssignmentStatement>,
                 std::unique_ptr<CookedUpAssignmentStatement>, std::unique_ptr<FrOngJustLikeThatStatement>, std::unique_ptr<holdUpStatement>,
                 std::unique_ptr<GhostStatement>, std::unique_ptr<RizzStatement>, std::unique_ptr<PeriodtStatement>,
                 std::unique_ptr<CompoundStatement>, std::unique_ptr<RatioedStatement>, std::unique_ptr<GangStatement>>;

class FrOngJustLikeThatStatement {
  private:
    Expression condition;
    Statement then_statement;
    Statement else_statement;
  public:
    FrOngJustLikeThatStatement(FrOngJustLikeThatStatement&&) = default;
    explicit FrOngJustLikeThatStatement(Expression condition, Statement then_statement, Statement else_statement);
    Expression& get_condition();
    Statement& get_then_statement();
    Statement& get_else_statement();
    FrOngJustLikeThatStatement& operator=(FrOngJustLikeThatStatement&&) = default;
    bool operator==(const FrOngJustLikeThatStatement& rhs) const = default;
};

class holdUpStatement {
  private:
    Expression condition;
    Statement body;
  public:
    holdUpStatement(holdUpStatement&&) = default;
    explicit holdUpStatement(Expression condition, Statement body);
    Expression& get_condition();
    Statement& get_body();
    holdUpStatement& operator=(holdUpStatement&&) = default;
    bool operator==(const holdUpStatement& rhs) const = default;
};

class RatioedStatement {
  private:
    Statement init;
    Expression condition;
    Statement increment;
    Statement body;
  public:
    RatioedStatement(RatioedStatement&&) = default;
    explicit RatioedStatement(Statement init, Expression condition, Statement increment, Statement body);
    Statement& get_init();
    Expression& get_condition();
    Statement& get_increment();
    Statement& get_body();
    RatioedStatement& operator=(RatioedStatement&&) = default;
    bool operator==(const RatioedStatement& rhs) const = default;
};

class CompoundStatement {
  private:
    std::vector<Statement> statements;
  public:
    CompoundStatement(CompoundStatement&&) = default;
    explicit CompoundStatement(std::vector<Statement> statements);
    std::vector<Statement>& get_statements();
    CompoundStatement& operator=(CompoundStatement&&) = default;
    bool operator==(const CompoundStatement& rhs) const = default;
};

class PluhDeclaration {
  private:
    Prototype p;
    std::optional<Statement> function_body;
  public:
    PluhDeclaration(PluhDeclaration&&) = default;
    explicit PluhDeclaration(Prototype p, Statement function_body);
    explicit PluhDeclaration(Prototype p);
    Prototype& get_prototype();
    std::optional<Statement>& get_function_body();
    PluhDeclaration& operator=(PluhDeclaration&&) = default;
    bool operator==(const PluhDeclaration& rhs) const = default;
};

class TeaSpill {
  private:
    std::string name;
    std::vector<std::variant<PluhDeclaration>> declarations;
  public:
    TeaSpill(TeaSpill&&) = default;
    std::string get_name() const;
    std::vector<std::variant<PluhDeclaration>>& get_declarations();
    explicit TeaSpill(const std::string& name, std::vector<std::variant<PluhDeclaration>> declarations);
    TeaSpill& operator=(TeaSpill&&) = default;
};

#endif