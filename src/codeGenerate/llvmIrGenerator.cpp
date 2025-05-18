#include "codeGenerate/llvmIrGenerator.hpp"
#define impl(node_type) impl_visit(LLVMIrGenerator, node_type)

namespace XYZ {
LLVMIrGenerator::LLVMIrGenerator() {}
LLVMIrGenerator::~LLVMIrGenerator() {}
void LLVMIrGenerator::generateCode(ASTNode::ASTNodePtr root) {}
void LLVMIrGenerator::setOutputFile(const std::string &filename) {}

impl(TerminalNode) {}
impl(ProgramStructNode) {}
impl(ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot) {}

impl(ProgramHeadNode) {}
impl(ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen) {}
impl(ProgramHeadNode_Program_Id) {}

impl(ProgramBodyNode) {}

impl(ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement) {}

impl(IdListNode) {}
impl(IdListNode_Id) {}
impl(IdListNode_IdList_Comma_Id) {}

impl(ConstDeclsNode) {}
impl(ConstDeclsNode_Const_ConstDecl) {}

impl(ConstDeclNode) {}
impl(ConstDeclNode_Id_Relop_ConstVal_Semicolon) {}
impl(ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon) {}

impl(ConstValNode) {}
impl(ConstValNode_Plus_Number) {}
impl(ConstValNode_Minus_Number) {}
impl(ConstValNode_Number) {}
impl(ConstValNode_StringLiteral) {}
impl(ConstValNode_CharLiteral) {}

impl(TypeNode) {}
impl(TypeNode_BasicType) {}

impl(TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType) {}
impl(TypeNode_String) {}

impl(BasicTypeNode) {}
impl(BasicTypeNode_Integer) {}
impl(BasicTypeNode_Real) {}
impl(BasicTypeNode_Boolean) {}
impl(BasicTypeNode_Char) {}

impl(PeriodNode) {}
impl(PeriodNode_Number_Dot_Dot_Number) {}
impl(PeriodNode_Period_Comma_Number_Dot_Dot_Number) {}

impl(VarDeclsNode) {}
impl(VarDeclsNode_Var_VarDecl_Semicolon) {}

impl(VarDeclNode) {}
impl(VarDeclNode_IdList_Colon_Type) {}
impl(VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type) {}

impl(SubprogramDeclsNode) {}
impl(SubprogramDeclsNode_SubprogramDecls_Subprogram) {}

impl(SubprogramNode) {}

impl(SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON) {}

impl(SubprogramHeadNode) {}
impl(SubprogramHeadNode_Procedure_Id_FormalParameter) {}

impl(SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType) {}

impl(FormalParameterNode) {}
impl(FormalParameterNode_Lparen_ParameterList_Rparen) {}

impl(ParameterListNode) {}
impl(ParameterListNode_Parameter) {}
impl(ParameterListNode_ParameterList_Semicolon_Parameter) {}

impl(ParameterNode) {}
impl(ParameterNode_VarParameter) {}
impl(ParameterNode_ValueParameter) {}

impl(VarParameterNode) {}
impl(VarParameterNode_Var_ValueParameter) {}

impl(ValueParameterNode) {}
impl(ValueParameterNode_IdList_Colon_BasicType) {}

impl(SubprogramBodyNode) {}
impl(SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement) {}

impl(CompoundStatementNode) {}
impl(CompoundStatementNode_Begin_StatementList_End) {}

impl(StatementListNode) {}
impl(StatementListNode_Statement) {}
impl(StatementListNode_StatementList_Semicolon_Statement) {}

impl(StatementNode) {}
impl(StatementNode_Variable_Assignop_Expression) {}
impl(StatementNode_Id_Assignop_Expression) {}
impl(StatementNode_ProcedureCall) {}
impl(StatementNode_If_Expression_Then_Statement_ElsePart) {}
impl(

    StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement) {}
impl(StatementNode_While_Expression_Do_Statement) {}
impl(StatementNode_Read_Lparen_VariableList_Rparen) {}
impl(StatementNode_Write_Lparen_ExpressionList_Rparen) {}
impl(StatementNode_CompoundStatement) {}
impl(StatementNode_Break) {}
impl(StatementNode_Continue) {}

impl(VariableListNode) {}
impl(VariableListNode_Variable) {}
impl(VariableListNode_VariableList_Comma_Variable) {}

impl(VariableNode) {}
impl(VariableNode_Id_IdVarpart) {}

impl(IdVarPartNode) {}
impl(IdVarPartNode_Lbracket_ExpressionList_Rbracket) {}

impl(ProcedureCallNode) {}
impl(ProcedureCallNode_Id) {}
impl(ProcedureCallNode_Id_Lparen_Rparen) {}
impl(ProcedureCallNode_Id_Lparen_ExpressionList_Rparen) {}

impl(ElsePartNode) {}
impl(ElsePartNode_Else_Statement) {}

impl(ExpressionListNode) {}
impl(ExpressionListNode_Expression) {}
impl(ExpressionListNode_ExpressionList_Comma_Expression) {}

impl(ExpressionNode) {}
impl(ExpressionNode_SimpleExpression) {}

impl(ExpressionNode_SimpleExpression_Relop_SimpleExpression) {}

impl(SimpleExpressionNode) {}
impl(SimpleExpressionNode_Term) {}
impl(SimpleExpressionNode_SimpleExpression_Plus_Term) {}
impl(SimpleExpressionNode_SimpleExpression_Minus_Term) {}
impl(SimpleExpressionNode_SimpleExpression_Or_Term) {}

impl(TermNode) {}
impl(TermNode_Factor) {}
impl(TermNode_Term_Mulop_Factor) {}

impl(FactorNode) {}
impl(FactorNode_Number) {}
impl(FactorNode_CharLiteral) {}
impl(FactorNode_BoolLiteral) {}
impl(FactorNode_Variable) {}
impl(FactorNode_Lparen_Expression_Rparen) {}
impl(FactorNode_Not_Factor) {}
impl(FactorNode_Plus_Factor) {}
impl(FactorNode_Minus_Factor) {}
impl(FactorNode_ID_Lparen_ExpressionList_Rparen) {}

} // namespace XYZ

#undef impl
