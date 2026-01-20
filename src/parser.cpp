#include <cctype>
#include <stdexcept>
#include <utility>
#include "../include/parser.hpp"
#include "../include/util.hpp"
#include "enums.hpp"


// ----------------------------------------------------------------------
// Parser
// ----------------------------------------------------------------------
void Parser::error(PErrorKind k, string msg) {
  m_errors.emplace_back(PError(k, msg));
}

const PError* Parser::get_last_error() {
  if (!m_errors.empty()) return &m_errors.back();
  return nullptr;
}

void Parser::run() {
  using tk = TokenKind;
  using ps = PState;
  // string section = "@default@";

  if (!m_input.empty()) { parsing_loop(); } // all's well - parse away
  else {                                    // empty input
    const string emsg = "<CfgParser> Input empty";
    error(PErrorKind::ParseError, emsg);
    std::print(stderr, "ERROR: {}", emsg);
  }

}

bool Parser::expect(TokenKind k) {
  if (m_input[m_cursor].kind == k) return true;
  return false;
}

// TODO: don't throw
Token* Parser::peek() {
  if (m_cursor+1 <= m_input.size()) return &m_input[m_cursor+1];
  const string emsg = "<CfgParser> Tried to peek past end of input\n";
  std::print("{}\n", emsg);
  error(PErrorKind::OutOfRange, emsg);
  throw std::out_of_range("Can't peek past end of input");
}

// TODO: don't throw
Token& Parser::get_tkn() {
  if (m_cursor <= m_input.size()) return m_input[m_cursor];
  const string emsg = "<CfgParser> Tried to get token past end of input vector\n";
  printf("%s", emsg.c_str());
  error(PErrorKind::OutOfRange, emsg);
  throw std::out_of_range("Can't get token past end of input vector");
}

void Parser::promote_alnum(TokenKind new_kind) {
  using tk = TokenKind;
  Token &current = get_tkn();

  if (current.kind == tk::AlNum) { current.kind = new_kind; }
  else {
    const string emsg = "<CfgParser> Can't promote non-AlNum token\n";
    printf("%s", emsg.c_str());
    error(PErrorKind::ParseError, emsg);
  }
}

PState Parser::next() {
  switch (m_state) {
  case PState::Idle:
  case PState::ParseHeading:
  case PState::ParseComment:
  case PState::ParseDelim:
  case PState::ParseAlNum:
  case PState::ParseSettingKey:
  case PState::ParseSettingValue:
  case PState::ParseEOL:
  case PState::ParseOther:
    break;

  }

  throw std::runtime_error("Unreachable :(");
}

void Parser::parse_alnum() {
  using ps = PState;
  using tk = TokenKind;

  if (peek()->kind == tk::Delim) {
    m_prev_state = ps::ParseSettingKey;
    promote_alnum(TokenKind::SettingKey);
    printf("<CfgParser> Got Key:\t%s\n", get_tkn().value.c_str());
  }
  else if (m_prev_state == ps::ParseDelim) {
    promote_alnum(TokenKind::SettingValue);
    m_prev_state = PState::ParseSettingValue;
    printf("<CfgParser> Got Value:\t%s\n", get_tkn().value.c_str());
  }
  else {
    m_input[m_cursor].kind = tk::Other;
    printf("[red]<CfgParser> Unknown token:\t%s[/]\n", get_tkn().value.c_str());
  }
}

void Parser::_init(Lexer *lexer) {
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
}

void Parser::parsing_loop() {
  using ps = PState;
  using tk = TokenKind;
  m_cursor = 0; // reset cursor

  for (auto &t : m_input) {
    m_prev_state = m_state;
    m_state = m_state_map[t.kind];

    switch (m_state) {
    case PState::Idle:
    case PState::ParseHeading:
      _push_section(t.value);
      break;
    case PState::ParseComment:
      break;
    case PState::ParseDelim:
      break;
    case PState::ParseAlNum:
      break;
    case PState::ParseSettingKey:
      _push_key(t.value);
      break;
    case PState::ParseSettingValue:
      break;
    case PState::ParseEOL:
      break;
    case PState::ParseOther:
      break;
    }
  }
}

void Parser::_push_section(string s) { m_sections.emplace_back(s); }

void Parser::_push_key(string k) {
  // TODO: check curr_section: if '@default@' -> set to 'main'
  if (m_sections.empty()) {
    m_kvps.emplace_back("main", k, "");
  } else {
    m_kvps.emplace_back(m_sections.back(), k, "");
  }
}

void Parser::_push_val(string v) {
  if (m_sections.empty()) {
    auto out = std::make_tuple("", "", v);

  } else {
    m_kvps.emplace_back(m_sections.back(), k, v);
  }

}

// ----------------------------------------------------------------------
// Stringification helpers
// ----------------------------------------------------------------------
const string ps2s(PState s) {
  using ps = PState;
  switch (s) {
  case ps::ParseHeading:        return "ParseHeading";
  case ps::ParseComment:        return "ParseComment";
  case ps::ParseDelim:          return "ParseDelim";
  case ps::ParseAlNum:          return "ParseAlNum";
  case ps::ParseSettingKey:     return "ParseSettingKey";
  case ps::ParseSettingValue:   return "ParseSettingValue";
  case ps::ParseEOL:            return "EOL";
  case ps::ParseOther:          return "ParseOther";
  }
}

const string pe2s(PEvent e) {
  using pe = PEvent;
  switch (e) {
  case pe::ParseOk:             return "ParseOk";
  case pe::ParseFail:           return "ParseFail";
  case pe::NewInput:               return "Input received";
  }
}
