#pragma once
#include "ast/ast.h"
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
    KEYWORD, // 新增关键字类型
    CHAR_LITERAL,
    BOOL_LITERAL,
    QUOTE,         // 新增字符串类型
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

  TerminalNode(Type type, ValT val, size_t line)
      : ASTNode("Terminal", line), value(val), type(type) {}
  // 拷贝构造函数
  TerminalNode(const TerminalNode &other)
      : ASTNode(other), value(other.value), type(other.type) {}
  TerminalNode &operator=(const TerminalNode &) = default;

  ~TerminalNode() override = default;
  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  std::string getValStr() { return val2string(); }
  ValT getValue() const { return value; }
  //   泛型 get
  template <typename T> T get() const { return std::get<T>(value); }
  //   指针版 get
  template <typename T> T *getPtr() { return std::get_if<T>(&value); }

  virtual void print(std::string prefix) const override {
    std::cout << prefix;
    std::cout << type2string() << " : " << val2string() << std::endl;
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
  std::string val2string() const {
    if (std::holds_alternative<std::string>(value)) {
      return std::get<std::string>(value);
    } else if (std::holds_alternative<int>(value)) {
      return std::to_string(std::get<int>(value));
    } else if (std::holds_alternative<double>(value)) {
      std::ostringstream oss;
      // output double with maximum precision
      oss << std::setprecision(std::numeric_limits<double>::max_digits10)
          << std::get<double>(value);
      return oss.str();
    }
    throw std::runtime_error("Unknown value type");
  }
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
    default:
      return "";
    }
  }

private:
  Type type;
  ValT value;
};
} // namespace XYZ
