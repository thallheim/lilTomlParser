#include <clocale>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <print>
#include <stdexcept>
#include <string>

#include "../include/util.hpp"
#include "../include/config.hpp"


std::string slurp_file(const fs::path& path) {
  std::ifstream ifs(path, std::ios::binary);
  if (!ifs) {
    printf("<%s> Couldn't open file at '%s'\n", __FUNCTION__, path.c_str());
    throw std::runtime_error("File read failed");
  }

  const std::size_t size = fs::file_size(path);
  std::string data(size, '\0');
  ifs.read(data.data(), static_cast<std::streamsize>(size));
  return data;
}

bool fexists(const fs::path &file) { return fs::exists(file); }


// ----------------------------------------
// Stringy, texty stuff
// ----------------------------------------

bool match_any(sview str, std::initializer_list<sview> list) {
  for (auto cand : list) { if (str == cand) return true; }
  return false;
}

bool is_alnum(char c) {
  return std::isalnum(c) || c == '_' || c == '-';
}
