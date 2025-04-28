#include "cli/cli.hpp"
#include <filesystem>

namespace fs = std::filesystem;

OutputConfig parse_arguments(int argc, char **argv) {
  argparse::ArgumentParser program("kpc");

  // 核心参数定义
  program.add_argument("-i", "--input")
      .default_value("")
      .help("Input Pascal source file");

  program.add_argument("-o", "--output")
      .help("Output path with filename and format extension");

  program.add_argument("-f", "--format")
      .default_value(std::string("c"))
      .choices("c", "llvm-ir", "risc-v", "token", "ast")
      .help("Output format (default: c, supported: c, llvm-ir, riscv, token, "
            "ast)");

  program.add_argument("-v", "--verbose")
      .default_value(false)
      .implicit_value(true)
      .help("Enable verbose mode");

  // 元信息参数
  program.add_argument("--version")
      .default_value(false)
      .implicit_value(true)
      .help("Show version info");

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << "\n\n";
    std::cerr << program;
    exit(1);
  }

  // 构建输出配置
  OutputConfig config;
  config.input_path = program.get<std::string>("--input");
  config.verbose = program.get<bool>("--verbose");
  config.show_version = program.get<bool>("--version");

  // 自动生成输出路径
  if (program.is_used("--output")) {
    config.output_path = program.get<std::string>("--output");
  } else {
    fs::path input_path(config.input_path);
    std::string ext = get_extension(program.get<std::string>("--format"));
    config.output_path = input_path.replace_extension(ext).string();
  }

  return config;
}

std::string get_extension(const std::string &format) {
  static std::unordered_map<std::string, std::string> ext_map = {
      {"c", ".c"},
      {"llvm-ir", ".ll"},
      {"risc-v", ".s"},
      {"token", ".tokens"},
      {"ast", ".ast"}};
  return ext_map.at(format);
}
std::string resolve_output_path(const OutputConfig &config) {

  fs::path output_path(config.output_path);
  if (output_path.is_relative()) {
    output_path = fs::current_path() / output_path;
  }

  if (!fs::exists(output_path.parent_path())) {
    fs::create_directories(output_path.parent_path());
  }

  return output_path.string();
}

std::string detect_format_from_extension(const std::string &output_path) {
  static std::unordered_map<std::string, std::string> ext_to_format = {
      {".c", "c"},
      {".ll", "llvm-ir"},
      {".s", "risc-v"},
      {".tokens", "token"},
      {".ast", "ast"}};
  if (output_path.empty()) {
    throw std::runtime_error("No input file");
  }
  fs::path p(output_path);
  std::string ext = p.extension().string();

  if (ext_to_format.count(ext)) {
    return ext_to_format[ext];
  } else {
    throw std::runtime_error("Unrecognized output file extension: " + ext);
  }
}
