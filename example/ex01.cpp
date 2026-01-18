#include <print>
#include <parser.hpp>
#include <util.hpp>

int main() {

  string in = "[heading1]"
    "# a comment\n"
    "thekey = theval\n"
    "\n";

  Lexer L(in);
  Parser P(&L);
  P.run();

  std::print("Parser inputs: {}\n", P.m_input.size());

  return 0;
}
