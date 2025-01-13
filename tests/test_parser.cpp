#include "ast.hpp"
#include "parser.hpp"
#include <gtest/gtest.h>

bool debug_mode = false;
DebugStream debug;

template <typename T, size_t N>
std::vector<T> make_vector(std::array<T, N>&& arr) {
    return {std::make_move_iterator(arr.begin()), std::make_move_iterator(arr.end())};
}

template <typename T, typename... U>
auto make_vector(U&&... t) {
    return make_vector(std::to_array<T>({std::forward<U>(t)...}));
}

// Test Int
TEST(TestParser, Int) {
    Parser parser("42");
    auto node = parser.parse_int();
    EXPECT_EQ(node.get_value(), 42);
}

// Test Float
TEST(TestParser, Float) {
    std::string num = "1.234567";
    Parser parser(num);
    auto node = parser.parse_float();
    EXPECT_NEAR(node.get_value(), std::stod(num), 1e-6); // 1e-6 is the error margin
    EXPECT_EQ(node.get_value(), std::stod(num));
}

// Test Bool
TEST(TestParser, Bool) {
    Parser parser("true false"); //// CHANGE THE NAMES OF ALL THE TEST NAMES AND ADD
                                 ///COMMENTS AND SEPERATE TO EXPECT VS ACTUAL
    auto node = parser.parse_bool();
    EXPECT_TRUE(node.get_value());
    node = parser.parse_bool();
    EXPECT_FALSE(node.get_value());
}

// Test String
TEST(TestParser, String) {
    Parser parser(R"("hello world")");
    auto node = parser.parse_string();
    EXPECT_EQ(node.get_value(), "hello world");
}

TEST(TestParser, Unary_Expression) {
    Parser parser("-123");
    Expression actual = parser.parse_unary_expression();
    Expression expected = std::make_unique<UnaryExpression>("-", Literal<int>(123));
    EXPECT_EQ(expected, actual);
}

TEST(TestParser, ProtoType) {
    Parser parser("hello(this: float, is: int, cool: int): float");
    Prototype expected("hello", {{"this", "float"}, {"is", "int"}, {"cool", "int"}},
                       "float");
    Prototype parsed = parser.parse_prototype();
    EXPECT_EQ(expected, parsed);
}

TEST(TestParser, Let) {
    Parser parser("cookUp hello: int");
    Statement expected = std::make_unique<CookedUpStatement>("hello", "int");
    Statement parsed = parser.parse_statement();
    EXPECT_EQ(parsed, expected);
}

// TEST(TestParser, Assignment) {
//     Parser parser("hello = 123 + bonjour");
//     Statement expected = std::make_unique<AssignmentStatement>("hello",
//     std::make_unique<BinaryExpression>("+", Literal<int>(123),
//     VariableExpression("bonjour"))); Statement parsed = parser.parse_statement();
//     EXPECT_EQ(parsed, expected);
// }

TEST(TestParser, Extern) {
    Parser parser("plug hello(bonjour: int, hola: float): npc");
    PluhDeclaration expected(PluhDeclaration(
        Prototype("hello", {{"bonjour", "int"}, {"hola", "float"}}, "npc")));
    PluhDeclaration parsed = parser.parse_plug();
    EXPECT_EQ(expected, parsed);
}

