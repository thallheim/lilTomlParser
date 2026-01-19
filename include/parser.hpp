#pragma once

#include <print>
#include <map>
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
  Lexer                        *m_lexer;
  PState                       m_prev_state;
  PState                       m_state;
  map<TokenKind, PState>       m_state_map;
  vector<PError>               m_errors;
  vector<Token>                m_input;
  size_t                       m_cursor = 0;
  vector<pair<string, string>> m_results;

  Parser(Lexer *lexer) : m_lexer(lexer) {
    // set idle to start
    m_state = PState::Idle;

    // Populate state map
    m_state_map.emplace(tk::AlNum,        PState::ParseAlNum);
    m_state_map.emplace(tk::Comment,      PState::ParseComment);
    m_state_map.emplace(tk::Delim,        PState::ParseDelim);
    m_state_map.emplace(tk::EOL,          PState::ParseEOL);
    m_state_map.emplace(tk::Heading,      PState::ParseHeading);
    m_state_map.emplace(tk::SettingKey,   PState::ParseSettingKey);
    m_state_map.emplace(tk::SettingValue, PState::ParseSettingValue);
    m_state_map.emplace(tk::Other,        PState::ParseOther);

    if (lexer == nullptr) { // no lexer == no good
      // TODO: add error kind 'fatal' or some such?
      error(PErrorKind::ParseError, "Lexer ptr is null");
      std::print(stderr, "ERROR: Lexer ptr is null\n");
    } else {
      // TODO: check to make sure lexer isn't empty?
      m_input = m_lexer->m_results;
    }
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
  void              parsing_loop();
};
