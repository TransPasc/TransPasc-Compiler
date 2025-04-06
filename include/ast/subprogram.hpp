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

// subprogram := subprogram_head SEMICOLON subprogram_body
class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody
    : public SubprogramNode {
 public:
  SubprogramNode_SubprogramHead_Semicolon_SubprogramBody(
      ASTNodePtr subprogramHead, ASTNodePtr semicolon,
      ASTNodePtr subprogramBody, size_t line)
      : SubprogramNode(line) {
    addChild(subprogramHead);
    addChild(semicolon);
    addChild(subprogramBody);
  }
  ~SubprogramNode_SubprogramHead_Semicolon_SubprogramBody() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSubprogramHead() const { return m_children[0]; }
  ASTNodePtr getSemicolon() const { return m_children[1]; }
  ASTNodePtr getSubprogramBody() const { return m_children[2]; }
};

}  // namespace XYZ
