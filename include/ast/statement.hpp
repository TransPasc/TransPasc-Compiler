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

  ASTNodePtr getVariable() const { return m_children[0]; }
  ASTNodePtr getAssignop() const { return m_children[1]; }
  ASTNodePtr getExpression() const { return m_children[2]; }
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

  ASTNodePtr getId() const { return m_children[0]; }
  ASTNodePtr getAssignop() const { return m_children[1]; }
  ASTNodePtr getExpression() const { return m_children[2]; }
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

  ASTNodePtr getProcedureCall() const { return m_children[0]; }
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

  ASTNodePtr getIfToken() const { return m_children[0]; }
  ASTNodePtr getExpression() const { return m_children[1]; }
  ASTNodePtr getThenToken() const { return m_children[2]; }
  ASTNodePtr getStatement() const { return m_children[3]; }
  ASTNodePtr getElsePart() const { return m_children[4]; }
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

  ASTNodePtr getForToken() const { return m_children[0]; }
  ASTNodePtr getId() const { return m_children[1]; }
  ASTNodePtr getAssignop() const { return m_children[2]; }
  ASTNodePtr getExpression1() const { return m_children[3]; }
  ASTNodePtr getToToken() const { return m_children[4]; }
  ASTNodePtr getExpression2() const { return m_children[5]; }
  ASTNodePtr getDoToken() const { return m_children[6]; }
  ASTNodePtr getStatement() const { return m_children[7]; }
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

  ASTNodePtr getReadToken() const { return m_children[0]; }
  ASTNodePtr getLparen() const { return m_children[1]; }
  ASTNodePtr getVariableList() const { return m_children[2]; }
  ASTNodePtr getRparen() const { return m_children[3]; }
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

  ASTNodePtr getWriteToken() const { return m_children[0]; }
  ASTNodePtr getLparen() const { return m_children[1]; }
  ASTNodePtr getExpressionList() const { return m_children[2]; }
  ASTNodePtr getRparen() const { return m_children[3]; }
};

}  // namespace XYZ
