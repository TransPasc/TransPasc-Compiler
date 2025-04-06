#pragma once
#include "ast/ast.h"

namespace XYZ {

// term 的基类
class TermNode : public ASTNode {
 public:
  TermNode(size_t line) : ASTNode("Term", line) {}
  ~TermNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// term := factor
class TermNode_Factor : public TermNode {
 public:
  TermNode_Factor(ASTNodePtr factor, size_t line) : TermNode(line) {
    addChild(factor);
  }
  ~TermNode_Factor() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getFactor() const { return m_children[0]; }
};

// term := term MULOP factor
class TermNode_Term_Mulop_Factor : public TermNode {
 public:
  TermNode_Term_Mulop_Factor(ASTNodePtr term, ASTNodePtr mulop,
                             ASTNodePtr factor, size_t line)
      : TermNode(line) {
    addChild(term);
    addChild(mulop);
    addChild(factor);
  }
  ~TermNode_Term_Mulop_Factor() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getTerm() const { return m_children[0]; }
  ASTNodePtr getMulop() const { return m_children[1]; }
  ASTNodePtr getFactor() const { return m_children[2]; }
};

}  // namespace XYZ
