#include "../../include/lexer/Lexer.h"
#include <gtest/gtest.h>


TEST(LexerSmokeTest, TestEmpty) {
    std::stringstream input("");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {};

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestSpace) {
    std::stringstream input("   ");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {};

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestClass) {
    std::stringstream input("cLaSs");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "CLASS" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestELSE) {
    std::stringstream input("elSe");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "ELSE" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestFALSE) {
    std::stringstream input("faLse");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::BoolConst, "false" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestFI) {
    std::stringstream input("fI");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "FI" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestIF) {
    std::stringstream input("iF");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IF" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestIN) {
    std::stringstream input("iN");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IN" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestINHERITS) {
    std::stringstream input("InHeriTs");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "INHERITS" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestISVOID) {
    std::stringstream input("iSvOid");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "ISVOID" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestLET) {
    std::stringstream input("LeT");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "LET" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestLOOP) {
    std::stringstream input("LoOP");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "LOOP" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestPOOL) {
    std::stringstream input("PoOL");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "POOL" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestTHEN) {
    std::stringstream input("ThEn");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "THEN" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestWHILE) {
    std::stringstream input("WHIlE");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "WHILE" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestCASE) {
    std::stringstream input("casE");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "CASE" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestESAC) {
    std::stringstream input("EsAC");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "ESAC" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestNEW) {
    std::stringstream input("NEw");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "NEW" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestOF) {
    std::stringstream input("OF");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "OF" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestNOT) {
    std::stringstream input("nOt");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "NOT" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestTRUE) {
    std::stringstream input("trUe");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::BoolConst, "true" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestBadTrue) {
    std::stringstream input("True");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::TypeIdentifier, "True" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestTwoIF) {
    std::stringstream input("if if");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IF" },
        { 1, cool::lexer::TokenType::Keyword, "IF" },
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestObjectID) {
    std::stringstream input("if123");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::ObjectIdentifier, "if123" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestTypeID) {
    std::stringstream input("Stack");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::TypeIdentifier, "Stack" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestStringSimple) {
    std::stringstream input("\"42\"");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::String, "\"42\"" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestStringTricky) {
    std::stringstream input("\"1\\\"2\\\"3\"");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::String, "\"1\\\"2\\\"3\"" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestStringNested) {
    std::stringstream input("a=\"42\";");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::ObjectIdentifier, "a" },
        { 1, cool::lexer::TokenType::SpecialNotation, "=" },
        { 1, cool::lexer::TokenType::String, "\"42\"" },
        { 1, cool::lexer::TokenType::SpecialNotation, ";" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestStringMultiline) {
    std::stringstream input("\"A \\\n B\"");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::String, "\"A \\\n B\"" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestCommentOneLine) {
    std::stringstream input("A -- this is comment\nB");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::TypeIdentifier, "A" },
        { 2, cool::lexer::TokenType::TypeIdentifier, "B" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestCommentMultiline) {
    std::stringstream input("A (* this is comment\nB*)C");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::TypeIdentifier, "A" },
        { 2, cool::lexer::TokenType::TypeIdentifier, "C" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestCommentMultilineNested) {
    std::stringstream input("A (* this (* is com\n*)ment\nB*)C");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::TypeIdentifier, "A" },
        { 2, cool::lexer::TokenType::TypeIdentifier, "C" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestComplexIF) {
    std::stringstream input("if(true){123}");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IF" },
        { 1, cool::lexer::TokenType::SpecialNotation, "(" },
        { 1, cool::lexer::TokenType::BoolConst, "true" },
        { 1, cool::lexer::TokenType::SpecialNotation, ")" },
        { 1, cool::lexer::TokenType::SpecialNotation, "{" },
        { 1, cool::lexer::TokenType::Integer, "123" },
        { 1, cool::lexer::TokenType::SpecialNotation, "}" },
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestComplexSpacedIF) {
    std::stringstream input("if ( true ) { 123 } ");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IF" },
        { 1, cool::lexer::TokenType::SpecialNotation, "(" },
        { 1, cool::lexer::TokenType::BoolConst, "true" },
        { 1, cool::lexer::TokenType::SpecialNotation, ")" },
        { 1, cool::lexer::TokenType::SpecialNotation, "{" },
        { 1, cool::lexer::TokenType::Integer, "123" },
        { 1, cool::lexer::TokenType::SpecialNotation, "}" },
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestError) {
    std::stringstream input("[]");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Error, "[]" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestErrorEOFComment) {
    std::stringstream input("class (* \n if");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "CLASS" },
        { 2, cool::lexer::TokenType::Error, "EOF in comment" }
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestErrorUnterminatedString) {
    std::stringstream input("class \" \n if");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "CLASS" },
        { 1, cool::lexer::TokenType::Error, "Unterminated string constant" },
        { 2, cool::lexer::TokenType::Keyword, "IF" },
    };

    EXPECT_EQ(result, expected);
}


TEST(LexerSmokeTest, TestErrorEOFString) {
    std::stringstream input("class \" \\\n if\\");

    cool::lexer::Lexer lexer(input);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "CLASS" },
        { 2, cool::lexer::TokenType::Error, "EOF in string constant" },
    };

    EXPECT_EQ(result, expected);
}

