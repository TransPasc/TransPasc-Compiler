#pragma once

#include "cli/argparse.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace fs = std::filesystem;

struct OutputConfig;

std::string get_extension(const std::string &format);
std::string resolve_output_path(const OutputConfig &config);
std::string detect_format_from_extension(const std::string &output_path);

struct OutputConfig {
  std::string input_path;
  std::string output_path;
  std::string format;
  bool verbose;
  bool show_version = false;

  // 格式验证方法
  bool is_valid_format() {
    static const std::unordered_set<std::string> valid_formats = {
        "c", "llvm-ir", "risc-v", "token", "ast"};
    if (format.empty()) {
      format = detect_format_from_extension(output_path);
    }
    return valid_formats.find(format) != valid_formats.end();
  }
};

OutputConfig parse_arguments(int argc, char **argv);
