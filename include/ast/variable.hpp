#pragma once
#include "ast/ast.h"

namespace XYZ {

// variable 的基类
class VariableNode : public ASTNode {
 public:
  VariableNode(size_t line) : ASTNode("Variable", line) {}
  ~VariableNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// variable := ID id_varpart
class VariableNode_Id_IdVarpart : public VariableNode {
 public:
  VariableNode_Id_IdVarpart(ASTNodePtr id, ASTNodePtr idVarpart, size_t line)
      : VariableNode(line) {
    addChild(id);
    addChild(idVarpart);
  }
  ~VariableNode_Id_IdVarpart() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getId() const { return m_children[0]; }
  ASTNodePtr getIdVarpart() const { return m_children[1]; }
};

}  // namespace XYZ
