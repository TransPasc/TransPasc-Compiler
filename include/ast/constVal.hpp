#pragma once
#include "ast/ast.h"
#include "symbolTable/type.hpp"

namespace XYZ {

// const_val 的基类
class ConstValNode : public ASTNode {
public:
  ConstValNode(size_t line) : ASTNode("ConstVal", line) {}
  ~ConstValNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  //   get type
  virtual std::shared_ptr<SymbolType> getType() const = 0;
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

  shared_ptr<TerminalNode> getPlus() const {
    return static_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getNumber() const {
    return static_pointer_cast<TerminalNode>(m_children[1]);
  }
  virtual std::shared_ptr<SymbolType> getType() const override {
    return std::make_shared<SymbolType>(
        SymbolType::MakeBasic(SymbolType::BasicType::INTEGER));
  }
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

  shared_ptr<TerminalNode> getMinus() const {
    return static_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getNumber() const {
    return static_pointer_cast<TerminalNode>(m_children[1]);
  }
  virtual std::shared_ptr<SymbolType> getType() const override {
    return std::make_shared<SymbolType>(
        SymbolType::MakeBasic(SymbolType::BasicType::INTEGER));
  }
};

// const_val := NUMBER
class ConstValNode_Number : public ConstValNode {
public:
  ConstValNode_Number(ASTNodePtr number, size_t line) : ConstValNode(line) {
    addChild(number);
  }
  ~ConstValNode_Number() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getNumber() const {
    return static_pointer_cast<TerminalNode>(m_children[0]);
  }
  virtual std::shared_ptr<SymbolType> getType() const override {
    return std::make_shared<SymbolType>(
        SymbolType::MakeBasic(SymbolType::BasicType::INTEGER));
  }
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

  shared_ptr<TerminalNode> getCharLiteral() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  virtual std::shared_ptr<SymbolType> getType() const override {
    return std::make_shared<SymbolType>(
        SymbolType::MakeBasic(SymbolType::BasicType::CHAR));
  }
};

} // namespace XYZ
