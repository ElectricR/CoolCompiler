cmake_minimum_required(VERSION 3.14)

project(CoolCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wshadow -Wconversion -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wextra-semi -Wformat=2")

################################
# GTest
################################
add_subdirectory(googletest)
enable_testing()

include_directories(googletest/googletest/include)

add_executable(lexer_smoke_test tests/lexer_tests/lexer_smoke_test.cpp)
target_link_libraries(lexer_smoke_test gtest_main)
add_test(lexer_smoke_test lexer_smoke_test)

###############################

add_executable(lexer src/lexer/lexer.cpp)
