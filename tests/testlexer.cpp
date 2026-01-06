#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "../include/util.hpp"

#include <cstdlib>

const string heading = "[title]";

bool lex_heading() {
  Lexer L;
  L.load(heading);
  L.scan();

  if (L.m_results.back().kind == TokenKind::Heading) return true;
  return false;
}

int main() {
  bool tests_ok = true;

  if (!lex_heading()) tests_ok = false;

  if (!tests_ok) return EXIT_FAILURE;
  std::println("YES");
  return EXIT_SUCCESS;
}
