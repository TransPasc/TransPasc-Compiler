#pragma once
#include "ast/ast.h"

namespace XYZ {
// id_varpart 的基类
class IdVarPartNode : public ASTNode {
 public:
  IdVarPartNode(size_t line) : ASTNode("IdVarPart", line) {}
  ~IdVarPartNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};
// id_varpart := LBRACKET expression RBRACKET
class IdVarPartNode_Lbracket_Expression_Rbracket : public IdVarPartNode {
 public:
  IdVarPartNode_Lbracket_Expression_Rbracket(ASTNodePtr lbracket,
                                             ASTNodePtr expression,
                                             ASTNodePtr rbracket, size_t line)
      : IdVarPartNode(line) {
    addChild(lbracket);
    addChild(expression);
    addChild(rbracket);
  }
  ~IdVarPartNode_Lbracket_Expression_Rbracket() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getLbracket() const { return m_children[0]; }
  ASTNodePtr getExpression() const { return m_children[1]; }
  ASTNodePtr getRbracket() const { return m_children[2]; }
};
}  // namespace XYZ