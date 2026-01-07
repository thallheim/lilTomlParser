#pragma once

#include <filesystem>
#include <print>
#include <string_view>
#include <vector>


namespace fs      = std::filesystem;
using     sview   = std::string_view;
using     string  = std::string;


struct Setting {
  string key;
  string val;

  Setting(sview key, sview val);
};

struct Config {
  fs::path              m_config_fpath;
  fs::path              m_dirpath_home;
  fs::path              m_dirpath_conf;
  std::vector<Setting>  m_settings;

  Config FromParseResult();
  void AddSetting(sview key, sview val);
};


void validate_config();
void validate_header(fs::path p, sview header);
