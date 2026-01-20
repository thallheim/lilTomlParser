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
  Idle = 0,
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
  ParseOk = 0, ParseFail, NewInput,
};

enum class PErrorKind {
  ParseError,
  OutOfRange,
  UnknownToken,
};
