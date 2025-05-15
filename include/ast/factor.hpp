#pragma once
#include "ast/ast.h"
#include "ast/exception.hpp"
#include "ast/expression.hpp"
#include "ast/expressionList.hpp"
#include "ast/terminal.hpp"
#include "ast/variable.hpp"

namespace XYZ {
class ExpressionNode;

// factor 的基类
class FactorNode : public ASTNode {
public:
  FactorNode(size_t line);
  ~FactorNode() override;

  void accept(ASTVisitor &visitor) override;
  virtual std::shared_ptr<SymbolType> getType() const = 0;
};

// factor := NUMBER
class FactorNode_Number : public FactorNode {
public:
  FactorNode_Number(ASTNodePtr number, size_t line);
  ~FactorNode_Number() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<TerminalNode> getNumber() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// factor := CHAR_LITERAL
class FactorNode_CharLiteral : public FactorNode {
public:
  FactorNode_CharLiteral(ASTNodePtr charLiteral, size_t line);
  ~FactorNode_CharLiteral() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<TerminalNode> getCharLiteral() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// factor := BOOL_LITERAL
class FactorNode_BoolLiteral : public FactorNode {
public:
  FactorNode_BoolLiteral(ASTNodePtr boolLiteral, size_t line);
  ~FactorNode_BoolLiteral() override;

  void accept(ASTVisitor &vistor) override;
  std::shared_ptr<TerminalNode> getBoolLiteral() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// factor := variable
class FactorNode_Variable : public FactorNode {
public:
  FactorNode_Variable(ASTNodePtr variable, size_t line);
  ~FactorNode_Variable() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<VariableNode> getVariable() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// factor := LPAREN expression RPAREN
class FactorNode_Lparen_Expression_Rparen : public FactorNode {
public:
  FactorNode_Lparen_Expression_Rparen(ASTNodePtr lparen, ASTNodePtr expression,
                                      ASTNodePtr rparen, size_t line);
  ~FactorNode_Lparen_Expression_Rparen() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<TerminalNode> getLparen() const;
  std::shared_ptr<ExpressionNode> getExpression() const;
  std::shared_ptr<TerminalNode> getRparen() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// factor := NOT factor
class FactorNode_Not_Factor : public FactorNode {
public:
  FactorNode_Not_Factor(ASTNodePtr notToken, ASTNodePtr factor, size_t line);
  ~FactorNode_Not_Factor() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<TerminalNode> getNotToken() const;
  std::shared_ptr<FactorNode> getFactor() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// factor := MINUS factor
class FactorNode_Minus_Factor : public FactorNode {
public:
  FactorNode_Minus_Factor(ASTNodePtr minus, ASTNodePtr factor, size_t line);
  ~FactorNode_Minus_Factor() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<TerminalNode> getMinus() const;
  std::shared_ptr<FactorNode> getFactor() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// factor := ID LPAREN expression_list RPAREN
class FactorNode_ID_Lparen_ExpressionList_Rparen : public FactorNode {
  SymbolType retType;

public:
  FactorNode_ID_Lparen_ExpressionList_Rparen(ASTNodePtr id, ASTNodePtr lparen,
                                             ASTNodePtr expressionList,
                                             ASTNodePtr rparen, size_t line);
  ~FactorNode_ID_Lparen_ExpressionList_Rparen() override;

  void accept(ASTVisitor &visitor) override;
  std::shared_ptr<TerminalNode> getID() const;
  std::shared_ptr<ExpressionListNode> getExpressionList() const;
  void setRetType(const SymbolType &type);
  virtual std::shared_ptr<SymbolType> getType() const override;
};
} // namespace XYZ
