cmake_minimum_required(VERSION 3.23)

add_executable(lexer_test testlexer.cpp)
target_link_libraries(lexer_test PRIVATE ltp)
