#include "lexer.hpp"
#include <gtest/gtest.h>

bool debug_mode = false;
DebugStream debug;

// Test Return
TEST(LexerTest, Periodt_Statement) {
    Lexer lexer("periodt 1337");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::RETURN, "periodt"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1337"));
}

// Test Def
TEST(LexerTest, Pluh_Statement) {
    Lexer lexer("pluh");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::DEF, "pluh"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Plug
TEST(LexerTest, Plug_Statement) {
    Lexer lexer("plug func(x : int) : int");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::EXTERN, "plug"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "func"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "x"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "int"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "int"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test cookUp Int
TEST(LexerTest, cookUp_Int_Statement) {
    Lexer lexer("cookUp hello: int");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::LET, "cookUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "int"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test cookUp Char
TEST(LexerTest, cookUp_Char_Statement) {
    Lexer lexer("cookUp hello: char");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::LET, "cookUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "char"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test cookUp String
TEST(LexerTest, cookUp_String_Statement) {
    Lexer lexer("cookUp hello: string");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::LET, "cookUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "string"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test cookUp and Assignment Int
TEST(LexerTest, cookUp_Assignment_Int_Statement) {
    Lexer lexer("cookUp hello: int = 1337");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::LET, "cookUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "int"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1337"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test cookUp and Assignment String
TEST(LexerTest, cookUp_Assignment_String_Statement) {
    Lexer lexer("cookUp hello: string = \"hello\"");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::LET, "cookUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "string"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::STRING, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test If
TEST(LexerTest, Fr_Statement) {
    Lexer lexer("fr? bonjour < 0");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IF, "fr?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "bonjour"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "<"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "0"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Literals (Int, Float, Bool, Char, String)
TEST(LexerTest, Literals) {
    Lexer lexer("1 12 -123 1.234567 facts cap 'a' \"hello\" 31.87");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "12"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "-"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "123"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::FLOAT, "1.234567"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BOOL, "facts"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BOOL, "cap"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::CHAR, "a"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::STRING, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::FLOAT, "31.87"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Pluh Defintion
TEST(LexerTest, Pluh_Definitions) {
    Lexer lexer("pluh func() 0");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::DEF, "pluh"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "func"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "0"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Function Declaration/Prototype
TEST(LexerTest, Prototypes) {
    Lexer lexer("greeting_func(hola: int): npc");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "greeting_func"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hola"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "int"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "npc"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Operators
TEST(LexerTest, Operators) {
    Lexer lexer("((1 + 2 * 3 >= 4) != facts) == cap");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "+"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "2"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "*"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "3"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, ">="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "4"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "!="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BOOL, "facts"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "=="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BOOL, "cap"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test holdUp, Ghost, Rizz, Ong?, JustLikeThat?
TEST(LexerTest, holdUp_Ghost_Rizz_Ong_JustLikeThat) {
    Lexer lexer(R"(holdUp hello > 1 {
    fr? hola % 2 == 3 {
        ghost
    } ong? bonjour % 4 == 5 {
        rizz
    } justLikeThat? {
        ghost
    }
    })");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::WHILE, "holdUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, ">"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IF, "fr?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hola"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "%"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "2"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "==")); 
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "3"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BREAK, "ghost"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::ELSEIF, "ong?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "bonjour"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "%"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "4"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "=="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "5"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::CONTINUE, "rizz"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::ELSE, "justLikeThat?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BREAK, "ghost"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Comments
TEST(LexerTest, Comments) {
    Lexer lexer(R"(holdUp hello > 1 {
    fr? hola % 2 == 3 {
        Cancelled this line should be ignored
        ghost
    } ong? bonjour % 4 == 5 {
        Blocked this line should be ignored
        also this line should be ignored
        maybe this line too
        Unblocked
        rizz
    } justLikeThat? {
        ghost
    }
    })");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::WHILE, "holdUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, ">"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IF, "fr?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hola"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "%"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "2"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "==")); 
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "3"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BREAK, "ghost"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::ELSEIF, "ong?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "bonjour"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "%"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "4"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "=="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "5"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::CONTINUE, "rizz"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::ELSE, "justLikeThat?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BREAK, "ghost"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Ratio, Ghost, Rizz
TEST(LexerTest, Ratioed_Ghost_Rizz) {
    Lexer lexer(R"(ratioed cookUp i : int = 0 | i < 10 | i = i + 1 {
        fr? i == 5 {
            ghost
        } ong? i == 7 {
            rizz
        } justLikeThat? {
            yap(i)
        }
    })");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::FOR, "ratioed"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::LET, "cookUp"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "i"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "int"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "0"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "|"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "i"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "<"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "10"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "|"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "i"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "i"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "+"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IF, "fr?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "i"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "=="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "5"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::BREAK, "ghost"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::ELSEIF, "ong?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "i"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "=="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "7"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::CONTINUE, "rizz"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::ELSE, "justLikeThat?"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "{"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "yap"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "i"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "}"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Arrays
TEST(LexerTest, Gang) {
    Lexer lexer("gang hello = (1, 2, 3, 4) : int");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::ARRAY, "gang"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "="));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ","));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "2"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ","));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "3"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ","));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "4"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ":"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "int"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));

}

// Test Exitcode
TEST(LexerTest, Yeet) {
    Lexer lexer("yeet 0");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::EXITCODE, "yeet"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "0"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Print/Yap Int
TEST(LexerTest, Yap) {
    Lexer lexer("yap(0)");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "yap"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "0"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Print/Yap String
TEST(LexerTest, Yap_String) {
    Lexer lexer("yap(\"hello\")");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "yap"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::STRING, "hello"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}

// Test Print/Yap Expression
TEST(LexerTest, Yap_Expression) {
    Lexer lexer("yap(1 + 2)");
    EXPECT_EQ(lexer.get_token(), Token(TokenType::IDENTIFIER, "yap"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, "("));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "1"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::OPERATOR, "+"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::INT, "2"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::COMPLEX, ")"));
    EXPECT_EQ(lexer.get_token(), Token(TokenType::END_OF_FILE, ""));
}