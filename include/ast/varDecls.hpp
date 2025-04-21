#pragma once
#include "ast/ast.h"

namespace XYZ {

// var_decls 的基类
// var_decls := EMPTY
class VarDeclsNode : public ASTNode {
public:
  VarDeclsNode(size_t line) : ASTNode("VarDecls", line) {}
  ~VarDeclsNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// var_decls := VAR var_decl SEMICOLON
class VarDeclsNode_Var_VarDecl_Semicolon : public VarDeclsNode {
public:
  VarDeclsNode_Var_VarDecl_Semicolon(ASTNodePtr varToken, ASTNodePtr varDecl,
                                     ASTNodePtr semicolon, size_t line)
      : VarDeclsNode(line) {
    addChild(varToken);
    addChild(varDecl);
    addChild(semicolon);
  }
  ~VarDeclsNode_Var_VarDecl_Semicolon() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getVarToken() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<VarDeclNode> getVarDecl() const {
    return dynamic_pointer_cast<VarDeclNode>(m_children[1]);
  }
  std::shared_ptr<TerminalNode> getSemicolon() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
};

} // namespace XYZ