// TEST(TestParser, ParenthesesExpression) {
//     Parser parser("(x + y) / (z - a)");
//     Expression expected = std::make_unique<BinaryExpression>(
//         "/", std::make_unique<BinaryExpression>("+", VariableExpression("x"),
//         VariableExpression("y")), std::make_unique<BinaryExpression>("-",
//         VariableExpression("z"), VariableExpression("a")));
//     Expression parsed = parser.parse_expression();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, CallExpressionT) {
//     Parser parser("hello(123, x / y, bonjour(456))");
//     auto args = make_vector<Expression>(
//         Literal<int>(123),
//         std::make_unique<BinaryExpression>("/", VariableExpression("x"),
//         VariableExpression("y")), std::make_unique<CallExpression>("bonjour",
//         make_vector<Expression>(Literal<int>(456))));
//     Expression expected = std::make_unique<CallExpression>("hello", std::move(args));
//     Expression parsed = parser.parse_expression();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, UnaryExprWithBinary) {
//     Parser parser("(-42 + -x) == !y");
//     Expression expected = std::make_unique<BinaryExpression>(
//         "==",
//         std::make_unique<BinaryExpression>("+", std::make_unique<UnaryExpression>("-",
//         LiteralExpression<int>(42)),
//                                            std::make_unique<UnaryExpression>("-",
//                                            VariableExpression("x"))),
//         std::make_unique<UnaryExpression>("!", VariableExpression("y")));
//     Expression parsed = parser.parse_expression();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, Expr) {
//     Parser parser("a * b + c * d - e * f + g % h + i / j != 123");
//     Expression expected = std::make_unique<BinaryExpression>(
//         "!=",
//         std::make_unique<BinaryExpression>(
//             "+",
//             std::make_unique<BinaryExpression>(
//                 "+",
//                 std::make_unique<BinaryExpression>(
//                     "-",
//                     std::make_unique<BinaryExpression>(
//                         "+", std::make_unique<BinaryExpression>("*",
//                         VariableExpression("a"), VariableExpression("b")),
//                         std::make_unique<BinaryExpression>("*",
//                         VariableExpression("c"), VariableExpression("d"))),
//                     std::make_unique<BinaryExpression>("*", VariableExpression("e"),
//                     VariableExpression("f"))),
//                 std::make_unique<BinaryExpression>("%", VariableExpression("g"),
//                 VariableExpression("h"))),
//             std::make_unique<BinaryExpression>("/", VariableExpression("i"),
//             VariableExpression("j"))),
//         LiteralExpression<int>(123));
//     Expression parsed = parser.parse_expression();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, If) {
//     Parser parser(R"({
//                         if x == y {
//                             let z: int
//                             z = 123
//                         }
//                             let w: float
//                             w = f(x, y)
//                         })");
//     Statement expected(std::make_unique<CompoundStatement>(make_vector<Statement>(
//         std::make_unique<FrOngJustLikeThatStatement>(
//             std::make_unique<BinaryExpression>("==", VariableExpression("x"),
//             VariableExpression("y")),
//             std::make_unique<CompoundStatement>(make_vector<Statement>(
//                 CookedUpStatement("z", "int"), AssignmentStatement("z",
//                 LiteralExpression<int>(123)))),
//             std::make_unique<CompoundStatement>(std::vector<Statement>())),
//         CookedUpStatement("w", "float"),
//         AssignmentStatement("w",
//                             std::make_unique<CallExpression>(
//                                 "f", make_vector<Expression>(VariableExpression("x"),
//                                 VariableExpression("y")))))));
//     Statement parsed = parser.parse_statement();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, IfElseIfElse) {
//     Parser parser(R"(if x == y {
//                         let z: int
//                         z = 42
//                     } else if x > y {
//                         let w: float
//                         w = f(x, y)
//                     } else {
//                         y = 0
//                     })");
//     auto call_args = make_vector<Expression>(VariableExpression("x"),
//     VariableExpression("y")); auto then_stmt =
//         make_vector<Statement>(CookedUpStatement("z", "int"), AssignmentStatement("z",
//         LiteralExpression<int>(42)));
//     auto else_if_stmt =
//         make_vector<Statement>(CookedUpStatement("w", "float"),
//                                AssignmentStatement("w",
//                                std::make_unique<CallExpression>("f",
//                                std::move(call_args))));
//     auto else_else_stmt = make_vector<Statement>(AssignmentStatement("y",
//     LiteralExpression<int>(0))); Statement
//     expected(std::make_unique<FrOngJustLikeThatStatement>(
//         std::make_unique<BinaryExpression>("==", VariableExpression("x"),
//         VariableExpression("y")),
//         std::make_unique<CompoundStatement>(std::move(then_stmt)),
//         std::make_unique<FrOngJustLikeThatStatement>(
//             std::make_unique<BinaryExpression>(">", VariableExpression("x"),
//             VariableExpression("y")),
//             std::make_unique<CompoundStatement>(std::move(else_if_stmt)),
//             std::make_unique<CompoundStatement>(std::move(else_else_stmt)))));
//     Statement parsed = parser.parse_statement();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, While) {
//     Parser parser(R"(while x == y {
//                         if x == 0 {
//                             continue
//                         } else if x > 0 {
//                             break
//                         }
//                         x = x + 1
//                         y = f(x)
//                     })");
//     auto while_body = make_vector<Statement>(
//         std::make_unique<FrOngJustLikeThatStatement>(
//             std::make_unique<BinaryExpression>("==", VariableExpression("x"),
//             LiteralExpression<int>(0)),
//             std::make_unique<CompoundStatement>(make_vector<Statement>(RizzStatement())),
//             std::make_unique<FrOngJustLikeThatStatement>(
//                 std::make_unique<BinaryExpression>(">", VariableExpression("x"),
//                 LiteralExpression<int>(0)),
//                 std::make_unique<CompoundStatement>(make_vector<Statement>(GhostStatement())),
//                 std::make_unique<CompoundStatement>(std::vector<Statement>()))),
//         AssignmentStatement(
//             "x", std::make_unique<BinaryExpression>("+", VariableExpression("x"),
//             LiteralExpression<int>(1))),
//         AssignmentStatement("y",
//                             std::make_unique<CallExpression>("f",
//                             make_vector<Expression>(VariableExpression("x")))));
//     Statement expected = std::make_unique<HoldUpStatement>(
//         std::make_unique<BinaryExpression>("==", VariableExpression("x"),
//         VariableExpression("y")),
//         std::make_unique<CompoundStatement>(std::move(while_body)));
//     Statement parsed = parser.parse_holdup();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, Return) {
//     Parser parser("return hello(123) + 1.20 - x");
//     Statement expected(FlexStatement(std::make_unique<BinaryExpression>(
//         "-",
//         std::make_unique<BinaryExpression>(
//             "+", std::make_unique<CallExpression>("hello",
//             make_vector<Expression>(LiteralExpression<int>(123))),
//             LiteralExpression<double>(1.20)),
//         VariableExpression("x"))));
//     Statement parsed = parser.parse_statement();
//     EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, Compound) {
//     Parser parser(R"({
//                     let x: int
//                     x = 42
//                     let y: float
//                     y = f(x)
//                     })");
//     auto stmts = make_vector<Statement>(
//         CookedUpStatement("x", "int"), AssignmentStatement("x",
//         LiteralExpression<int>(42)), CookedUpStatement("y", "float"),
//         AssignmentStatement("y",
//                             std::make_unique<CallExpression>("f",
//                             make_vector<Expression>(VariableExpression("x")))));
//     Statement expected(std::make_unique<CompoundStatement>(std::move(stmts))), parsed =
//     parser.parse_statement(); EXPECT_EQ(expected, parsed);
// }

