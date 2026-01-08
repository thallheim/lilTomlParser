#include <cctype>
#include <stdexcept>
#include "../include/parser.hpp"
#include "../include/util.hpp"


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
  string buf;   // tkn value
  Token  t;     // output
  char   curr;  // current char

  while (m_cursor < m_inbuf.size()) { // scanning loop
    if (get_ch() == '\n' || get_ch() == '\0') {
      t.kind = tk::EOL;
      buf = get_ch();
      m_results.emplace_back(t);
      buf.clear();
      m_cursor++;
      continue;
    }

    while (get_ch() == ' ') { m_cursor++; }
    curr = get_ch();

    if (curr == '[') {
      t.kind = tk::Heading;
      m_cursor++;
      while (is_alnum(m_inbuf[m_cursor])) {
        buf += m_inbuf[m_cursor++];
      }
      t.value = buf; // stash heading value
    } else if (curr == ']') {
      m_cursor++;
      continue;
    } else if (curr == '=') {
      t.kind = tk::Delim;
      t.value = '=';
      m_cursor++;
    } else if (is_alnum(curr)) {
      t.kind = tk::AlNum;
      while (isalnum(get_ch()) && !std::isspace(get_ch())) {
        buf += m_inbuf[m_cursor++];
      }
      t.value = buf; // stash alnum value
    } else if (curr == '#') {
      t.kind = tk::Comment;
      while (!eol()) {
        buf += m_inbuf[m_cursor++];
      }
      t.value = buf; // stash comment value
    } else {
      t.kind = tk::Other;
      while (!eol()) {
        buf += m_inbuf[m_cursor++];
      }
      t.value = buf; // stash unknown token value

    }

    m_results.emplace_back(t);
    buf.clear();
  } // /scan loop
}

char Lexer::get_ch() {
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

std::vector<Token> Parser::run() {
  using tk = TokenKind;
  using ps = PState;

  string    section;

  printf("<CfgParser> [b]%zu[/] tokens to parse\n", m_input.size());

  // Main parsing loop
  while (m_cursor < m_input.size()) {

    m_prev_state = m_state;

    switch (get_tkn().kind) {
    case tk::Heading: {
      section = get_tkn().value;
      m_state = PState::ParseHeading;
      printf("<CfgParser> Got section: '%s'\n", get_tkn().value.c_str());
      break;
    }
    case tk::Comment: {
      m_state = ps::ParseComment;
      printf("<CfgParser> Got comment: '%s'\n", get_tkn().value.c_str());
      break;
    }
    case tk::AlNum: {
      m_state = ps::ParseAlNum;
      parse_alnum();
      break;
    }
    case tk::Delim:        { m_state = ps::ParseDelim;                break; }
    case tk::SettingKey:   { m_state = ps::ParseSettingKey;           break; }
    case tk::SettingValue: { m_state = ps::ParseSettingValue;         break; }
    case tk::EOL: {
      printf("<CfgParser> Got EOL\n");
      m_state = ps::ParseEOL;
      if (m_prev_state == ps::ParseSettingKey) {
        m_input[m_cursor-1].kind = tk::Other; // "downgrade" prev. token
      }

      break;
    }
    case tk::Other: {
      m_state = ps::ParseOther;
      printf("<CfgParser> Got unknown token: '%s'\n", get_tkn().value.c_str());
      // TODO: reg. & log parse error: unknown tkn
      break;
    }
    }
    m_results.emplace_back();
    m_cursor++;
  }
  printf("<CfgParser> Done (%zu errors) (%zu results)\n",
         m_errors.size(),m_results.size());

  return m_results;
}

bool Parser::expect(TokenKind k) {
  if (m_input[m_cursor].kind == k) return true;
  return false;
}

// TODO: don't throw
Token* Parser::peek() {
  if (m_cursor+1 <= m_input.size()) return &m_input[m_cursor+1];
  const string emsg = "<CfgParser> Tried to peek past end of input\n";
  printf("%s", emsg.c_str());
  error(PErrorKind::OutOfRange, emsg);
  throw std::out_of_range("Cant peek past end of input");
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
  case pe::Input:               return "Input received";
  }
}
