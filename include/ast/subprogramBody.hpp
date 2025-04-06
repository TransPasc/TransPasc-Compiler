#pragma once
#include "ast/ast.h"

namespace XYZ {

// subprogram_body 的基类
class SubprogramBodyNode : public ASTNode {
 public:
  SubprogramBodyNode(size_t line) : ASTNode("SubprogramBody", line) {}
  ~SubprogramBodyNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// subprogram_body := const_decls var_decls compound_statement
class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement
    : public SubprogramBodyNode {
 public:
  SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement(
      ASTNodePtr constDecls, ASTNodePtr varDecls, ASTNodePtr compoundStatement,
      size_t line)
      : SubprogramBodyNode(line) {
    addChild(constDecls);
    addChild(varDecls);
    addChild(compoundStatement);
  }
  ~SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement() override =
      default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getConstDecls() const { return m_children[0]; }
  ASTNodePtr getVarDecls() const { return m_children[1]; }
  ASTNodePtr getCompoundStatement() const { return m_children[2]; }
};

}  // namespace XYZ
