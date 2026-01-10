#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "tests.hpp" // for testing string (TEST_STR)
#include "util.hpp"


int parser_run() {
  Lexer L;
  L.load(TEST_STR);
  L.scan();
  Parser P(L);
  P.m_input = L.m_results;
  P.run();
  if (P.m_results.at(0).kind == TokenKind::Heading) return 0;

  return 0;
}

int parse_comment() {
  Lexer L;
  L.load({"# un commento"});
  L.scan();
  Parser P(L);
  P.m_input = L.m_results;
  P.run();
  if (P.m_results.back().kind == TokenKind::Comment) return 0;

  return 0;
}

int parse_delim() {
  Lexer L;
  L.load({"="});
  L.scan();
  Parser P(L);
  P.m_input = L.m_results;
  P.run();
  if (P.m_results.back().kind == TokenKind::Delim) return 0;

  return 0;
}

int parse_other() {
  Lexer L;
  L.load({"bad"});
  L.scan();
  Parser P(L);
  P.m_input = L.m_results;
  P.run();
  if (P.m_results.back().kind == TokenKind::Other) return 0;

  return 0;
}


int main(int argc, char **argv) {
  // args tracked to receive test names from CTest, if user runs single test
  string arg;
  if (argc > 1) arg = argv[1];

  if (arg == "parse-heading") return parser_run();
  if (arg == "parse-comment") return parse_comment();
  if (arg == "parse-delim")   return parse_delim();
  if (arg == "parse-other")   return parse_other();

  // should never get here, so report failure before returning
  // TODO: report failure
  return 1;
}
