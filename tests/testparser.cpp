#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "tests.hpp" // for testing string (TEST_STR)
#include "util.hpp"

using tk = TokenKind;

int parser_run() {
  Lexer L;
  L.load(TEST_STR);
  L.scan();
  Parser P(L);
  P.m_input = L.m_results;
  P.run();
  size_t i;

  if (P.m_results.size() != 0) {
    for (const auto &c : P.m_results) {
      if (c.kind == tk::EOL) {
        printf("%zu: Got %s\n", i, c.kind_as_string().c_str());
      } else {
        printf("%zu: Got %s %s\n", i, c.kind_as_string().c_str(), c.value.c_str());
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
