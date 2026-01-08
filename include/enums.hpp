#pragma once

#include <cstdint>

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
