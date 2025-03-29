#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "parser.hpp"
#include "scanner.h"

namespace XYZ {

class Driver {
 public:
  Driver();

  int parse();

  void clear();

  std::string str() const;

  void switchInputStream(std::istream *is);

  friend class Parser;
  friend class Scanner;

 private:
  void increaseLocation(unsigned int loc);

  unsigned int location() const;

 private:
  Scanner m_scanner;
  Parser m_parser;
  unsigned int m_location;  // Used by scanner
};

}  // namespace XYZ

#endif  // INTERPRETER_H
