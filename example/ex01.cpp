#include <print>
#include <parser.hpp>
#include <util.hpp>

int main() {

  string in = "[heading1]"
    "# a comment\n"
    "thekey = theval\n"
    "\n";

  Lexer L(in);
  Parser P(L);
  P.m_input = L.m_results;
  P.run();

  return 0;
}
