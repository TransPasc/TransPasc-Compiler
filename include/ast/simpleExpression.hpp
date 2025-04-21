#pragma once
#include "ast/ast.h"
#include "ast/term.hpp"
#include "ast/terminal.hpp"
#include "symbolTable/type.hpp"

namespace XYZ {
class TermNode;

// simple_expression 的基类
class SimpleExpressionNode : public ASTNode {
public:
  SimpleExpressionNode(size_t line);
  ~SimpleExpressionNode() override;

  void accept(ASTVisitor &visitor) override;
  virtual std::shared_ptr<SymbolType> getType() const = 0;
};

// simple_expression := term
class SimpleExpressionNode_Term : public SimpleExpressionNode {
public:
  SimpleExpressionNode_Term(ASTNodePtr term, size_t line);
  ~SimpleExpressionNode_Term() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<TermNode> getTerm() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// simple_expression := simple_expression PLUS term
class SimpleExpressionNode_SimpleExpression_Plus_Term
    : public SimpleExpressionNode {
public:
  SimpleExpressionNode_SimpleExpression_Plus_Term(ASTNodePtr simpleExpression,
                                                  ASTNodePtr plus,
                                                  ASTNodePtr term, size_t line);
  ~SimpleExpressionNode_SimpleExpression_Plus_Term() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<SimpleExpressionNode> getSimpleExpression() const;
  std::shared_ptr<TerminalNode> getPlus() const;
  std::shared_ptr<TermNode> getTerm() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// simple_expression := simple_expression MINUS term
class SimpleExpressionNode_SimpleExpression_Minus_Term
    : public SimpleExpressionNode {
public:
  SimpleExpressionNode_SimpleExpression_Minus_Term(ASTNodePtr simpleExpression,
                                                   ASTNodePtr minus,
                                                   ASTNodePtr term,
                                                   size_t line);
  ~SimpleExpressionNode_SimpleExpression_Minus_Term() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<SimpleExpressionNode> getSimpleExpression() const;
  std::shared_ptr<TerminalNode> getMinus() const;
  std::shared_ptr<TermNode> getTerm() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// simple_expression := simple_expression OR term
class SimpleExpressionNode_SimpleExpression_Or_Term
    : public SimpleExpressionNode {
public:
  SimpleExpressionNode_SimpleExpression_Or_Term(ASTNodePtr simpleExpression,
                                                ASTNodePtr orToken,
                                                ASTNodePtr term, size_t line);
  ~SimpleExpressionNode_SimpleExpression_Or_Term() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<SimpleExpressionNode> getSimpleExpression() const;
  std::shared_ptr<TerminalNode> getOrToken() const;
  std::shared_ptr<TermNode> getTerm() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

} // namespace XYZ
