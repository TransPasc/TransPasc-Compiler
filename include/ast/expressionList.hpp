#pragma once
#include "ast/ast.h"

namespace XYZ {

// expression_list 的基类
class ExpressionListNode : public ASTNode {
 public:
  ExpressionListNode(size_t line) : ASTNode("ExpressionList", line) {}
  ~ExpressionListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// expression_list := expression
class ExpressionListNode_Expression : public ExpressionListNode {
 public:
  ExpressionListNode_Expression(ASTNodePtr expression, size_t line)
      : ExpressionListNode(line) {
    addChild(expression);
  }
  ~ExpressionListNode_Expression() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getExpression() const { return m_children[0]; }
};

// expression_list := expression_list COMMA expression
class ExpressionListNode_ExpressionList_Comma_Expression
    : public ExpressionListNode {
 public:
  ExpressionListNode_ExpressionList_Comma_Expression(ASTNodePtr expressionList,
                                                     ASTNodePtr comma,
                                                     ASTNodePtr expression,
                                                     size_t line)
      : ExpressionListNode(line) {
    addChild(expressionList);
    addChild(comma);
    addChild(expression);
  }
  ~ExpressionListNode_ExpressionList_Comma_Expression() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getExpressionList() const { return m_children[0]; }
  ASTNodePtr getComma() const { return m_children[1]; }
  ASTNodePtr getExpression() const { return m_children[2]; }
};

}  // namespace XYZ
