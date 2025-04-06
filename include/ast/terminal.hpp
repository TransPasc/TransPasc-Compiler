#pragma once
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
    PERIOD
  };
  TerminalNode(Type type, std::string val, size_t line)
      : ASTNode("TerminalNode", line), value() {
    switch (type) {
      case Type::ID:
        value = val;
        break;
      case Type::NUMBER:
        value = std::stoi(val);
        break;
      case Type::COMMA:
        value = val;
        break;
      case Type::DOT:
        value = val;
        break;
      case Type::LBRACKET:
        value = val;
        break;
      case Type::RBRACKET:
        value = val;
        break;
      case Type::LPAREN:
        value = val;
        break;
      case Type::RELOP:
        value = val;
        break;
      case Type::ADDOP:
        value = val;
        break;
      case Type::MULOP:
        value = val;
        break;
      case Type::SEMICOLON:
        value = val;
        break;
      case Type::COLON:
        value = val;
        break;
      case Type::PERIOD:
        value = val;
        break;
    };
  }
  ~TerminalNode() override = default;
  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  ValT getValue() const { return value; }
};
}  // namespace XYZ