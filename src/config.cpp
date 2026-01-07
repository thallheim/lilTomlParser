#include <cstdlib>
#include <filesystem>
#include <format>

#include "../include/config.hpp"
#include "../include/util.hpp"

Setting::Setting(sview key, sview val) {
  key = key;
  val = val;
}

// Config Config::FromParseResult(std::vector<Token>) {
//   Config cfg;
//   for (auto const& c : )
//   return cfg;
// }

void Config::AddSetting(sview key, sview val) {
  m_settings.emplace_back(Setting(key, val));
}


void validate_config() {
  // try binary's dir
  std::println("{}", "oi");

  // try config dir
}


void validate_header(fs::path p, sview header) {

}
