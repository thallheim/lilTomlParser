#pragma once

#include <print>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

#include "enums.hpp"
#include "types.hpp"
#include "lexer.hpp"

using sview  = std::string_view;
using tk     = TokenKind;
using std::string;
using std::map;
using std::pair;
using std::tuple;
using std::vector;


// Utils

/// Parser state as const string.
const string ps2s(PState s);
/// Parser event as const string.
const string pe2s(PEvent e);

enum class PImplementation : uint8_t {
  Default = 0,

};


// ----------------------------------------------------------------------
// Parser
// ----------------------------------------------------------------------
struct Parser {
  Lexer                                  *m_lexer;
  PState                                 m_prev_state;
  PState                                 m_state;
  map<TokenKind, PState>                 m_state_map;
  vector<PError>                         m_errors;
  // TODO: save a copy: make ptr to lexer output vec
  vector<Token>                          m_input;
  size_t                                 m_cursor = 0;
  vector<string>                         m_sections;
  vector<tuple<string, string, string>>  m_kvps;

  Parser(Lexer *lexer) : m_lexer(lexer) {
    _init(lexer);
  };

  void              error(PErrorKind k, string msg);
  const PError*     get_last_error();
  void              run();
  bool              expect(TokenKind k);
  Token*            peek();
  Token&            get_tkn();
  void              promote_alnum(TokenKind k);
  PState            next();
  void              parse_alnum();

private:
  void              _init(Lexer *lexer);
  void              parsing_loop();
  void              push_section(string s);
  void              push_kvp(string k, string v);
};
