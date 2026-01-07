#pragma once

#include <print>
#include "util.hpp"

// namespace fs     = std::filesystem;
using     sview  = std::string_view;
using     string = std::string;


enum class TokenKind : uint8_t {
  Heading,
  Comment,
  AlNum,
  Delim,
  SettingKey,
  SettingValue,
  EOL,
  Other
};

struct Token {
  TokenKind kind;
  string value;

  const string kind_as_string() const {
    switch (this->kind) {
    case TokenKind::Heading:        return "Heading";
    case TokenKind::Comment:        return "Comment";
    case TokenKind::AlNum:          return "AlNum";
    case TokenKind::Delim:          return "Delim";
    case TokenKind::SettingKey:     return "SettingKey";
    case TokenKind::SettingValue:   return "SettingValue";
    case TokenKind::EOL:            return "EOL";
    case TokenKind::Other:          return "Other";
    }
  }

};

enum class PState : uint8_t {
  ParseHeading,
  ParseComment,
  ParseDelim,
  ParseAlNum,
  ParseSettingKey,
  ParseSettingValue,
  ParseEOL,
  ParseOther
};

enum class PEvent : uint8_t {
  ParseOk = 0, ParseFail, Input,
};

enum class PErrorKind {
  ParseError,
  OutOfRange,
  UnknownToken,
};

struct PError {
  PErrorKind  kind;
  string      msg;

  PError(PErrorKind kind, string msg) {
    this->kind = kind;
    this->msg  = msg;
  }

};


/// Returns parser state as string.
const string ps2s(PState s);
/// Returns parser event as string.
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
  Lexer(string &in) {
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
  /// Run the lexer on `m_scanbuf`, dumping results into `m_tokens`.
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
  Lexer                 &m_lexer;
  PState                m_prev_state;
  PState                m_state;
  std::vector<PError>   m_errors;
  std::vector<Token>    m_input;
  size_t                m_cursor = 0;
  std::vector<Token>    m_results;

  Parser(Lexer &lexer)
    : m_lexer(lexer) {
    // log_dbg("Parser: %s\n", ps2s(m_state).c_str());
  };

  void                  error(PErrorKind k, string msg);
  const PError*         get_last_error();
  std::vector<Token>    run();
  bool                  expect(TokenKind k);
  Token*                peek();
  Token&                get_tkn();
  void                  promote_alnum(TokenKind k);
  PState                next();
  void                  parse_alnum();
};
