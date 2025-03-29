#include <iostream>

#include "driver.h"
#include "parser.hpp"
#include "scanner.h"

using namespace XYZ;
using namespace std;

int main(int argc, char **argv) {
  Driver i;
  int res = i.parse();
  cout << "Parse complete. Result = " << res << endl;
  return res;
}
