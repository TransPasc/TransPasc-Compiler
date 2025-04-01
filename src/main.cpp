#include <fstream>
#include <iostream>

#include "driver.h"
#include "parser.hpp"
#include "scanner.h"

using namespace XYZ;
using namespace std;
namespace MENU {
using namespace std;
void showMenu() {}
void readFromFile(Driver& driver, string path) {
  auto inputFile = new ifstream(path);
  if (inputFile->fail()) {
    cerr << "Error opening file." << endl;
    exit(1);
  }
  driver.switchInputStream(inputFile);
  cout << "Reading from file: " << path << endl;
}
void printTokens(Driver& driver) { driver.printTokens(); }
void parse(Driver& driver) {
  int res = driver.parse();
  cout << "Parse complete. Result = " << res << endl;
}
}  // namespace MENU
int main(int argc, char** argv) {
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

  return 0;
}
