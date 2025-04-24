#pragma once
#include "cli/argparse.hpp"
#include "cli/cli.hpp"
#include "driver.h"
/**
 * @brief This class handles the menu operations and argument parsing.
 *
 */
class Menu {

public:
  Menu();
  ~Menu();
  static void showWelcomeMessage();
  void showMenu();
  void showVersion();
  void readFromFile(Driver &driver, const OutputConfig &config);
  void setOutputFileName(Driver &driver, const std::string &path);
  void generateCode(Driver &driver, const std::string &format);

private:
  void parse(Driver &driver);
};
