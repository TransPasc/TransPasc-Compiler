#include "driver.h"
#include <format>
#include <iostream>
#include <string>
#include <string_view>

using namespace XYZ;
namespace XYZ {

// 定义 static root
std::shared_ptr<ProgramStructNode> Driver::root = nullptr;

} // namespace XYZ

Driver::Driver()
    : m_scanner(*this), m_parser(m_scanner, *this), m_location(0) {}

int Driver::parse() {
  cout << "Parsing..." << endl;
  return m_parser.parse();
}

// 添加打印token流的方法
void Driver::printTokens() {
  while (true) {
    auto token = m_scanner.get_next_token();
    if (token.kind() == XYZ::Parser::symbol_kind_type::S_YYEOF) {
      break;
    }
    auto lineno = token.location.begin.line;
    auto fmt_str = format("[{}] Token: {}", lineno, getTokenName(token.kind()));
    cout << fmt_str << endl;
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
  case XYZ::Parser::symbol_kind_type::S_CHAR_LITERAL:
    return "Char Literal";
  case XYZ::Parser::symbol_kind_type::S_RELOP:
    return "Relational Operator";
  case XYZ::Parser::symbol_kind_type::S_PLUS:
    return "Plus";
  case XYZ::Parser::symbol_kind_type::S_MINUS:
    return "Minus";
  case XYZ::Parser::symbol_kind_type::S_OR:
    return "Or";
  case XYZ::Parser::symbol_kind_type::S_MULOP:
    return "Multiplication Operator";
  case XYZ::Parser::symbol_kind_type::S_ASSIGNOP:
    return "Assignment Operator";
  case XYZ::Parser::symbol_kind_type::S_COMMA:
    return "Comma";
  case XYZ::Parser::symbol_kind_type::S_SEMICOLON:
    return "Semicolon";
  case XYZ::Parser::symbol_kind_type::S_COLON:
    return "Colon";
  case XYZ::Parser::symbol_kind_type::S_DOT:
    return "Dot";
  case XYZ::Parser::symbol_kind_type::S_QUOTE:
    return "Quote";
  case XYZ::Parser::symbol_kind_type::S_TO:
    return "To";
  case XYZ::Parser::symbol_kind_type::S_READ:
    return "Read";
  case XYZ::Parser::symbol_kind_type::S_WRITE:
    return "Write";
  case XYZ::Parser::symbol_kind_type::S_NOT:
    return "Not";
  case XYZ::Parser::symbol_kind_type::S_LPAREN:
    return "Left Parenthesis";
  case XYZ::Parser::symbol_kind_type::S_RPAREN:
    return "Right Parenthesis";
  case XYZ::Parser::symbol_kind_type::S_LBRACKET:
    return "Left Bracket";
  case XYZ::Parser::symbol_kind_type::S_RBRACKET:
    return "Right Bracket";
  case XYZ::Parser::symbol_kind_type::S_PROGRAM:
    return "Program";
  case XYZ::Parser::symbol_kind_type::S_VAR:
    return "Var";
  case XYZ::Parser::symbol_kind_type::S_BEGIN:
    return "Begin";
  case XYZ::Parser::symbol_kind_type::S_END:
    return "End";
  case XYZ::Parser::symbol_kind_type::S_IF:
    return "If";
  case XYZ::Parser::symbol_kind_type::S_THEN:
    return "Then";
  case XYZ::Parser::symbol_kind_type::S_ELSE:
    return "Else";
  case XYZ::Parser::symbol_kind_type::S_WHILE:
    return "While";
  case XYZ::Parser::symbol_kind_type::S_DO:
    return "Do";
  case XYZ::Parser::symbol_kind_type::S_CONST:
    return "Const";
  case XYZ::Parser::symbol_kind_type::S_ARRAY:
    return "Array";
  case XYZ::Parser::symbol_kind_type::S_OF:
    return "Of";
  case XYZ::Parser::symbol_kind_type::S_PROCEDURE:
    return "Procedure";
  case XYZ::Parser::symbol_kind_type::S_FUNCTION:
    return "Function";
  case XYZ::Parser::symbol_kind_type::S_FOR:
    return "For";
  case XYZ::Parser::symbol_kind_type::S_RECORD:
    return "Record";
  case XYZ::Parser::symbol_kind_type::S_TYPE:
    return "Type";
  case XYZ::Parser::symbol_kind_type::S_LABEL:
    return "Label";
  case XYZ::Parser::symbol_kind_type::S_CASE:
    return "Case";
  case XYZ::Parser::symbol_kind_type::S_GOTO:
    return "Goto";
  case XYZ::Parser::symbol_kind_type::S_CHAR:
    return "Char";
  case XYZ::Parser::symbol_kind_type::S_BOOLEAN:
    return "Boolean";
  case XYZ::Parser::symbol_kind_type::S_STRING:
    return "String";
  case XYZ::Parser::symbol_kind_type::S_INTEGER:
    return "Integer";
  case XYZ::Parser::symbol_kind_type::S_REAL:
    return "Real";
  case XYZ::Parser::symbol_kind_type::S_UMINUS:
    return "Unary Minus";
  default:
    return "Unknown Token";
  }
}

void Driver::clear() { m_location = location(); }

void Driver::switchInputStream(std::istream *is) {
  m_scanner.switch_streams(is, NULL);
}

void Driver::increaseLocation(unsigned int leng) {
  m_location.columns(leng);
  //   std::cout << "increaseLocation(): " << leng << ", total = " << m_location
  //             << std::endl;
}
void Driver::increaseLine() { m_location.lines(1); }

void Driver::step() { m_location.step(); }

const location &Driver::getLocation() const { return m_location; }
int Driver::getLine() const { return m_location.begin.line; }
void Driver::printAST() {
  if (root) {
    root->print("");
  } else {
    std::cout << "AST is empty." << std::endl;
  }

  std::cout << "End of AST." << std::endl;
}

void Driver::handleError(const std::string &msg, const location &loc) {
  // TODO: 改用日志库
  auto formatted_msg = std::format("Error[{}]: {}", loc.begin.line, msg);
  std::cerr << formatted_msg << std::endl;
}
void Driver::analyze() {
  if (!root)
    parse();
  if (root) {
    m_analyzer.analyze(root);
  } else {
    std::cerr << "AST is empty, cannot analyze." << std::endl;
  }
}
