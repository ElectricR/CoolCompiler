cmake_minimum_required(VERSION 3.14)

project(CoolCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

include_directories("include/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

################################
# GTest
################################
add_subdirectory(googletest)
enable_testing()
include_directories(googletest/googletest/include)
###############################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wconversion -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wextra-semi -Wformat=2")

set(LEXER_SOURCES src/lexer/Lexer.cpp)
set(PARSER_SOURCES ${LEXER_SOURCES} src/parser/Parser.cpp)
set(SEMANT_SOURCES ${PARSER_SOURCES} src/semant/Semant.cpp)

###############################
# Smoke
###############################
add_executable(lexer_smoke_test tests/lexer_tests/lexer_smoke_test.cpp ${LEXER_SOURCES})
target_link_libraries(lexer_smoke_test gtest_main)
add_test(lexer_smoke_test lexer_smoke_test)

add_executable(parser_smoke_test tests/parser_tests/parser_smoke_test.cpp ${PARSER_SOURCES})
target_link_libraries(parser_smoke_test gtest_main)
add_test(parser_smoke_test parser_smoke_test)

add_executable(semant_smoke_test tests/semant_tests/semant_smoke_test.cpp ${SEMANT_SOURCES})
target_link_libraries(semant_smoke_test gtest_main)
add_test(semant_smoke_test semant_smoke_test)
###############################

add_executable(lexer src/lexer/lexer_printer.cpp ${LEXER_SOURCES})
add_executable(parser src/parser/parser_printer.cpp ${PARSER_SOURCES})
add_executable(semant src/semant/semant_printer.cpp ${SEMANT_SOURCES})

