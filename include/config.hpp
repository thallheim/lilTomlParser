#pragma once

#include <filesystem>
#include <print>
#include <string_view>
#include <vector>


namespace fs      = std::filesystem;
using     sview   = std::string_view;
using     string  = std::string;




void validate_config();
void validate_header(fs::path p, sview header);
