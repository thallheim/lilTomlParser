cmake_minimum_required(VERSION 3.23)

add_executable(lexer_test testlexer.cpp)
add_executable(utils_test testutils.cpp)

target_link_libraries(lexer_test PRIVATE ltp)
target_link_libraries(utils_test PRIVATE ltp)
