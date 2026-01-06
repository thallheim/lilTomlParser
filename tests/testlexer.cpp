#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "../include/util.hpp"




int parse_heading() {
  const string heading = "[title]";
  Lexer L;
  L.load(heading);
  L.scan();

  if (L.m_results.back().kind == TokenKind::Heading) return 0;
  return 1;
}

int parse_comment() {
  const string comment = "# one comment, please";
  Lexer L;
  L.load(comment);
  L.scan();

  if (L.m_results.back().kind == TokenKind::Comment) return 0;
  return 1;
}

int parse_other() {
  const string bad = "bad input yo";
  Lexer L;
  L.load(bad);
  L.scan();

  if (L.m_results.back().kind == TokenKind::Other) return 0;
  return 1;
}


int main(int argc, char **argv) {
  string arg;
  if (argc > 1) arg = argv[1];

  bool tests_ok = true;

  if (arg == "lex-heading") return parse_heading();
  if (arg == "lex-comment") return parse_comment();



  // if (!tests_ok) return 1;

  return 1; // should never get here, so report failure
}
