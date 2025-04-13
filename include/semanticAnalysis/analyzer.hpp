#pragma once
#include "ast/ast.hpp"
// TODO: 补充各个节点的visit函数
namespace XYZ {
using namespace std;
class Analyzer : public ASTVisitor {
  shared_ptr<ProgramStructNode> root = nullptr;

public:
  Analyzer() {}

  void analyze(shared_ptr<ProgramStructNode> root) {
    this->root = root;
    root->accept(*this);
  }

  virtual void visit(class TerminalNode &node) {};

  virtual void visit(class ProgramStructNode &node) {
    cout << "ProgramStructNode" << endl;
  };
  virtual void
  visit(class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node) {
    cout << "ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot" << endl;
  };

  virtual void visit(class ProgramHeadNode &node) {};
  virtual void
  visit(class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen &node) {};
  virtual void visit(class ProgramHeadNode_Program_Id &node) {};

  virtual void visit(class ProgramBodyNode &node) {};
  virtual void
  visit(class
        ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
            &node) {};

  virtual void visit(class IdListNode &node) {};
  virtual void visit(class IdListNode_Id &node) {};
  virtual void visit(class IdListNode_IdList_Comma_Id &node) {};

  virtual void visit(class ConstDeclsNode &node) {};
  virtual void visit(class ConstDeclsNode_Const_ConstDecl &node) {};

  virtual void visit(class ConstDeclNode &node) {};
  virtual void visit(class ConstDeclNode_Id_Assignop_ConstVal &node) {};
  virtual void
  visit(class ConstDeclNode_ConstDecl_Semicolon_Id_Assignop_ConstVal &node) {};

  virtual void visit(class ConstValNode &node) {};
  virtual void visit(class ConstValNode_Plus_Number &node) {};
  virtual void visit(class ConstValNode_Minus_Number &node) {};
  virtual void visit(class ConstValNode_Number &node) {};
  virtual void visit(class ConstValNode_CharLiteral &node) {};

  virtual void visit(class TypeNode &node) {};
  virtual void visit(class TypeNode_BasicType &node) {};
  virtual void
  visit(class TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType &node) {};

  virtual void visit(class BasicTypeNode &node) {};
  virtual void visit(class BasicTypeNode_Integer &node) {};
  virtual void visit(class BasicTypeNode_Real &node) {};
  virtual void visit(class BasicTypeNode_Boolean &node) {};
  virtual void visit(class BasicTypeNode_Char &node) {};

  virtual void visit(class PeriodNode &node) {};
  virtual void visit(class PeriodNode_Number_Dot_Dot_Number &node) {};
  virtual void
  visit(class PeriodNode_Period_Comma_Number_Dot_Dot_Number &node) {};

  virtual void visit(class VarDeclsNode &node) {};
  virtual void visit(class VarDeclsNode_Var_VarDecl_Semicolon &node) {};

  virtual void visit(class VarDeclNode &node) {};
  virtual void visit(class VarDeclNode_IdList_Colon_Type &node) {};
  virtual void
  visit(class VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type &node) {};

  virtual void visit(class SubprogramDeclsNode &node) {};
  virtual void
  visit(class SubprogramDeclsNode_SubprogramDecls_Subprogram &node) {};

  virtual void visit(class SubprogramNode &node) {};
  virtual void
  visit(class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody &node) {};

  virtual void visit(class SubprogramHeadNode &node) {};
  virtual void
  visit(class SubprogramHeadNode_Procedure_Id_FormalParameter &node) {};
  virtual void
  visit(class SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType
            &node) {};

  virtual void visit(class FormalParameterNode &node) {};
  virtual void visit(class FormalParameterNode_Empty &node) {};
  virtual void
  visit(class FormalParameterNode_Lparen_ParameterList_Rparen &node) {};

