#pragma once
#include "ast/ast.h"

namespace XYZ {

// type 的基类
class TypeNode : public ASTNode {
  shared_ptr<SymbolType> m_type = make_shared<SymbolType>();

public:
  TypeNode(size_t line) : ASTNode("Type", line) {}
  ~TypeNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  void setType(shared_ptr<SymbolType> type) { m_type = type; }
  const shared_ptr<SymbolType> &getType() const { return m_type; }
};

// type := basic_type
class TypeNode_BasicType : public TypeNode {
public:
  TypeNode_BasicType(ASTNodePtr basicType, size_t line) : TypeNode(line) {
    addChild(basicType);
  }
  ~TypeNode_BasicType() override = default;
  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<BasicTypeNode> getBasicType() const {
    return dynamic_pointer_cast<BasicTypeNode>(m_children[0]);
  }
};

// type := ARRAY LBRACKET period RBRACKET OF basic_type
class TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType : public TypeNode {
public:
  TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType(
      ASTNodePtr arrayToken, ASTNodePtr lbracket, ASTNodePtr period,
      ASTNodePtr rbracket, ASTNodePtr ofToken, ASTNodePtr basicType,
      size_t line)
      : TypeNode(line) {
    addChild(arrayToken);
    addChild(lbracket);
    addChild(period);
    addChild(rbracket);
    addChild(ofToken);
    addChild(basicType);
  }
  ~TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType() override = default;
  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getArrayToken() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getLbracket() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  shared_ptr<PeriodNode> getPeriod() const {
    return dynamic_pointer_cast<PeriodNode>(m_children[2]);
  }
  shared_ptr<TerminalNode> getRbracket() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[3]);
  }
  shared_ptr<TerminalNode> getOfToken() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[4]);
  }
  shared_ptr<BasicTypeNode> getBasicType() const {
    return dynamic_pointer_cast<BasicTypeNode>(m_children[5]);
  }
};

} // namespace XYZ
