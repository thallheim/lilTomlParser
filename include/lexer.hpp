#pragma once
#include "types.hpp"

using sview  = std::string_view;
using tk     = TokenKind;
using std::map;
using std::pair;
using std::string;
using std::vector;

// ----------------------------------------------------------------------
// Lexer
// ----------------------------------------------------------------------
struct Lexer {
  string         m_inbuf;
  int            m_cursor;
  char           m_prev;
  vector<Token>  m_results;

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
