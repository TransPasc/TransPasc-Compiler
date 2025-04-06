#pragma once
#include "ast/ast.h"

namespace XYZ {

// type 的基类
class TypeNode : public ASTNode {
 public:
  TypeNode(size_t line) : ASTNode("Type", line) {}
  ~TypeNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// type := basic_type
class TypeNode_BasicType : public TypeNode {
 public:
  TypeNode_BasicType(ASTNodePtr basicType, size_t line) : TypeNode(line) {
    addChild(basicType);
  }
  ~TypeNode_BasicType() override = default;

  ASTNodePtr getBasicType() const { return m_children[0]; }
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

  ASTNodePtr getArrayToken() const { return m_children[0]; }
  ASTNodePtr getLbracket() const { return m_children[1]; }
  ASTNodePtr getPeriod() const { return m_children[2]; }
  ASTNodePtr getRbracket() const { return m_children[3]; }
  ASTNodePtr getOfToken() const { return m_children[4]; }
  ASTNodePtr getBasicType() const { return m_children[5]; }
};

}  // namespace XYZ
