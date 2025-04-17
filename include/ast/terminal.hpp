#pragma once
#include <iostream>
#include <string>
#include <variant>

#include "ast/ast.h"

namespace XYZ {
// 终结符节点
class TerminalNode : public ASTNode {
  using ValT = std::variant<std::string, int, float>;
  ValT value;

public:
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
    QUOTE,         // 新增字符串类型
    STRING_LITERAL // 新增字符串类型
  };
  TerminalNode(Type type, std::string val, size_t line)
      : ASTNode("Terminal", line), value(val), type(type) {}
  // 拷贝构造函数
  TerminalNode(const TerminalNode &other)
      : ASTNode(other), value(other.value), type(other.type) {}
  TerminalNode &operator=(const TerminalNode &) = default;

  ~TerminalNode() override = default;
  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  ValT getValue() const { return value; }

  virtual void print(size_t indent) const override {
    printIndent(indent);
    std::cout << type2string() << " : " << val2string() << std::endl;
    for (const auto &child : m_children) {
      child->print(indent + 2);
    }
  }

protected:
  std::string val2string() const {
    if (std::holds_alternative<std::string>(value)) {
      return std::get<std::string>(value);
    } else if (std::holds_alternative<int>(value)) {
      return std::to_string(std::get<int>(value));
    } else if (std::holds_alternative<float>(value)) {
      return std::to_string(std::get<float>(value));
    }
    return "";
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
};
} // namespace XYZ
