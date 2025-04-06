#pragma once
#include "ast/ast.h"

namespace XYZ {

// var_decl 的基类
class VarDeclNode : public ASTNode {
 public:
  VarDeclNode(size_t line) : ASTNode("VarDecl", line) {}
  ~VarDeclNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// var_decl := idlist COLON type
class VarDeclNode_IdList_Colon_Type : public VarDeclNode {
 public:
  VarDeclNode_IdList_Colon_Type(ASTNodePtr idlist, ASTNodePtr colon,
                                ASTNodePtr type, size_t line)
      : VarDeclNode(line) {
    addChild(idlist);
    addChild(colon);
    addChild(type);
  }
  ~VarDeclNode_IdList_Colon_Type() override = default;

  ASTNodePtr getIdList() const { return m_children[0]; }
  ASTNodePtr getColon() const { return m_children[1]; }
  ASTNodePtr getType() const { return m_children[2]; }
};

// var_decl := var_decl SEMICOLON idlist COLON type
class VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type : public VarDeclNode {
 public:
  VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type(ASTNodePtr varDecl,
                                                  ASTNodePtr semicolon,
                                                  ASTNodePtr idlist,
                                                  ASTNodePtr colon,
                                                  ASTNodePtr type, size_t line)
      : VarDeclNode(line) {
    addChild(varDecl);
    addChild(semicolon);
    addChild(idlist);
    addChild(colon);
    addChild(type);
  }
  ~VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type() override = default;

  ASTNodePtr getVarDecl() const { return m_children[0]; }
  ASTNodePtr getSemicolon() const { return m_children[1]; }
  ASTNodePtr getIdList() const { return m_children[2]; }
  ASTNodePtr getColon() const { return m_children[3]; }
  ASTNodePtr getType() const { return m_children[4]; }
};

}  // namespace XYZ
