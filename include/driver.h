#pragma once

#include <vector>

#include "ast/ast.hpp"
#include "codeGenerate/generator.hpp"
#include "err.hpp"
#include "location.hh"
#include "parser.hpp"
#include "scanner.h"
#include "semanticAnalysis/analyzer.hpp"

namespace XYZ {

class Driver {
  std::string m_outputFileName;

public:
  Driver();
  // 语法分析
  int parse();
  //   语义分析
  void analyze();
  // 代码生成
  void generateCode(std::shared_ptr<Generator> generator);

  // 添加打印token流的方法
  void printTokens();

  void printAST();

  // 获取token名称的辅助方法
  std::string getTokenName(XYZ::Parser::symbol_kind_type kind);

  void clear();

  std::string str() const;

  void switchInputStream(std::istream *is);

  void handleError(const std::string &msg, const location &loc);

  void setOutputFileName(const std::string &filename);
  // TODO: Implement the set_verbose method
  void set_verbose(bool verbose) {};

  static std::shared_ptr<ProgramStructNode> root;

  friend class Parser;
  friend class Scanner;

private:
  void increaseLocation(unsigned int leng);
  void increaseLine();
  void step();

  const location &getLocation() const;
  int getLine() const;

private:
  Scanner m_scanner;
  Parser m_parser;
  Analyzer m_analyzer;
  // 声明位置实例
  location m_location;
};

} // namespace XYZ
