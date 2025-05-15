#pragma once
#include "ast/ast.h"
#include "ast/expression.hpp"
#include "symbolTable/type.hpp"
#include <memory>
#include <vector>

namespace XYZ {

// expression_list 的基类
class ExpressionListNode : public ASTNode {
public:
  ExpressionListNode(size_t line);
  ~ExpressionListNode() override = default;

  void accept(ASTVisitor &visitor) override;
  virtual std::vector<std::shared_ptr<SymbolType>> getTypeList() const;
};

// expression_list := expression
class ExpressionListNode_Expression : public ExpressionListNode {
public:
  ExpressionListNode_Expression(ASTNodePtr expression, size_t line);
  ~ExpressionListNode_Expression() override = default;

  void accept(ASTVisitor &visitor) override;

  std::shared_ptr<ExpressionNode> getExpression() const;
  virtual std::vector<std::shared_ptr<SymbolType>> getTypeList() const override;
};

// expression_list := expression_list COMMA expression
class ExpressionListNode_ExpressionList_Comma_Expression
    : public ExpressionListNode {
public:
  ExpressionListNode_ExpressionList_Comma_Expression(ASTNodePtr expressionList,
                                                     ASTNodePtr comma,
                                                     ASTNodePtr expression,
                                                     size_t line);
  ~ExpressionListNode_ExpressionList_Comma_Expression() override = default;

  void accept(ASTVisitor &visitor) override;

  std::shared_ptr<ExpressionListNode> getExpressionList() const;
  std::shared_ptr<TerminalNode> getComma() const;
  std::shared_ptr<ExpressionNode> getExpression() const;

  virtual std::vector<std::shared_ptr<SymbolType>> getTypeList() const override;
};

} // namespace XYZ
