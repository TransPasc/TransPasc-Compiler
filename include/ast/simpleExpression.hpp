#pragma once
#include "ast/ast.h"

namespace XYZ {

// simple_expression 的基类
class SimpleExpressionNode : public ASTNode {
 public:
  SimpleExpressionNode(size_t line) : ASTNode("SimpleExpression", line) {}
  ~SimpleExpressionNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// simple_expression := term
class SimpleExpressionNode_Term : public SimpleExpressionNode {
 public:
  SimpleExpressionNode_Term(ASTNodePtr term, size_t line)
      : SimpleExpressionNode(line) {
    addChild(term);
  }
  ~SimpleExpressionNode_Term() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getTerm() const { return m_children[0]; }
};

// simple_expression := simple_expression PLUS term
class SimpleExpressionNode_SimpleExpression_Plus_Term
    : public SimpleExpressionNode {
 public:
  SimpleExpressionNode_SimpleExpression_Plus_Term(ASTNodePtr simpleExpression,
                                                  ASTNodePtr plus,
                                                  ASTNodePtr term, size_t line)
      : SimpleExpressionNode(line) {
    addChild(simpleExpression);
    addChild(plus);
    addChild(term);
  }
  ~SimpleExpressionNode_SimpleExpression_Plus_Term() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSimpleExpression() const { return m_children[0]; }
  ASTNodePtr getPlus() const { return m_children[1]; }
  ASTNodePtr getTerm() const { return m_children[2]; }
};

// simple_expression := simple_expression MINUS term
class SimpleExpressionNode_SimpleExpression_Minus_Term
    : public SimpleExpressionNode {
 public:
  SimpleExpressionNode_SimpleExpression_Minus_Term(ASTNodePtr simpleExpression,
                                                   ASTNodePtr minus,
                                                   ASTNodePtr term, size_t line)
      : SimpleExpressionNode(line) {
    addChild(simpleExpression);
    addChild(minus);
    addChild(term);
  }
  ~SimpleExpressionNode_SimpleExpression_Minus_Term() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSimpleExpression() const { return m_children[0]; }
  ASTNodePtr getMinus() const { return m_children[1]; }
  ASTNodePtr getTerm() const { return m_children[2]; }
};

// simple_expression := simple_expression OR term
class SimpleExpressionNode_SimpleExpression_Or_Term
    : public SimpleExpressionNode {
 public:
  SimpleExpressionNode_SimpleExpression_Or_Term(ASTNodePtr simpleExpression,
                                                ASTNodePtr orToken,
                                                ASTNodePtr term, size_t line)
      : SimpleExpressionNode(line) {
    addChild(simpleExpression);
    addChild(orToken);
    addChild(term);
  }
  ~SimpleExpressionNode_SimpleExpression_Or_Term() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSimpleExpression() const { return m_children[0]; }
  ASTNodePtr getOrToken() const { return m_children[1]; }
  ASTNodePtr getTerm() const { return m_children[2]; }
};

}  // namespace XYZ
