#pragma once

#include <filesystem>
#include <print>
#include <string_view>
#include <vector>


namespace fs      = std::filesystem;
using     sview   = std::string_view;
using     string  = std::string;

enum class SettingKind : uint8_t {
  Alpha, Num, Bool
};

struct Setting {
  SettingKind   kind;
  string        key;
  string        val;
};

struct Config {
public:
  fs::path              m_config_fpath;
  fs::path              m_dirpath_home;
  fs::path              m_dirpath_conf;
  std::vector<Setting>  m_settings;
};


void validate_config();
void validate_header(fs::path p, sview header);
