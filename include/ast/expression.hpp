#pragma once
#include "ast/ast.h"
#include "ast/simpleExpression.hpp"
#include "ast/terminal.hpp"
#include "symbolTable/type.hpp"
namespace XYZ {

// expression 的基类
class ExpressionNode : public ASTNode {
public:
  ExpressionNode(size_t line);
  ~ExpressionNode() override;

  void accept(ASTVisitor &visitor) override;
  virtual std::shared_ptr<SymbolType> getType() const = 0;
};

// expression := simple_expression
class ExpressionNode_SimpleExpression : public ExpressionNode {
public:
  ExpressionNode_SimpleExpression(ASTNodePtr simpleExpression, size_t line);
  ~ExpressionNode_SimpleExpression() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<SimpleExpressionNode> getSimpleExpression() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// expression := simple_expression RELOP simple_expression
class ExpressionNode_SimpleExpression_Relop_SimpleExpression
    : public ExpressionNode {
public:
  ExpressionNode_SimpleExpression_Relop_SimpleExpression(
      ASTNodePtr simpleExpression1, ASTNodePtr relop,
      ASTNodePtr simpleExpression2, size_t line);
  ~ExpressionNode_SimpleExpression_Relop_SimpleExpression() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<SimpleExpressionNode> getSimpleExpression1() const;
  std::shared_ptr<TerminalNode> getRelop() const;
  std::shared_ptr<SimpleExpressionNode> getSimpleExpression2() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

} // namespace XYZ
