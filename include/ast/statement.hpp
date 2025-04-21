#pragma once
#include "ast/ast.h"

namespace XYZ {

// statement 的基类
// statement := EMPTY
class StatementNode : public ASTNode {
public:
  StatementNode(size_t line) : ASTNode("Statement", line) {}
  ~StatementNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// statement := variable ASSIGNOP expression
class StatementNode_Variable_Assignop_Expression : public StatementNode {
public:
  StatementNode_Variable_Assignop_Expression(ASTNodePtr variable,
                                             ASTNodePtr assignop,
                                             ASTNodePtr expression, size_t line)
      : StatementNode(line) {
    addChild(variable);
    addChild(assignop);
    addChild(expression);
  }
  ~StatementNode_Variable_Assignop_Expression() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<VariableNode> getVariable() const {
    return std::dynamic_pointer_cast<VariableNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getAssignop() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<ExpressionNode> getExpression() const {
    return std::dynamic_pointer_cast<ExpressionNode>(m_children[2]);
  }
};

// statement := ID ASSIGNOP expression
class StatementNode_Id_Assignop_Expression : public StatementNode {
public:
  StatementNode_Id_Assignop_Expression(ASTNodePtr id, ASTNodePtr assignop,
                                       ASTNodePtr expression, size_t line)
      : StatementNode(line) {
    addChild(id);
    addChild(assignop);
    addChild(expression);
  }
  ~StatementNode_Id_Assignop_Expression() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getId() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getAssignop() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<ExpressionNode> getExpression() const {
    return std::dynamic_pointer_cast<ExpressionNode>(m_children[2]);
  }
};

// statement := procedure_call
class StatementNode_ProcedureCall : public StatementNode {
public:
  StatementNode_ProcedureCall(ASTNodePtr procedureCall, size_t line)
      : StatementNode(line) {
    addChild(procedureCall);
  }
  ~StatementNode_ProcedureCall() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<ProcedureCallNode> getProcedureCall() const {
    return std::dynamic_pointer_cast<ProcedureCallNode>(m_children[0]);
  }
};

// statement := IF expression THEN statement else_part
class StatementNode_If_Expression_Then_Statement_ElsePart
    : public StatementNode {
public:
  StatementNode_If_Expression_Then_Statement_ElsePart(
      ASTNodePtr ifToken, ASTNodePtr expression, ASTNodePtr thenToken,
      ASTNodePtr statement, ASTNodePtr elsePart, size_t line)
      : StatementNode(line) {
    addChild(ifToken);
    addChild(expression);
    addChild(thenToken);
    addChild(statement);
    addChild(elsePart);
  }
  ~StatementNode_If_Expression_Then_Statement_ElsePart() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getIfToken() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<ExpressionNode> getExpression() const {
    return std::dynamic_pointer_cast<ExpressionNode>(m_children[1]);
  }
  std::shared_ptr<TerminalNode> getThenToken() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
  std::shared_ptr<StatementNode> getStatement() const {
    return std::dynamic_pointer_cast<StatementNode>(m_children[3]);
  }
  std::shared_ptr<ElsePartNode> getElsePart() const {
    return std::dynamic_pointer_cast<ElsePartNode>(m_children[4]);
  }
};

// statement := FOR ID ASSIGNOP expression TO expression DO statement
class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
    : public StatementNode {
public:
  StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement(
      ASTNodePtr forToken, ASTNodePtr id, ASTNodePtr assignop,
      ASTNodePtr expression1, ASTNodePtr toToken, ASTNodePtr expression2,
      ASTNodePtr doToken, ASTNodePtr statement, size_t line)
      : StatementNode(line) {
    addChild(forToken);
    addChild(id);
    addChild(assignop);
    addChild(expression1);
    addChild(toToken);
    addChild(expression2);
    addChild(doToken);
    addChild(statement);
  }
  ~StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement()
      override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getForToken() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getId() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<TerminalNode> getAssignop() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
  std::shared_ptr<ExpressionNode> getExpression1() const {
    return std::dynamic_pointer_cast<ExpressionNode>(m_children[3]);
  }
  std::shared_ptr<TerminalNode> getToToken() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[4]);
  }
  std::shared_ptr<ExpressionNode> getExpression2() const {
    return std::dynamic_pointer_cast<ExpressionNode>(m_children[5]);
  }
  std::shared_ptr<TerminalNode> getDoToken() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[6]);
  }
  std::shared_ptr<StatementNode> getStatement() const {
    return std::dynamic_pointer_cast<StatementNode>(m_children[7]);
  }
};

// statement := READ LPAREN variable_list RPAREN
class StatementNode_Read_Lparen_VariableList_Rparen : public StatementNode {
public:
  StatementNode_Read_Lparen_VariableList_Rparen(ASTNodePtr readToken,
                                                ASTNodePtr lparen,
                                                ASTNodePtr variableList,
                                                ASTNodePtr rparen, size_t line)
      : StatementNode(line) {
    addChild(readToken);
    addChild(lparen);
    addChild(variableList);
    addChild(rparen);
  }
  ~StatementNode_Read_Lparen_VariableList_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getReadToken() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getLparen() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<VariableListNode> getVariableList() const {
    return std::dynamic_pointer_cast<VariableListNode>(m_children[2]);
  }
  std::shared_ptr<TerminalNode> getRparen() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[3]);
  }
};

// statement := WRITE LPAREN expression_list RPAREN
class StatementNode_Write_Lparen_ExpressionList_Rparen : public StatementNode {
public:
  StatementNode_Write_Lparen_ExpressionList_Rparen(ASTNodePtr writeToken,
                                                   ASTNodePtr lparen,
                                                   ASTNodePtr expressionList,
                                                   ASTNodePtr rparen,
                                                   size_t line)
      : StatementNode(line) {
    addChild(writeToken);
    addChild(lparen);
    addChild(expressionList);
    addChild(rparen);
  }
  ~StatementNode_Write_Lparen_ExpressionList_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getWriteToken() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getLparen() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<ExpressionListNode> getExpressionList() const {
    return std::dynamic_pointer_cast<ExpressionListNode>(m_children[2]);
  }
  std::shared_ptr<TerminalNode> getRparen() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[3]);
  }
};

} // namespace XYZ
