#pragma once
#include "ast/ast.h"

namespace XYZ {

// const_val 的基类
class ConstValNode : public ASTNode {
 public:
  ConstValNode(size_t line) : ASTNode("ConstVal", line) {}
  ~ConstValNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// const_val := PLUS NUMBER
class ConstValNode_Plus_Number : public ConstValNode {
 public:
  ConstValNode_Plus_Number(ASTNodePtr plus, ASTNodePtr number, size_t line)
      : ConstValNode(line) {
    addChild(plus);
    addChild(number);
  }
  ~ConstValNode_Plus_Number() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getPlus() const { return m_children[0]; }
  ASTNodePtr getNumber() const { return m_children[1]; }
};

// const_val := MINUS NUMBER
class ConstValNode_Minus_Number : public ConstValNode {
 public:
  ConstValNode_Minus_Number(ASTNodePtr minus, ASTNodePtr number, size_t line)
      : ConstValNode(line) {
    addChild(minus);
    addChild(number);
  }
  ~ConstValNode_Minus_Number() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getMinus() const { return m_children[0]; }
  ASTNodePtr getNumber() const { return m_children[1]; }
};

// const_val := NUMBER
class ConstValNode_Number : public ConstValNode {
 public:
  ConstValNode_Number(ASTNodePtr number, size_t line) : ConstValNode(line) {
    addChild(number);
  }
  ~ConstValNode_Number() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getNumber() const { return m_children[0]; }
};

// const_val := CHAR_LITERAL
class ConstValNode_CharLiteral : public ConstValNode {
 public:
  ConstValNode_CharLiteral(ASTNodePtr charLiteral, size_t line)
      : ConstValNode(line) {
    addChild(charLiteral);
  }
  ~ConstValNode_CharLiteral() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getCharLiteral() const { return m_children[0]; }
};

}  // namespace XYZ
