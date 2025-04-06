#pragma once
#include "ast/ast.h"

namespace XYZ {

// program_body 的基类
class ProgramBodyNode : public ASTNode {
 public:
  ProgramBodyNode(size_t line) : ASTNode("ProgramBodyNode", line) {}
  ~ProgramBodyNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// program_body := const_decls var_decls subprogram_decls compound_statement
class ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
    : public ProgramBodyNode {
 public:
  ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement(
      ASTNodePtr constDecls, ASTNodePtr varDecls, ASTNodePtr subprogramDecls,
      ASTNodePtr compoundStatement, size_t line)
      : ProgramBodyNode(line) {
    addChild(constDecls);
    addChild(varDecls);
    addChild(subprogramDecls);
    addChild(compoundStatement);
  }
  ~ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement()
      override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getConstDecls() const { return m_children[0]; }
  ASTNodePtr getVarDecls() const { return m_children[1]; }
  ASTNodePtr getSubprogramDecls() const { return m_children[2]; }
  ASTNodePtr getCompoundStatement() const { return m_children[3]; }
};

}  // namespace XYZ
