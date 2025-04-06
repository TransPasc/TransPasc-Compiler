#pragma once
#include "ast/ast.h"

namespace XYZ {

// const_decls 的基类
// const_decls := EMPTY
class ConstDeclsNode : public ASTNode {
 public:
  ConstDeclsNode(size_t line) : ASTNode("ConstDeclsNode", line) {}
  ~ConstDeclsNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// const_decls := CONST const_decl
class ConstDeclsNode_Const_ConstDecl : public ConstDeclsNode {
 public:
  ConstDeclsNode_Const_ConstDecl(ASTNodePtr constToken, ASTNodePtr constDecl,
                                 size_t line)
      : ConstDeclsNode(line) {
    addChild(constToken);
    addChild(constDecl);
  }
  ~ConstDeclsNode_Const_ConstDecl() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getConstToken() const { return m_children[0]; }
  ASTNodePtr getConstDecl() const { return m_children[1]; }
};

}  // namespace XYZ
