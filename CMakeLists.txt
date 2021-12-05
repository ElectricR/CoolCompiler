cmake_minimum_required(VERSION 3.14)

project(CoolCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

include_directories("src/")

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
set(CODEGEN_SOURCES ${SEMANT_SOURCES} src/codegen/Codegen.cpp
    src/codegen/MIPS32/PrototypeGen.cpp
    src/codegen/MIPS32/DispTableGen.cpp
    src/codegen/MIPS32/ConstantGenerator.cpp
    src/codegen/MIPS32/MiscGen.cpp
    src/codegen/MIPS32/TextGen.cpp)

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
    # Codegen
    ###############################
    add_executable(prototype_smoke_test tests/codegen_tests/prototype_smoke_test.cpp ${CODEGEN_SOURCES})
    target_link_libraries(prototype_smoke_test gtest_main)
    add_test(prototype_smoke_test prototype_smoke_test)

    add_executable(disptable_smoke_test tests/codegen_tests/disptable_smoke_test.cpp ${CODEGEN_SOURCES})
    target_link_libraries(disptable_smoke_test gtest_main)
    add_test(disptable_smoke_test disptable_smoke_test)

    add_executable(constants_smoke_test tests/codegen_tests/constants_smoke_test.cpp ${CODEGEN_SOURCES})
    target_link_libraries(constants_smoke_test gtest_main)
    add_test(constants_smoke_test constants_smoke_test)

    add_executable(misc_smoke_test tests/codegen_tests/misc_smoke_test.cpp ${CODEGEN_SOURCES})
    target_link_libraries(misc_smoke_test gtest_main)
    add_test(misc_smoke_test misc_smoke_test)

    add_executable(text_smoke_test tests/codegen_tests/text_smoke_test.cpp ${CODEGEN_SOURCES})
    target_link_libraries(text_smoke_test gtest_main)
    add_test(text_smoke_test text_smoke_test)

    add_executable(codegen_smoke_test tests/codegen_tests/codegen_smoke_test.cpp ${CODEGEN_SOURCES})
    target_link_libraries(codegen_smoke_test gtest_main)
    add_test(codegen_smoke_test codegen_smoke_test)
    ###############################
###############################

add_executable(lexer src/lexer/lexer_printer.cpp ${LEXER_SOURCES})
add_executable(parser src/parser/parser_printer.cpp ${PARSER_SOURCES})
add_executable(semant src/semant/semant_printer.cpp ${SEMANT_SOURCES})
add_executable(codegen src/codegen/codegen.cpp ${CODEGEN_SOURCES})

