#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "util.hpp"


int parse_heading() {
  Lexer L;
  L.load({"[section]"});
  L.scan();
  Parser P(L);
  P.run();
  if (P.m_results.back().kind == TokenKind::Heading) return 0;

  return 1;
}


int main(int argc, char **argv) {
  // args tracked to receive test names from CTest, if user runs single test
  string arg;
  if (argc > 1) arg = argv[1];

  if (arg == "parse-heading") return parse_heading();

  // should never get here, so report failure before returning
  // TODO: report failure
  return 1;
}
