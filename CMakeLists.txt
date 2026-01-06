cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_C_COMPILER "/usr/bin/clang")
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # ninja/makefile only

project(lilTomlParser VERSION 0.0.1)

add_subdirectory(tests)
enable_testing()

add_library(ltp STATIC)

target_sources(ltp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "include/"
    FILES
      "include/config.hpp"
      "include/parser.hpp"
      "include/util.hpp"
  PRIVATE
    "src/config.cpp"
    "src/parser.cpp"
    "src/util.cpp"
)

add_compile_options(
  Wall
  Wextra
  Wflexible-array-extensions
  Wfor-loop-analysis
  $<$<CONFIG:Debug>:
    ggdb
    gcall-site-info>
)

target_compile_definitions(ltp PRIVATE
  $<$<CONFIG:Release>:NDEBUG>
)

### TESTS ##############################

# Lexer
add_test(NAME lex-heading
  COMMAND lexer_test lex-heading
  WORKING_DIRECTORY "tests/"
)
add_test(NAME lex-comment
  COMMAND lexer_test lex-comment
  WORKING_DIRECTORY "tests/"
)

# Utils
add_test(
  NAME utils
  COMMAND utils_test
  WORKING_DIRECTORY "tests/"
)
