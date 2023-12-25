#include "ast.hpp"

template <typename T>
Literal<T>::Literal(T value) : value(value) {}

template <typename T>
T Literal<T>::get_value() const {
    return value;
}

VariableExpression::VariableExpression(const std::string& name) : name(name) {
    debug << "[DEBUG] Variable Expression Initialized: " << name << std::endl;
}

std::string VariableExpression::get_name() const {
    return name;
}

UnaryExpression::UnaryExpression(const std::string& op, Expression rhs)
    : op(op), rhs(std::move(rhs)) {
    debug << "[DEBUG] Unary Expression Initialized: " << op << std::endl;
}

std::string UnaryExpression::get_op() const {
    return op;
}

Expression& UnaryExpression::get_rhs() {
    return rhs;
}

BinaryExpression::BinaryExpression(const std::string& op, Expression lhs, Expression rhs)
    : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {
    debug << "[DEBUG] Binary Expression Initialized: " << op << std::endl;
}

std::string BinaryExpression::get_op() const {
    return op;
}

Expression& BinaryExpression::get_lhs() {
    return lhs;
}

Expression& BinaryExpression::get_rhs() {
    return rhs;
}

CallExpression::CallExpression(const std::string& callee,
                               std::vector<Expression> arguments)
    : callee(callee), arguments(std::move(arguments)) {
    debug << "[DEBUG] Call Expression Initialized: " << callee << std::endl;
}

std::string CallExpression::get_callee() const {
    return callee;
}

std::vector<Expression>& CallExpression::get_arguments() {
    return arguments;
}

Prototype::Prototype(const std::string& name,
                     std::vector<std::pair<std::string, std::string>> arguments,
                     const std::string& return_type)
    : name(name), arguments(arguments), return_type(return_type) {
    debug << "[DEBUG] Prototype Initialized: " << name << std::endl;
}

std::string Prototype::get_name() const {
    return name;
}

std::vector<Argument>& Prototype::get_arguments() {
    return arguments;
}

std::string Prototype::get_return_type() const {
    return return_type;
}

CookedUpStatement::CookedUpStatement(const std::string& var_name,
                                     const std::string& var_type)
    : var_name(var_name), var_type(var_type) {
    debug << "[DEBUG] cookUp Statement Initialized: " << var_name << std::endl;
}

std::string CookedUpStatement::get_var_name() const {
    return var_name;
}

std::string CookedUpStatement::get_var_type() const {
    return var_type;
}

AssignmentStatement::AssignmentStatement(const std::string& var_name,
                                         Expression assignment_expression)
    : var_name(var_name), assignment_expression(std::move(assignment_expression)) {
    debug << "[DEBUG] Assignment Statement Initialized: " << var_name << std::endl;
}

std::string AssignmentStatement::get_var_name() const {
    return var_name;
}

Expression& AssignmentStatement::get_assignment_expression() {
    return assignment_expression;
}

CookedUpAssignmentStatement::CookedUpAssignmentStatement(const std::string& var_name,
                                                         const std::string& var_type,
                                                         Expression assignment_expression)
    : var_name(var_name),
      var_type(var_type),
      assignment_expression(std::move(assignment_expression)) {
    debug << "[DEBUG] cookUp AssignmentStatement Initialized: " << var_name << std::endl;
}

std::string CookedUpAssignmentStatement::get_var_name() const {
    return var_name;
}

std::string CookedUpAssignmentStatement::get_var_type() const {
    return var_type;
}

Expression& CookedUpAssignmentStatement::get_assignment_expression() {
    return assignment_expression;
}

YeetStatement::YeetStatement(Expression yeet_expr) : yeet_expr(std::move(yeet_expr)) {
    debug << "[DEBUG] Yeet Statement Initialized" << std::endl;
}

Expression& YeetStatement::get_yeet_expr() {
    return yeet_expr;
}

FrOngJustLikeThatStatement::FrOngJustLikeThatStatement(Expression condition,
                                                       Statement then_statement,
                                                       Statement else_statement)
    : condition(std::move(condition)),
      then_statement(std::move(then_statement)),
      else_statement(std::move(else_statement)) {
    debug << "[DEBUG] Fr?Ong?justLikeThat? Statement Initialized" << std::endl;
}

Expression& FrOngJustLikeThatStatement::get_condition() {
    return condition;
}

Statement& FrOngJustLikeThatStatement::get_then_statement() {
    return then_statement;
}

Statement& FrOngJustLikeThatStatement::get_else_statement() {
    return else_statement;
}

HoldUpStatement::HoldUpStatement(Expression condition, Statement body)
    : condition(std::move(condition)), body(std::move(body)) {
    debug << "[DEBUG] holdUp Statement Initialized" << std::endl;
}

Expression& HoldUpStatement::get_condition() {
    return condition;
}

Statement& HoldUpStatement::get_body() {
    return body;
}

CompoundStatement::CompoundStatement(std::vector<Statement> statements)
    : statements(std::move(statements)) {
    debug << "[DEBUG] Compound Statement Initialized" << std::endl;
}

std::vector<Statement>& CompoundStatement::get_statements() {
    return statements;
}

PluhDeclaration::PluhDeclaration(Prototype p, std::optional<Statement> body)
    : p(std::move(p)), body(std::move(body)) {
    debug << "[DEBUG] Pluh Declaration Initialized: " << p.get_name() << std::endl;
}

Prototype& PluhDeclaration::get_prototype() {
    return p;
}

std::optional<Statement>& PluhDeclaration::get_body() {
    return body;
}

TeaSpill::TeaSpill(const std::string& name,
                   std::vector<std::variant<PluhDeclaration>> declarations)
    : name(name), declarations(std::move(declarations)) {
    debug << "[DEBUG] spillingTeaAbout: " << name << std::endl;
}

std::string TeaSpill::get_name() const {
    return name;
}

std::vector<std::variant<PluhDeclaration>>& TeaSpill::get_declarations() {
    return declarations;
}

template class Literal<int>;
template class Literal<double>;
template class Literal<bool>;
template class Literal<char>;
template class Literal<std::string>;
