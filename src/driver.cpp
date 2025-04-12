#include "driver.h"

#include <sstream>

using namespace XYZ;
namespace XYZ {

// 定义 static root
std::shared_ptr<ProgramStructNode> Driver::root = nullptr;

} // namespace XYZ

Driver::Driver()
    : m_scanner(*this), m_parser(m_scanner, *this), m_location(0), m_line(0) {}

int Driver::parse() { return m_parser.parse(); }

// 添加打印token流的方法
void Driver::printTokens() {
  while (true) {
    auto token = m_scanner.get_next_token();
    if (token.kind() == XYZ::Parser::symbol_kind_type::S_YYEOF) {
      break;
    }
    // std::cout << "Token: " << getTokenName(token.kind()) << endl;
  }
}

// 获取token名称的辅助方法
std::string Driver::getTokenName(XYZ::Parser::symbol_kind_type kind) {
  switch (kind) {
  case XYZ::Parser::symbol_kind_type::S_YYEOF:
    return "EOF";
  case XYZ::Parser::symbol_kind_type::S_ID:
    return "Id";
  case XYZ::Parser::symbol_kind_type::S_NUMBER:
    return "Number";
  // 添加其他token类型的名称
  default:
    return "Unknown Token";
  }
}

void Driver::clear() { m_location = 0; }

std::string Driver::str() const {
  std::stringstream s;
  s << "Driver: ";
  return s.str();
}

void Driver::switchInputStream(std::istream *is) {
  m_scanner.switch_streams(is, NULL);
}

void Driver::increaseLocation(unsigned int loc) {
  m_location += loc;
  //   cout << "increaseLocation(): " << loc << ", total = " << m_location <<
  //   endl;
}
void Driver::increaseLine() {
  m_line++;
  //   cout << "Line : " << m_line << endl;
}

unsigned int Driver::location() const { return m_location; }

void Driver::printAST() {
  if (root) {
    root->print(0);
  } else {
    std::cout << "AST is empty." << std::endl;
  }

  std::cout << "End of AST." << std::endl;
}
