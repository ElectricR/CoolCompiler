#include "AST/AST.h"
#include "semant/Semant.h"

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <string>

TEST(ParserSmokeTest, MainCheck) {
    std::stringstream test_string{"class Main { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::semant::Semant semant(parser);

    ASSERT_EQ(semant.is_successfull(), true);
}
TEST(ParserSmokeTest, NoMainClass) {
    std::stringstream test_string{"class Foo { main(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::semant::Semant semant(parser);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Main class not found.");
}

TEST(ParserSmokeTest, NoMainMethod) {
    std::stringstream test_string{"class Main { foo(): Object { 1 }; };"};

    cool::lexer::Lexer lexer(test_string);

    cool::parser::Parser parser(lexer);

    cool::semant::Semant semant(parser);

    ASSERT_EQ(semant.is_successfull(), false);
    ASSERT_EQ(semant.get_error(), "Method main not found.");
}
