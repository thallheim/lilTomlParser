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

  std::println("Parser inputs: {}", P.m_input.size());
  std::printf("Sections: %zu\n", P.m_sections.size());
  std::println("Sections: {}", P.m_sections.size());

  return 0;
}
