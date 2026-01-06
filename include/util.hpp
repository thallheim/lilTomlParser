#pragma once

#include <filesystem>
#include <initializer_list>
#include <vector>

#include "config.hpp"

namespace fs = std::filesystem;
using strvec = std::vector<const char*>;


std::string slurp_file(const fs::path& path);
bool        file_exists(const fs::path& file);


// ----------------------------------------
// Stringy, texty stuff
// ----------------------------------------

/// Returns its input lowercased.
inline auto auto_lcase = [](unsigned char c) -> unsigned char {
  return std::tolower(c);
};

/// Returns true if any string in 'list' matches 'str'.
/// Case-sensitive.
bool match_any(sview str, std::initializer_list<sview> list);

/// Returns whether `c` is alphanumeric, '-', or '_'.
bool is_alnum(char c);
