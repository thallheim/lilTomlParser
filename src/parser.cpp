#include <cctype>
#include <stdexcept>
#include "../include/parser.hpp"
#include "../include/util.hpp"
#include "enums.hpp"


// ----------------------------------------------------------------------
// Lexer
// ----------------------------------------------------------------------
void Lexer::load(const string &str) {
  m_inbuf = str;
  m_cursor = 0;
}

void Lexer::scan() {
  using tk = TokenKind;
  m_cursor = 0; // reset cursor
  Token  out;   // output 'buffer' obj.
  char   cc;    // current char

  while (m_cursor < m_inbuf.size()) { // scanning loop
    cc = get_ch();
    out.value = cc;

    if (eol()) { out.kind = tk::EOL; }
    else if (std::isspace(cc)) { m_cursor++; continue; }
    else if (cc == '[' || cc == ']' || cc == '=') { out.kind = tk::Delim; }
    // TODO: don't skip WS inside comments
    else if (cc == '#') { out.kind = tk::Comment; }
    else if (is_alnum(cc)) { out.kind = tk::AlNum; }
    else { out.kind = tk::Other; }

    m_results.emplace_back(out);
    m_cursor++;
  }
}

inline char Lexer::get_ch() {
  if (m_cursor > m_inbuf.size())
    return '\0';
  return m_inbuf[m_cursor];
}

bool Lexer::eol() {
  if (get_ch() == '\n' || get_ch() == '\0')
    return true;
  return false;
}

// FIXME: bwoken
char Lexer::peek(bool skip_ws) {
  size_t i = m_cursor+1;
  if (!skip_ws) return m_inbuf[i];

  if (is_alnum(m_inbuf[i])) return m_inbuf[i];

  while (std::isspace(m_inbuf[i])) {
    i++;
    if (is_alnum(m_inbuf[i])) return m_inbuf[i];
  }

  return '\0';
}

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
  string section = "@default@";

  if (!m_input.empty()) {
    parsing_loop();
  } else { // empty input
    // TODO:
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
    case PState::ParseComment:
    case PState::ParseDelim:
    case PState::ParseAlNum:
    case PState::ParseSettingKey:
    case PState::ParseSettingValue:
    case PState::ParseEOL:
    case PState::ParseOther:
      break;
    }
  }
}

void Parser::push_section(string s) { m_sections.emplace_back(s); }

void Parser::push_kvp(string k, string v) {
  if (m_sections.empty()) {
    m_kvps.emplace_back("main", k, v);
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
