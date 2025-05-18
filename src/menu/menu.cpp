#ifndef __KPC_VERSION__
#define __KPC_VERSION__ "Unknown version"
#endif
#include "menu/menu.hpp"
#include "codeGenerate/cLangGenerator.hpp"
#include "codeGenerate/llvmIrGenerator.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
Menu::Menu() { showWelcomeMessage(); }
Menu::~Menu() {}
void Menu::showWelcomeMessage() {
  std::cout << "Welcome to the KPC!" << std::endl;
  std::cout << "The best Compiler for Pascal in the world future (The King of "
               "Pascal Compiler)"
            << std::endl;
  std::cout << "user -h or -help for more information." << std::endl;
}
void Menu::showMenu() {

  std::cout << "Displaying menu options..." << std::endl;
}
void Menu::showVersion() {
  std::cout << "KPC version " << TOSTRING(__KPC_VERSION__) << std::endl;
}
void Menu::readFromFile(Driver &driver, const OutputConfig &config) {

  auto inputFile = new ifstream(config.input_path);
  if (inputFile->fail()) {
    std::cerr << "Error opening file." << std::endl;
    exit(1);
  }
  driver.switchInputStream(inputFile);

  std::cout << "Reading from file: " << config.input_path << std::endl;

  // default output file name is ${dir}/${basename}.c
  if (config.output_path.empty()) {
    std::filesystem::path inputPath(config.input_path);
    std::string outputFileName = inputPath.stem().string() + ".c";
    driver.setOutputFileName(outputFileName);
  }
}
void Menu::generateCode(Driver &driver, const std::string &format) {
  const static auto format2generator = std::unordered_map<
      std::string, std::function<void(Driver &)>>{
      {"c",
       [](Driver &driver) {
         driver.generateCode(make_shared<CLangGenerator>());
       }},
      {"llvm-ir",
       [](Driver &driver) {
         std::cerr
             << "\033[93mLLVM IR code generation is not implemented yet.\033[0m"
             << std::endl;
         driver.generateCode(make_shared<LLVMIrGenerator>());
       }},
      {"risc-v",
       [](Driver &driver) {
         std::cerr
             << "\033[93mRISC-V code generation is not implemented yet.\033[0m"
             << std::endl;
       }},
      {"token", [](Driver &driver) { driver.printTokens(); }},
      {"ast", [](Driver &driver) { driver.printAST(); }}};

  auto it = format2generator.find(format);
  if (it != format2generator.end()) {
    it->second(driver);
  } else {
    std::cerr << "Unrecognized format: " << format << std::endl;
  }
}
void Menu::parse(Driver &driver) {
  int res = driver.parse();
  if (res != 0) {
    std::cerr << "Parse failed. Error code: " << res << std::endl;
    return;
  }
  driver.printAST();
}
void Menu::setOutputFileName(Driver &driver, const std::string &path) {
  driver.setOutputFileName(path);
}
