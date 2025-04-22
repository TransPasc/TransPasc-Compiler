#include "codeGenerate/cLangGenerator.hpp"

namespace XYZ {
CLangGenerator::CLangGenerator() : Generator() {
  // Constructor implementation
}
CLangGenerator::~CLangGenerator() {
  // Destructor implementation
}

void CLangGenerator::setOutputFile(const std::string &filename) {
  m_outputFile = filename;
}

void CLangGenerator::generateCode(ASTNode::ASTNodePtr root) {
  root->accept(*this);
}
void CLangGenerator::visit(class TerminalNode &node) {};

void CLangGenerator::visit(class ProgramStructNode &node) {};
void CLangGenerator::visit(
    class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node) {};

void CLangGenerator::visit(class ProgramHeadNode &node) {};
void CLangGenerator::visit(
    class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen &node) {};
void CLangGenerator::visit(class ProgramHeadNode_Program_Id &node) {};

void CLangGenerator::visit(class ProgramBodyNode &node) {};
void CLangGenerator::visit(
    class ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
        &node) {};

void CLangGenerator::visit(class IdListNode &node) {};
void CLangGenerator::visit(class IdListNode_Id &node) {};
void CLangGenerator::visit(class IdListNode_IdList_Comma_Id &node) {};

void CLangGenerator::visit(class ConstDeclsNode &node) {};
void CLangGenerator::visit(class ConstDeclsNode_Const_ConstDecl &node) {};

void CLangGenerator::visit(class ConstDeclNode &node) {};
void CLangGenerator::visit(
    class ConstDeclNode_Id_Relop_ConstVal_Semicolon &node) {};
void CLangGenerator::visit(
    class ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon &node) {};

void CLangGenerator::visit(class ConstValNode &node) {};
void CLangGenerator::visit(class ConstValNode_Plus_Number &node) {};
void CLangGenerator::visit(class ConstValNode_Minus_Number &node) {};
void CLangGenerator::visit(class ConstValNode_Number &node) {};
void CLangGenerator::visit(class ConstValNode_CharLiteral &node) {};

void CLangGenerator::visit(class TypeNode &node) {};
void CLangGenerator::visit(class TypeNode_BasicType &node) {};
void CLangGenerator::visit(
    class TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType &node) {};

void CLangGenerator::visit(class BasicTypeNode &node) {};
void CLangGenerator::visit(class BasicTypeNode_Integer &node) {};
void CLangGenerator::visit(class BasicTypeNode_Real &node) {};
void CLangGenerator::visit(class BasicTypeNode_Boolean &node) {};
void CLangGenerator::visit(class BasicTypeNode_Char &node) {};

void CLangGenerator::visit(class PeriodNode &node) {};
void CLangGenerator::visit(class PeriodNode_Number_Dot_Dot_Number &node) {};
void CLangGenerator::visit(
    class PeriodNode_Period_Comma_Number_Dot_Dot_Number &node) {};

void CLangGenerator::visit(class VarDeclsNode &node) {};
void CLangGenerator::visit(class VarDeclsNode_Var_VarDecl_Semicolon &node) {};

void CLangGenerator::visit(class VarDeclNode &node) {};
void CLangGenerator::visit(class VarDeclNode_IdList_Colon_Type &node) {};
void CLangGenerator::visit(
    class VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type &node) {};

void CLangGenerator::visit(class SubprogramDeclsNode &node) {};
void CLangGenerator::visit(
    class SubprogramDeclsNode_SubprogramDecls_Subprogram &node) {};

void CLangGenerator::visit(class SubprogramNode &node) {};
void CLangGenerator::visit(
    class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON
        &node) {};

void CLangGenerator::visit(class SubprogramHeadNode &node) {};
void CLangGenerator::visit(
    class SubprogramHeadNode_Procedure_Id_FormalParameter &node) {};
void CLangGenerator::visit(
    class SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType
        &node) {};

void CLangGenerator::visit(class FormalParameterNode &node) {};
void CLangGenerator::visit(
    class FormalParameterNode_Lparen_ParameterList_Rparen &node) {};

void CLangGenerator::visit(class ParameterListNode &node) {};
void CLangGenerator::visit(class ParameterListNode_Parameter &node) {};
void CLangGenerator::visit(
    class ParameterListNode_ParameterList_Semicolon_Parameter &node) {};

void CLangGenerator::visit(class ParameterNode &node) {};
void CLangGenerator::visit(class ParameterNode_VarParameter &node) {};
void CLangGenerator::visit(class ParameterNode_ValueParameter &node) {};

void CLangGenerator::visit(class VarParameterNode &node) {};
void CLangGenerator::visit(class VarParameterNode_Var_ValueParameter &node) {};

void CLangGenerator::visit(class ValueParameterNode &node) {};
void CLangGenerator::visit(
    class ValueParameterNode_IdList_Colon_BasicType &node) {};

void CLangGenerator::visit(class SubprogramBodyNode &node) {};
void CLangGenerator::visit(
    class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement &node) {};

void CLangGenerator::visit(class CompoundStatementNode &node) {};
void CLangGenerator::visit(
    class CompoundStatementNode_Begin_StatementList_End &node) {};

void CLangGenerator::visit(class StatementListNode &node) {};
void CLangGenerator::visit(class StatementListNode_Statement &node) {};
void CLangGenerator::visit(
    class StatementListNode_StatementList_Semicolon_Statement &node) {};

void CLangGenerator::visit(class StatementNode &node) {};
void CLangGenerator::visit(
    class StatementNode_Variable_Assignop_Expression &node) {};
void CLangGenerator::visit(class StatementNode_Id_Assignop_Expression &node) {};
void CLangGenerator::visit(class StatementNode_ProcedureCall &node) {};
void CLangGenerator::visit(
    class StatementNode_If_Expression_Then_Statement_ElsePart &node) {};
void CLangGenerator::visit(
    class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
        &node) {};
void CLangGenerator::visit(
    class StatementNode_Read_Lparen_VariableList_Rparen &node) {};
void CLangGenerator::visit(
    class StatementNode_Write_Lparen_ExpressionList_Rparen &node) {};
void CLangGenerator::visit(class StatementNode_CompoundStatement &node) {};

void CLangGenerator::visit(class VariableListNode &node) {};
void CLangGenerator::visit(class VariableListNode_Variable &node) {};
void CLangGenerator::visit(
    class VariableListNode_VariableList_Comma_Variable &node) {};

void CLangGenerator::visit(class VariableNode &node) {};
void CLangGenerator::visit(class VariableNode_Id_IdVarpart &node) {};

void CLangGenerator::visit(class IdVarPartNode &node) {};
void CLangGenerator::visit(
    class IdVarPartNode_Lbracket_ExpressionList_Rbracket &node) {};

void CLangGenerator::visit(class ProcedureCallNode &node) {};
void CLangGenerator::visit(class ProcedureCallNode_Id &node) {};
void CLangGenerator::visit(
    class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen &node) {};

void CLangGenerator::visit(class ElsePartNode &node) {};
void CLangGenerator::visit(class ElsePartNode_Else_Statement &node) {};

void CLangGenerator::visit(class ExpressionListNode &node) {};
void CLangGenerator::visit(class ExpressionListNode_Expression &node) {};
void CLangGenerator::visit(
    class ExpressionListNode_ExpressionList_Comma_Expression &node) {};

void CLangGenerator::visit(class ExpressionNode &node) {};
void CLangGenerator::visit(class ExpressionNode_SimpleExpression &node) {};
void CLangGenerator::visit(
    class ExpressionNode_SimpleExpression_Relop_SimpleExpression &node) {};

void CLangGenerator::visit(class SimpleExpressionNode &node) {};
void CLangGenerator::visit(class SimpleExpressionNode_Term &node) {};
void CLangGenerator::visit(
    class SimpleExpressionNode_SimpleExpression_Plus_Term &node) {};
void CLangGenerator::visit(
    class SimpleExpressionNode_SimpleExpression_Minus_Term &node) {};
void CLangGenerator::visit(
    class SimpleExpressionNode_SimpleExpression_Or_Term &node) {};

void CLangGenerator::visit(class TermNode &node) {};
void CLangGenerator::visit(class TermNode_Factor &node) {};
void CLangGenerator::visit(class TermNode_Term_Mulop_Factor &node) {};

void CLangGenerator::visit(class FactorNode &node) {};
void CLangGenerator::visit(class FactorNode_Number &node) {};
void CLangGenerator::visit(class FactorNode_CharLiteral &node) {};
void CLangGenerator::visit(class FactorNode_Variable &node) {};
void CLangGenerator::visit(class FactorNode_Lparen_Expression_Rparen &node) {};
void CLangGenerator::visit(class FactorNode_Not_Factor &node) {};
void CLangGenerator::visit(class FactorNode_Minus_Factor &node) {};
void CLangGenerator::visit(
    class FactorNode_ID_Lparen_ExpressionList_Rparen &node) {};
} // namespace XYZ
