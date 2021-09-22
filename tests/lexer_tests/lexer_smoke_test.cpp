#include "../../include/lexer/Lexer.h"
#include <gtest/gtest.h>

const char* TEMP_TEST_NAME = "__test";

class LexerSmokeTest:
    public ::testing::Test
{
protected:
    void TearDown() override {
        std::filesystem::remove(TEMP_TEST_NAME);
    }

    void fill(const char* content) {
        std::ofstream file{TEMP_TEST_NAME};
        file << content;
    }
};


TEST_F(LexerSmokeTest, TestEmpty) {
    fill("");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {};

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestSpace) {
    fill("   ");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {};

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestClass) {
    fill("cLaSs");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "CLASS" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestELSE) {
    fill("elSe");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "ELSE" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestFALSE) {
    fill("faLse");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::BoolConst, "false" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestFI) {
    fill("fI");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "FI" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestIF) {
    fill("iF");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IF" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestIN) {
    fill("iN");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IN" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestINHERITS) {
    fill("InHeriTs");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "INHERITS" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestISVOID) {
    fill("iSvOid");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "ISVOID" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestLET) {
    fill("LeT");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "LET" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestLOOP) {
    fill("LoOP");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "LOOP" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestPOOL) {
    fill("PoOL");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "POOL" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestTHEN) {
    fill("ThEn");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "THEN" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestWHILE) {
    fill("WHIlE");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "WHILE" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestCASE) {
    fill("casE");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "CASE" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestESAC) {
    fill("EsAC");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "ESAC" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestNEW) {
    fill("NEw");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "NEW" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestOF) {
    fill("OF");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "OF" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestNOT) {
    fill("nOt");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "NOT" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestTRUE) {
    fill("trUe");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::BoolConst, "true" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestBadTrue) {
    fill("True");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::TypeIdentifier, "True" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestTwoIF) {
    fill("if if");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Keyword, "IF" },
        { 1, cool::lexer::TokenType::Keyword, "IF" },
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestObjectID) {
    fill("if123");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::ObjectIdentifier, "if123" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestTypeID) {
    fill("Stack");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::TypeIdentifier, "Stack" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestStringSimple) {
    fill("\"42\"");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::String, "\"42\"" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestStringTricky) {
    fill("\"1\\\"2\\\"3\"");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::String, "\"1\\\"2\\\"3\"" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestStringNested) {
    fill("a=\"42\";");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::ObjectIdentifier, "a" },
        { 1, cool::lexer::TokenType::SpecialNotation, "=" },
        { 1, cool::lexer::TokenType::String, "\"42\"" },
        { 1, cool::lexer::TokenType::SpecialNotation, ";" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestStringMultiline) {
    fill("\"A \n B\"");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::String, "\"A \n B\"" }
    };

    EXPECT_EQ(result, expected);
}


TEST_F(LexerSmokeTest, TestComplexIF) {
    fill("if(true){123}");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

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


TEST_F(LexerSmokeTest, TestComplexSpacedIF) {
    fill("if ( true ) { 123 } ");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

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


TEST_F(LexerSmokeTest, TestError) {
    fill("[]");

    cool::lexer::Lexer lexer(TEMP_TEST_NAME);

    auto result = lexer.get_result();

    std::vector<cool::lexer::Token> expected = {
        { 1, cool::lexer::TokenType::Error, "[]" }
    };

    EXPECT_EQ(result, expected);
}

