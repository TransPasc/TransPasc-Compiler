#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "ast/ast.hpp"
#include "err.h"
#include "parser.hpp"
#include "scanner.h"
#include "semanticAnalysis/analyzer.hpp"

namespace XYZ {

class Driver {
public:
  Driver();

  int parse();
  void analyze();

  // 添加打印token流的方法
  void printTokens();

  void printAST();

  // 获取token名称的辅助方法
  std::string getTokenName(XYZ::Parser::symbol_kind_type kind);

  void clear();

  std::string str() const;

  void switchInputStream(std::istream *is);

  void handleError(const std::string &msg);

  static std::shared_ptr<ProgramStructNode> root;

  friend class Parser;
  friend class Scanner;

private:
  void increaseLocation(unsigned int loc);
  void increaseLine();

  unsigned int location() const;

private:
  Scanner m_scanner;
  Parser m_parser;
  Analyzer m_analyzer;
  unsigned int m_location; // Used by scanner
  unsigned int m_line;     // Used by parser
};

} // namespace XYZ

#endif // INTERPRETER_H
