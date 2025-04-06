#pragma once
#include "ast/ast.h"

namespace XYZ {

// else_part 的基类
// else_part := EMPTY
class ElsePartNode : public ASTNode {
 public:
  ElsePartNode(size_t line) : ASTNode("ElsePart", line) {}
  ~ElsePartNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// else_part := ELSE statement
class ElsePartNode_Else_Statement : public ElsePartNode {
 public:
  ElsePartNode_Else_Statement(ASTNodePtr elseToken, ASTNodePtr statement,
                              size_t line)
      : ElsePartNode(line) {
    addChild(elseToken);
    addChild(statement);
  }
  ~ElsePartNode_Else_Statement() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getElseToken() const { return m_children[0]; }
  ASTNodePtr getStatement() const { return m_children[1]; }
};

}  // namespace XYZ
