#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "../include/util.hpp"


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

  if (!tests_ok) return 1;
  std::println("YES");
  return 0;
}
