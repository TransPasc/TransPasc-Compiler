#include <fstream>
#include <iostream>

#include "codeGenerate/cLangGenerator.hpp"
#include "driver.h"
#include "parser.hpp"
#include "scanner.h"

using namespace XYZ;
using namespace std;
namespace MENU {
// TODO: 移动到专门的menu.cpp文件, 引入 argparse
using namespace std;
void showMenu() {}
void readFromFile(Driver &driver, string path) {
  auto inputFile = new ifstream(path);
  if (inputFile->fail()) {
    cerr << "Error opening file." << endl;
    exit(1);
  }
  driver.switchInputStream(inputFile);
  cout << "Reading from file: " << path << endl;
}
void printTokens(Driver &driver) { driver.printTokens(); }
void parse(Driver &driver) {
  int res = driver.parse();
  if (res != 0) {
    cout << "Parse failed. Error code: " << res << endl;
    return;
  }
  driver.printAST();
}
void semanticAnalysis(Driver &driver) {
  driver.analyze();
  cout << "Semantic analysis completed." << endl;
}
void generateCLangCode(Driver &driver) {
  auto generator = make_shared<CLangGenerator>();
  driver.generateCode(generator);
  cout << "C language code generation completed." << endl;
}
} // namespace MENU
int main(int argc, char **argv) {
  Driver driver;
  if (argc > 1) {
    string path = argv[1];
    MENU::readFromFile(driver, path);
  } else {
    cout << "No input file provided. Reading from standard input." << endl;
    driver.switchInputStream(&cin);
  }

  //   MENU::printTokens(driver);
  MENU::parse(driver);
  //   MENU::semanticAnalysis(driver);
  MENU::generateCLangCode(driver);

  return 0;
}
