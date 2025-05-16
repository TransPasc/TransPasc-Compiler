#pragma once

#define dispatch(production) virtual void visit(class production &node) = 0

namespace XYZ {

class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;

  // TODO: make it much denser and informative as possible
  dispatch(TerminalNode);
  // /  virtual void visit(class TerminalNode &node) = 0;

  virtual void visit(class ProgramStructNode &node) = 0;
  virtual void visit(
      class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node) = 0;

  virtual void visit(class ProgramHeadNode &node) = 0;
  virtual void
  visit(class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen &node) = 0;
  virtual void visit(class ProgramHeadNode_Program_Id &node) = 0;

  virtual void visit(class ProgramBodyNode &node) = 0;
  virtual void
  visit(class
        ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
            &node) = 0;

  virtual void visit(class IdListNode &node) = 0;
  virtual void visit(class IdListNode_Id &node) = 0;
  virtual void visit(class IdListNode_IdList_Comma_Id &node) = 0;

  virtual void visit(class ConstDeclsNode &node) = 0;
  virtual void visit(class ConstDeclsNode_Const_ConstDecl &node) = 0;

  virtual void visit(class ConstDeclNode &node) = 0;
  virtual void visit(class ConstDeclNode_Id_Relop_ConstVal_Semicolon &node) = 0;
  virtual void
  visit(class ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon &node) = 0;

  virtual void visit(class ConstValNode &node) = 0;
  virtual void visit(class ConstValNode_Plus_Number &node) = 0;
  virtual void visit(class ConstValNode_Minus_Number &node) = 0;
  virtual void visit(class ConstValNode_Number &node) = 0;
  virtual void visit(class ConstValNode_CharLiteral &node) = 0;

  virtual void visit(class TypeNode &node) = 0;
  virtual void visit(class TypeNode_BasicType &node) = 0;
  virtual void
  visit(class TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType &node) = 0;

  virtual void visit(class BasicTypeNode &node) = 0;
  virtual void visit(class BasicTypeNode_Integer &node) = 0;
  virtual void visit(class BasicTypeNode_Real &node) = 0;
  virtual void visit(class BasicTypeNode_Boolean &node) = 0;
  virtual void visit(class BasicTypeNode_Char &node) = 0;

  virtual void visit(class PeriodNode &node) = 0;
  virtual void visit(class PeriodNode_Number_Dot_Dot_Number &node) = 0;
  virtual void
  visit(class PeriodNode_Period_Comma_Number_Dot_Dot_Number &node) = 0;

  virtual void visit(class VarDeclsNode &node) = 0;
  virtual void visit(class VarDeclsNode_Var_VarDecl_Semicolon &node) = 0;

  virtual void visit(class VarDeclNode &node) = 0;
  virtual void visit(class VarDeclNode_IdList_Colon_Type &node) = 0;
  virtual void
  visit(class VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type &node) = 0;

  virtual void visit(class SubprogramDeclsNode &node) = 0;
  virtual void
  visit(class SubprogramDeclsNode_SubprogramDecls_Subprogram &node) = 0;

  virtual void visit(class SubprogramNode &node) = 0;
  virtual void
  visit(class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON
            &node) = 0;

  virtual void visit(class SubprogramHeadNode &node) = 0;
  virtual void
  visit(class SubprogramHeadNode_Procedure_Id_FormalParameter &node) = 0;
  virtual void
  visit(class SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType
            &node) = 0;

  virtual void visit(class FormalParameterNode &node) = 0;
  virtual void
  visit(class FormalParameterNode_Lparen_ParameterList_Rparen &node) = 0;

  virtual void visit(class ParameterListNode &node) = 0;
  virtual void visit(class ParameterListNode_Parameter &node) = 0;
  virtual void
  visit(class ParameterListNode_ParameterList_Semicolon_Parameter &node) = 0;

  virtual void visit(class ParameterNode &node) = 0;
  virtual void visit(class ParameterNode_VarParameter &node) = 0;
  virtual void visit(class ParameterNode_ValueParameter &node) = 0;

  virtual void visit(class VarParameterNode &node) = 0;
  virtual void visit(class VarParameterNode_Var_ValueParameter &node) = 0;

