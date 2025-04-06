#pragma once
#include "ast/ast.h"

namespace XYZ {

// expression 的基类
class ExpressionNode : public ASTNode {
 public:
  ExpressionNode(size_t line) : ASTNode("Expression", line) {}
  ~ExpressionNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// expression := simple_expression
class ExpressionNode_SimpleExpression : public ExpressionNode {
 public:
  ExpressionNode_SimpleExpression(ASTNodePtr simpleExpression, size_t line)
      : ExpressionNode(line) {
    addChild(simpleExpression);
  }
  ~ExpressionNode_SimpleExpression() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSimpleExpression() const { return m_children[0]; }
};

// expression := simple_expression RELOP simple_expression
class ExpressionNode_SimpleExpression_Relop_SimpleExpression
    : public ExpressionNode {
 public:
  ExpressionNode_SimpleExpression_Relop_SimpleExpression(
      ASTNodePtr simpleExpression1, ASTNodePtr relop,
      ASTNodePtr simpleExpression2, size_t line)
      : ExpressionNode(line) {
    addChild(simpleExpression1);
    addChild(relop);
    addChild(simpleExpression2);
  }
  ~ExpressionNode_SimpleExpression_Relop_SimpleExpression() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSimpleExpression1() const { return m_children[0]; }
  ASTNodePtr getRelop() const { return m_children[1]; }
  ASTNodePtr getSimpleExpression2() const { return m_children[2]; }
};

}  // namespace XYZ
