#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "ast/ast.hpp"
#include "parser.hpp"
#include "scanner.h"

namespace XYZ {

class Driver {
 public:
  Driver();

  int parse();

  // 添加打印token流的方法
  void printTokens();

  void printAST();

  // 获取token名称的辅助方法
  std::string getTokenName(XYZ::Parser::symbol_kind_type kind);

  void clear();

  std::string str() const;

  void switchInputStream(std::istream *is);

  static std::shared_ptr<ProgramStructNode> root;  // 修改为 static

  friend class Parser;
  friend class Scanner;

 private:
  void increaseLocation(unsigned int loc);
  void increaseLine();

  unsigned int location() const;

 private:
  Scanner m_scanner;
  Parser m_parser;
  unsigned int m_location;  // Used by scanner
  unsigned int m_line;      // Used by parser
};

}  // namespace XYZ

#endif  // INTERPRETER_H
