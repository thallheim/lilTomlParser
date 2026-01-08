#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "util.hpp"



int lex_heading() {
  const string heading = "[section_heading]";
  Lexer L;
  L.load(heading);
  L.scan();

  if (L.m_results.back().kind == TokenKind::Heading) return 0;
  return 1;
}

int lex_comment() {
  const string comment = "# one comment, please";
  Lexer L;
  L.load(comment);
  L.scan();

  if (L.m_results.back().kind == TokenKind::Comment) return 0;
  return 1;
}

int lex_alnum() {
  Lexer L;
  L.load({"h"});
  L.scan();
  if (L.m_results.back().kind == TokenKind::AlNum) return 0;

  return 1;
}


int main(int argc, char **argv) {
  // args tracked to receive test names from CTest, if user runs single test
  string arg;
  if (argc > 1) arg = argv[1];

  bool tests_ok = true;

  if (arg == "lex-heading") return lex_heading();
  if (arg == "lex-comment") return lex_comment();
  if (arg == "lex-alnum")   return lex_alnum();
  if (arg == "lex-other")   return lex_eol();



  // should never get here, so report failure before returning
  // TODO: report failure
  return 1;
}