// TEST(TestParser, Def) {
//     Parser parser(R"(def f(x: int, y: float): npc {
//                         let z: int
//                         z = g(y)
//                         if x >= z {
//                             h(x)
//                         } else {
//                             h(z)
//                         }
//                     })");
//     auto then_stmts = make_vector<Statement>(AssignmentStatement(
//              "_", std::make_unique<CallExpression>("h",
//              make_vector<Expression>(VariableExpression("x"))))),
//          else_stmts = make_vector<Statement>(AssignmentStatement(
//              "_", std::make_unique<CallExpression>("h",
//              make_vector<Expression>(VariableExpression("z"))))),
//          stmts = make_vector<Statement>(
//              CookedUpStatement("z", "int"),
//              AssignmentStatement(
//                  "z", std::make_unique<CallExpression>("g",
//                  make_vector<Expression>(VariableExpression("y")))),
//              std::make_unique<FrOngJustLikeThatStatement>(
//                  std::make_unique<BinaryExpression>(">=", VariableExpression("x"),
//                  VariableExpression("z")),
//                  std::make_unique<CompoundStatement>(std::move(then_stmts)),
//                  std::make_unique<CompoundStatement>(std::move(else_stmts))));
//     PluhDeclaration expected(PluhDeclaration(Prototype("f", {{"x", "int"}, {"y",
//     "float"}}, "npc"),
//                                                      std::make_unique<CompoundStatement>(std::move(stmts))));
//     PluhDeclaration parsed = parser.parse_pluh_declaration();
//     EXPECT_EQ(expected, parsed);
// }
