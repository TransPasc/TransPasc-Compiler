#pragma once
#include "ast/ast.h"

namespace XYZ {

// const_decl 的基类
class ConstDeclNode : public ASTNode {
 public:
  ConstDeclNode(size_t line) : ASTNode("ConstDeclNode", line) {}
  ~ConstDeclNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// const_decl := ID ASSIGNOP const_val
class ConstDeclNode_Id_Assignop_ConstVal : public ConstDeclNode {
 public:
  ConstDeclNode_Id_Assignop_ConstVal(ASTNodePtr id, ASTNodePtr assignop,
                                     ASTNodePtr constVal, size_t line)
      : ConstDeclNode(line) {
    addChild(id);
    addChild(assignop);
    addChild(constVal);
  }
  ~ConstDeclNode_Id_Assignop_ConstVal() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getId() const { return m_children[0]; }
  ASTNodePtr getAssignop() const { return m_children[1]; }
  ASTNodePtr getConstVal() const { return m_children[2]; }
};

// const_decl := const_decl SEMICOLON ID ASSIGNOP const_val
class ConstDeclNode_ConstDecl_Semicolon_Id_Assignop_ConstVal
    : public ConstDeclNode {
 public:
  ConstDeclNode_ConstDecl_Semicolon_Id_Assignop_ConstVal(
      ASTNodePtr constDecl, ASTNodePtr semicolon, ASTNodePtr id,
      ASTNodePtr assignop, ASTNodePtr constVal, size_t line)
      : ConstDeclNode(line) {
    addChild(constDecl);
    addChild(semicolon);
    addChild(id);
    addChild(assignop);
    addChild(constVal);
  }
  ~ConstDeclNode_ConstDecl_Semicolon_Id_Assignop_ConstVal() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getConstDecl() const { return m_children[0]; }
  ASTNodePtr getSemicolon() const { return m_children[1]; }
  ASTNodePtr getId() const { return m_children[2]; }
  ASTNodePtr getAssignop() const { return m_children[3]; }
  ASTNodePtr getConstVal() const { return m_children[4]; }
};

}  // namespace XYZ
