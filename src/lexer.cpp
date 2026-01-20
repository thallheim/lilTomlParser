#include "../include/lexer.hpp"

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
