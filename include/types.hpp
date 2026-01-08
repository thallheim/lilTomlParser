#pragma once

#include <print>

#include "util.hpp"
#include "enums.hpp"
#include "types.hpp"

// namespace fs     = std::filesystem;
using     sview  = std::string_view;
using     string = std::string;

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

struct PError {
  PErrorKind  kind;
  string      msg;

  PError(PErrorKind kind, string msg) {
    this->kind = kind;
    this->msg  = msg;
  }

};
