#include <print>
#include "../include/parser.hpp"
#include "../include/util.hpp"

int main() {

  string in = slurp_file("test.toml");

  Lexer L(in);
  Parser P(L);
  P.m_input = L.m_results;
  P.run();

  std::println("Henlo, worldo");
  return 0;
}
