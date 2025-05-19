#include "codeGenerate/llvmIrGenerator.hpp"
#include "ast/ast.hpp"
#include <algorithm>
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#define impl(node_type) impl_visit(LLVMIrGenerator, node_type)

namespace XYZ {
LLVMIrGenerator::LLVMIrGenerator() : Generator() {}
LLVMIrGenerator::~LLVMIrGenerator() {}
void LLVMIrGenerator::setOutputFile(const std::string &filename) {
  m_outputFile = filename;
}

void LLVMIrGenerator::generateCode(ASTNode::ASTNodePtr root) {
  symbolTable = std::make_shared<StackLinkedSymbolTable>();
  symbolTable->enterBlock();
  std::cout << "Generating LLVM IR..." << std::endl;
  root->accept(*this);
  symbolTable->exitBlock();
  if (m_outputFile.empty()) {
    std::cout << m_outputBuffer;
    return;
  }
  std::ofstream outFile(m_outputFile);
  if (outFile.is_open()) {
    outFile << m_outputBuffer;
    outFile.close();
  } else {
    std::cerr << "Unable to open file: " << m_outputFile << std::endl;
  }
}

void LLVMIrGenerator::writeln(const std::string &str) {
  m_outputBuffer += str + "\n";
}

std::string LLVMIrGenerator::symbolType2LLVMStr(const SymbolType &type) {
  // 这里只实现最基础的类型映射
  static const auto printer =
      overloaded{[](std::monostate) -> std::string { return "void"; },
                 [](BasicType basic) -> std::string {
                   switch (basic) {
                   case BasicType::INTEGER:
                     return "i32";
                   case BasicType::REAL:
                     return "double";
                   case BasicType::BOOLEAN:
                     return "i1";
                   case BasicType::CHAR:
                     return "i8";
                   default:
                     return "i32";
                   }
                 },
                 [](auto &&) -> std::string { return "i32"; }};
  return type.visit(printer);
}
std::string LLVMIrGenerator::relop2LLVMStr(const std::string &relop) {
  if (relop == "<")
    return "icmp slt";
  if (relop == "<=")
    return "icmp sle";
  if (relop == ">")
    return "icmp sgt";
  if (relop == ">=")
    return "icmp sge";
  if (relop == "==")
    return "icmp eq";
  if (relop == "!=")
    return "icmp ne";
  return "icmp eq";
}
std::string LLVMIrGenerator::mulop2LLVMStr(const std::string &mulop) {
  if (mulop == "*")
    return "mul";
  if (mulop == "/")
    return "sdiv";
  if (mulop == "%")
    return "srem";
  return "mul";
}
impl(TerminalNode) {
  if (node.isRelOp()) {
    m_outputBuffer += std::format("{} ", relop2LLVMStr(node.getValStr()));
    return;
  }
  if (node.isMulOp()) {
    m_outputBuffer += std::format("{} ", mulop2LLVMStr(node.getValStr()));
    return;
  }
  m_outputBuffer += std::format("{} ", node.getValStr());
}
impl(ProgramStructNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProgramStructNode should not be visited");
}
impl(ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot) {
  writeln("; Generated LLVM IR by The King Of Pascal Compiler(kpc)");
  // macosx
  writeln("target datalayout = \"e-m:o-i64:64-f80:128-n8:16:32:64-S128\"");
  writeln("target triple = \"x86_64-apple-macosx10.15.0\"");
  writeln("");
  // linux
  writeln("; target datalayout = "
          "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
          "S128\"");
  writeln("; target triple = \"x86_64-pc-linux-gnu\"");
  writeln("");
  // windows
  writeln("; target datalayout = \"e-m:e-i64:64-f80:128-n8:16:32:64-S128\"");
  writeln("; target triple = \"x86_64-unknown-unknown-windows\"");
  writeln("");

  writeln("; program struct");
  writeln("declare i32 @printf(i8*, ...)");
  writeln("declare i32 @scanf(i8*, ...)");
  node.getProgramHead()->accept(*this);
  node.getSemicolon()->accept(*this);
  node.getProgramBody()->accept(*this);
}

impl(ProgramHeadNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProgramHeadNode should not be visited");
}
impl(ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen) {
  // TODO:暂不支持
  throw CodeGenerateException(
      ErrType::UNREACH_CODE,
      "ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen should not be visited, "
      "because we don't support this");
}
impl(ProgramHeadNode_Program_Id) { writeln("; program head"); }

impl(ProgramBodyNode) {}

impl(ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement) {
  node.getConstDecls()->accept(*this);
  node.getVarDecls()->accept(*this);
  node.getSubprogramDecls()->accept(*this);
  //   main func
  m_outputBuffer += "\ndefine i32 @main() {\n";
  node.getCompoundStatement()->accept(*this);
  m_outputBuffer += "  ret i32 0\n";
  m_outputBuffer += "}\n";
}

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
