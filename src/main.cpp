#include "cli/cli.hpp"
#include "driver.h"
#include "menu/menu.hpp"
#include "parser.hpp"
#include "scanner.h"
#include <iostream>
#include <memory>

using namespace XYZ;

int main(int argc, char *argv[]) {

  Driver driver;
  auto config = parse_arguments(argc, argv);
  if (!config.is_valid_format()) {
    std::cout << "Invalid configuration format." << endl;
    return 1;
  }

  auto menuManager = std::make_shared<Menu>();

  if (config.show_version) {
    menuManager->showVersion();
    return 0;
  }

  driver.set_verbose(config.verbose);
  if (!config.input_path.empty()) {
    menuManager->readFromFile(driver, config);
  }
  if (!config.output_path.empty()) {
    menuManager->setOutputFileName(driver, config.output_path);
  }

  // 代码生成路由
  menuManager->generateCode(driver, config.format);

  return 0;
}