  virtual void visit(class ValueParameterNode &node) = 0;
  virtual void visit(class ValueParameterNode_IdList_Colon_BasicType &node) = 0;

  virtual void visit(class SubprogramBodyNode &node) = 0;
  virtual void visit(
      class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement &node) = 0;

  virtual void visit(class CompoundStatementNode &node) = 0;
  virtual void
  visit(class CompoundStatementNode_Begin_StatementList_End &node) = 0;

  virtual void visit(class StatementListNode &node) = 0;
  virtual void visit(class StatementListNode_Statement &node) = 0;
  virtual void
  visit(class StatementListNode_StatementList_Semicolon_Statement &node) = 0;

  virtual void visit(class StatementNode &node) = 0;
  virtual void
  visit(class StatementNode_Variable_Assignop_Expression &node) = 0;
  virtual void visit(class StatementNode_Id_Assignop_Expression &node) = 0;
  virtual void visit(class StatementNode_ProcedureCall &node) = 0;
  virtual void
  visit(class StatementNode_If_Expression_Then_Statement_ElsePart &node) = 0;
  virtual void visit(
      class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
          &node) = 0;
  dispatch(StatementNode_While_Expression_Do_Statement);
  virtual void
  visit(class StatementNode_Read_Lparen_VariableList_Rparen &node) = 0;
  virtual void
  visit(class StatementNode_Write_Lparen_ExpressionList_Rparen &node) = 0;
  virtual void visit(class StatementNode_CompoundStatement &node) = 0;

  virtual void visit(class VariableListNode &node) = 0;
  virtual void visit(class VariableListNode_Variable &node) = 0;
  virtual void
  visit(class VariableListNode_VariableList_Comma_Variable &node) = 0;

  virtual void visit(class VariableNode &node) = 0;
  virtual void visit(class VariableNode_Id_IdVarpart &node) = 0;

  virtual void visit(class IdVarPartNode &node) = 0;
  virtual void
  visit(class IdVarPartNode_Lbracket_ExpressionList_Rbracket &node) = 0;

  virtual void visit(class ProcedureCallNode &node) = 0;
  virtual void visit(class ProcedureCallNode_Id &node) = 0;
  dispatch(ProcedureCallNode_Id_Lparen_Rparen);
  virtual void
  visit(class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen &node) = 0;

  virtual void visit(class ElsePartNode &node) = 0;
  virtual void visit(class ElsePartNode_Else_Statement &node) = 0;

  virtual void visit(class ExpressionListNode &node) = 0;
  virtual void visit(class ExpressionListNode_Expression &node) = 0;
  virtual void
  visit(class ExpressionListNode_ExpressionList_Comma_Expression &node) = 0;

  virtual void visit(class ExpressionNode &node) = 0;
  virtual void visit(class ExpressionNode_SimpleExpression &node) = 0;
  virtual void
  visit(class ExpressionNode_SimpleExpression_Relop_SimpleExpression &node) = 0;

  virtual void visit(class SimpleExpressionNode &node) = 0;
  virtual void visit(class SimpleExpressionNode_Term &node) = 0;
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Plus_Term &node) = 0;
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Minus_Term &node) = 0;
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Or_Term &node) = 0;

  virtual void visit(class TermNode &node) = 0;
  virtual void visit(class TermNode_Factor &node) = 0;
  virtual void visit(class TermNode_Term_Mulop_Factor &node) = 0;

  virtual void visit(class FactorNode &node) = 0;
  virtual void visit(class FactorNode_Number &node) = 0;
  virtual void visit(class FactorNode_CharLiteral &node) = 0;
  virtual void visit(class FactorNode_BoolLiteral &node) = 0;
  virtual void visit(class FactorNode_Variable &node) = 0;
  virtual void visit(class FactorNode_Lparen_Expression_Rparen &node) = 0;
  virtual void visit(class FactorNode_Not_Factor &node) = 0;
  virtual void visit(class FactorNode_Minus_Factor &node) = 0;
  virtual void
  visit(class FactorNode_ID_Lparen_ExpressionList_Rparen &node) = 0;
};

} // namespace XYZ

#undef dispatch
