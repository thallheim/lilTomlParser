#pragma once

#include <print>

#include "util.hpp"
#include "enums.hpp"


// namespace fs     = std::filesystem;
using     sview  = std::string_view;
using     string = std::string;

struct Setting {
  string key;
  string val;

  Setting(sview key, sview val) {
    key = key;
    val = val;
  }

};

struct Config {
  fs::path              m_config_fpath;
  fs::path              m_dirpath_home;
  fs::path              m_dirpath_conf;
  std::vector<Setting>  m_settings;

  Config FromParseResult();

  void AddSetting(sview key, sview val) {
    m_settings.emplace_back(Setting(key, val));
  }
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

  Token() {};
  Token(TokenKind k, string v) {
    this->kind = k;
    this->value = v;
  }

};

struct PError {
  PErrorKind  kind;
  string      msg;

  PError(PErrorKind kind, string msg) {
    this->kind = kind;
    this->msg  = msg;
  }

};
