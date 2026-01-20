#pragma once

#include <print>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

#include "enums.hpp"
#include "types.hpp"

using sview  = std::string_view;
using tk     = TokenKind;
using std::string;
using std::map;
using std::pair;
using std::vector;

/// Parser state as const string.
const string ps2s(PState s);
/// Parser event as const string.
const string pe2s(PEvent e);


// ----------------------------------------------------------------------
// Lexer
// ----------------------------------------------------------------------
struct Lexer {
  string                    m_inbuf;
  int                       m_cursor;
  char                      m_prev;
  std::vector<Token>        m_results;

  /// Convenience ctor to load and scan `in`.
  Lexer(string in) {
    load(in);
    scan();
  }
  /// Default ctor. Loading/scanning needs to be done separately.
  /// See `load()`, `scan()`.
  Lexer() {}
  /// Load a string into `m_inbuf`.
  void load(const string &str);
  /// Tokenise the input buffer, dumping results into `m_tokens`.
  void scan();
  /// Return char at current cursor pos. in the input buffer.
  char get_ch();
  /// Returns whether current char is null or newline.
  bool eol();
  /// Peek next non-whitespace char.
  char peek(bool skip_ws = true);


}; // lexer

// ----------------------------------------------------------------------
// Parser
// ----------------------------------------------------------------------
struct Parser {
  Lexer                                       *m_lexer;
  PState                                      m_prev_state;
  PState                                      m_state;
  map<TokenKind, PState>                      m_state_map;
  vector<PError>                              m_errors;
  // TODO: save a copy: make ptr to lexer output vec
  vector<Token>                               m_input;
  size_t                                      m_cursor = 0;
  vector<string>                              m_sections;
  vector<std::tuple<string, string, string>>  m_kvps;

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
