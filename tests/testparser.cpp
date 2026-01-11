#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "tests.hpp" // for testing string (TEST_STR)
#include "util.hpp"

using tk = TokenKind;

int parser_run() {
  Lexer L(TEST_STR);
  Parser P(L);
  P.run();
  size_t i = 0;

  if (P.m_input.size() != 0) {
    for (const auto &c : P.m_input) {
      if (c.kind == tk::EOL) {
        printf("%3zu: Tkn %s\n", i, c.kind_as_string().c_str());
      } else {
        printf("%3zu: Tkn %s %s\n", i, c.kind_as_string().c_str(), c.value.c_str());
      }
      i++;
    }
  }

  return 0;
}


int main(int argc, char **argv) {
  // args tracked to receive test names from CTest, if user runs single test
  string arg;
  if (argc > 1) arg = argv[1];

  if (arg == "parser-run") return parser_run();

  // should never get here, so report failure before returning
  // TODO: report failure
  return 1;
}