  virtual void visit(class ParameterListNode &node) {};
  virtual void visit(class ParameterListNode_Parameter &node) {};
  virtual void
  visit(class ParameterListNode_ParameterList_Semicolon_Parameter &node) {};

  virtual void visit(class ParameterNode &node) {};
  virtual void visit(class ParameterNode_VarParameter &node) {};
  virtual void visit(class ParameterNode_ValueParameter &node) {};

  virtual void visit(class VarParameterNode &node) {};
  virtual void visit(class VarParameterNode_Var_ValueParameter &node) {};

  virtual void visit(class ValueParameterNode &node) {};
  virtual void visit(class ValueParameterNode_IdList_Colon_BasicType &node) {};

  virtual void visit(class SubprogramBodyNode &node) {};
  virtual void
  visit(class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement &node) {
  };

  virtual void visit(class CompoundStatementNode &node) {};
  virtual void
  visit(class CompoundStatementNode_Begin_StatementList_End &node) {};

  virtual void visit(class StatementListNode &node) {};
  virtual void visit(class StatementListNode_Statement &node) {};
  virtual void
  visit(class StatementListNode_StatementList_Semicolon_Statement &node) {};

  virtual void visit(class StatementNode &node) {};
  virtual void visit(class StatementNode_Variable_Assignop_Expression &node) {};
  virtual void visit(class StatementNode_Id_Assignop_Expression &node) {};
  virtual void visit(class StatementNode_ProcedureCall &node) {};
  virtual void
  visit(class StatementNode_If_Expression_Then_Statement_ElsePart &node) {};
  virtual void visit(
      class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
          &node) {};
  virtual void
  visit(class StatementNode_Read_Lparen_VariableList_Rparen &node) {};
  virtual void
  visit(class StatementNode_Write_Lparen_ExpressionList_Rparen &node) {};

  virtual void visit(class VariableListNode &node) {};
  virtual void visit(class VariableListNode_Variable &node) {};
  virtual void visit(class VariableListNode_VariableList_Comma_Variable &node) {
  };

  virtual void visit(class VariableNode &node) {};
  virtual void visit(class VariableNode_Id_IdVarpart &node) {};

  virtual void visit(class IdVarPartNode &node) {};
  virtual void visit(class IdVarPartNode_Lbracket_Expression_Rbracket &node) {};

  virtual void visit(class ProcedureCallNode &node) {};
  virtual void visit(class ProcedureCallNode_Id &node) {};
  virtual void
  visit(class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen &node) {};

  virtual void visit(class ElsePartNode &node) {};
  virtual void visit(class ElsePartNode_Else_Statement &node) {};

  virtual void visit(class ExpressionListNode &node) {};
  virtual void visit(class ExpressionListNode_Expression &node) {};
  virtual void
  visit(class ExpressionListNode_ExpressionList_Comma_Expression &node) {};

  virtual void visit(class ExpressionNode &node) {};
  virtual void visit(class ExpressionNode_SimpleExpression &node) {};
  virtual void
  visit(class ExpressionNode_SimpleExpression_Relop_SimpleExpression &node) {};

  virtual void visit(class SimpleExpressionNode &node) {};
  virtual void visit(class SimpleExpressionNode_Term &node) {};
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Plus_Term &node) {};
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Minus_Term &node) {};
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Or_Term &node) {};

  virtual void visit(class TermNode &node) {};
  virtual void visit(class TermNode_Factor &node) {};
  virtual void visit(class TermNode_Term_Mulop_Factor &node) {};

  virtual void visit(class FactorNode &node) {};
  virtual void visit(class FactorNode_Number &node) {};
  virtual void visit(class FactorNode_CharLiteral &node) {};
  virtual void visit(class FactorNode_Variable &node) {};
  virtual void visit(class FactorNode_Lparen_Expression_Rparen &node) {};
  virtual void visit(class FactorNode_Not_Factor &node) {};
  virtual void visit(class FactorNode_Minus_Factor &node) {};
};
} // namespace XYZ
