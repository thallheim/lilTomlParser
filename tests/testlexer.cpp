#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "util.hpp"

using tk = TokenKind;

int lexer_scan() {
  const string str = R"([s1]
_yes = no-no
# un commento
@
bad)";

  Lexer L;
  L.load(str);
  L.scan();

  size_t i = 0;

  /* NOTE: Because of shell process forking, the prints below will only show up
     if calling the 'lexer_test' bin directly. They exist mostly because they
     made writing the rest of the test body faster. Call it directly and pass it
     'lexer-scan' if you really want to see them without changing anything else.
  */
  if (L.m_results.size() != 0) {
    for (const auto &c : L.m_results) {
      if (c.kind == tk::EOL) {
        printf("%zu: Got %s\n", i, c.kind_as_string().c_str());
      } else {
        printf("%zu: Got %s %s\n", i, c.kind_as_string().c_str(), c.value.c_str());
      }
      i++;
    }
  }
  // confirm expected results
  if (L.m_results.at(0).kind  == tk::Delim &&
      L.m_results.at(1).kind  == tk::AlNum &&
      L.m_results.at(3).kind  == tk::Delim &&
      L.m_results.at(4).kind  == tk::EOL &&
      L.m_results.at(5).kind  == tk::AlNum &&
      L.m_results.at(9).kind  == tk::Delim &&
      L.m_results.at(12).kind == tk::AlNum &&
      L.m_results.at(15).kind == tk::EOL &&
      L.m_results.at(16).kind == tk::Comment &&
      L.m_results.at(28).kind == tk::Other) {

    return 0;
  }

  std::print(stderr, "TEST FAIL: lexer_scan: Results don't match expected output\n");
  return 1;
}

int main(int argc, char **argv) {
  // args tracked to receive test names from CTest, if user runs single test
  string arg;
  if (argc > 1) arg = argv[1];

  bool tests_ok = true;

  if (arg == "lexer-scan")   return lexer_scan();



  // should never get here, so report failure before returning
  // TODO: report failure
  return 1;
}
