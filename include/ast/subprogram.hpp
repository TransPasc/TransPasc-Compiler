#pragma once
#include "ast/ast.h"

namespace XYZ {

// subprogram 的基类
class SubprogramNode : public ASTNode {
public:
  SubprogramNode(size_t line) : ASTNode("Subprogram", line) {}
  ~SubprogramNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// subprogram := subprogram_head SEMICOLON subprogram_body SEMI_COLON
class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON
    : public SubprogramNode {
public:
  SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON(
      ASTNodePtr subprogramHead, ASTNodePtr semicolon1,
      ASTNodePtr subprogramBody, ASTNodePtr semicolon2, size_t line)
      : SubprogramNode(line) {
    addChild(subprogramHead);
    addChild(semicolon1);
    addChild(subprogramBody);
    addChild(semicolon2);
  }
  ~SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON() override =
      default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSubprogramHead() const { return m_children[0]; }
  ASTNodePtr getSemicolon1() const { return m_children[1]; }
  ASTNodePtr getSubprogramBody() const { return m_children[2]; }
  ASTNodePtr getSemicolon2() const { return m_children[3]; }
};

} // namespace XYZ
