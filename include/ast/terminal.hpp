#pragma once
#include "ast/ast.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <variant>

namespace XYZ {
// 终结符节点
class TerminalNode : public ASTNode {

public:
  using ValT = std::variant<std::string, int, double>;
  enum class Type {
    ID,
    NUMBER,
    COMMA,
    DOT,
    LBRACKET,
    RBRACKET,
    LPAREN,
    RPAREN,
    ASSIGNOP,
    RELOP,
    ADDOP,
    MULOP,
    SEMICOLON,
    COLON,
    PERIOD,
    KEYWORD,      // 新增关键字类型
    CHAR_LITERAL, // 字符字面量
    BOOL_LITERAL,
    QUOTE,
    STRING_LITERAL // 新增字符串类型
  };
  /**
   * @brief 将字符串转换为数字
   * 可能是 int 或 double
   * @param val
   * @return ValT
   */
  static ValT makeNum(const std::string &val) {
    // TODO: 未来可能会有更复杂的数字类型
    auto is_double = [](const std::string &str) {
      return str.find('.') != std::string::npos;
    };
    if (is_double(val)) {
      return std::stod(val);
    } else {
      return std::stoi(val);
    }
  }

  TerminalNode(Type type, std::string val, size_t line)
      : ASTNode("Terminal", line), __row_value_(val), type(type) {
    if (type == Type::NUMBER) {
      value = makeNum(__row_value_);
    } else {
      value = __row_value_;
    }
  }
  // 拷贝构造函数
  TerminalNode(const TerminalNode &other)
      : ASTNode(other), __row_value_(other.__row_value_), type(other.type) {}
  TerminalNode &operator=(const TerminalNode &) = default;

  ~TerminalNode() override = default;
  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  std::string getValStr() { return __row_value_; }
  // std::string getValue() const { return __row_value_; }
  //   泛型 get
  template <typename T> T get() const { return std::get<T>(value); }
  //   指针版 get
  template <typename T> T *getPtr() { return std::get_if<T>(&value); }

  virtual void print(std::string prefix) const override {
    std::cout << prefix;
    std::cout << type2string() << " : " << __row_value_ << std::endl;
  }

  //   断言是不是某个字符串
  void expect_str(const std::string &str) const {
    if (!std::holds_alternative<std::string>(value)) {
      throw std::runtime_error("expect " + str + ", but got " + type2string());
    }
    auto res = std::get<std::string>(value);
    if (res != str) {
      throw std::runtime_error("expect " + str + ", but got " + res);
    }
  }
  bool isRelOp() const { return type == Type::RELOP; }
  bool isMulOp() const { return type == Type::MULOP; }

protected:
  std::string type2string() const {
    switch (type) {
    case Type::ID:
      return "ID";
    case Type::NUMBER:
      return "NUMBER";
    case Type::COMMA:
      return "COMMA";
    case Type::DOT:
      return "DOT";
    case Type::LBRACKET:
      return "LBRACKET";
    case Type::RBRACKET:
      return "RBRACKET";
    case Type::LPAREN:
      return "LPAREN";
    case Type::RELOP:
      return "RELOP";
    case Type::ADDOP:
      return "ADDOP";
    case Type::RPAREN:
      return "RPAREN";
    case Type::ASSIGNOP:
      return "ASSIGNOP";
    case Type::MULOP:
      return "MULOP";
    case Type::SEMICOLON:
      return "SEMICOLON";
    case Type::COLON:
      return "COLON";
    case Type::PERIOD:
      return "PERIOD";
    case Type::KEYWORD:
      return "KEYWORD";
    case Type::CHAR_LITERAL:
      return "CHAR_LITERAL";
    case Type::BOOL_LITERAL:
      return "BOOL_LITERAL";
    case Type::QUOTE:
      return "QUOTE";
    case Type::STRING_LITERAL:
      return "STRING_LITERAL";
    default:
      return "";
    }
  }

private:
  Type type;
  std::string __row_value_;
  ValT value;
};
} // namespace XYZ
