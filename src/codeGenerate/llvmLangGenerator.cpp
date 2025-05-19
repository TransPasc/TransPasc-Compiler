// #include "codeGenerate/llvmLangGenerator.hpp"
// #include "ast/ast.hpp"
// #include <fstream>
// #include <iostream>
// #include <memory>
// #include <sstream>
// #include <string>

// namespace XYZ {} // namespace XYZ

// /**

// void LLVMLangGenerator::visit(
//     class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node) {
//   writeln("; Generated LLVM IR by Pascal Compiler");
//   writeln("declare i32 @printf(i8*, ...)");
//   writeln("declare i32 @scanf(i8*, ...)");
//   node.getProgramHead()->accept(*this);
//   node.getSemicolon()->accept(*this);
//   node.getProgramBody()->accept(*this);
// }

// void LLVMLangGenerator::visit(
//     class
//     ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
//         &node) {
//   node.getConstDecls()->accept(*this);
//   node.getVarDecls()->accept(*this);
//   node.getSubprogramDecls()->accept(*this);
//   m_outputBuffer += "define i32 @main() {\n";
//   node.getCompoundStatement()->accept(*this);
//   m_outputBuffer += "  ret i32 0\n";
//   m_outputBuffer += "}\n";
// }

// void LLVMLangGenerator::visit(class ConstDeclsNode_Const_ConstDecl &node) {
//   writeln("; const decls");
//   node.getConstDecl()->accept(*this);
// }

// void LLVMLangGenerator::visit(
//     class ConstDeclNode_Id_Relop_ConstVal_Semicolon &node) {
//   // 这里只做简单的全局常量定义
//   auto type = node.getConstVal()->getType();
//   std::string llvmType = symbolType2LLVMStr(*type);
//   std::string name = node.getId()->get<std::string>();
//   m_outputBuffer += std::string("@") + name + " = constant " + llvmType + "
//   "; node.getConstVal()->accept(*this); m_outputBuffer += "\n";
// }

// void LLVMLangGenerator::visit(class VarDeclsNode_Var_VarDecl_Semicolon &node)
// {
//   writeln("; var decls");
//   node.getVarDecl()->accept(*this);
// }

// void LLVMLangGenerator::visit(class VarDeclNode_IdList_Colon_Type &node) {
//   auto type = node.getType()->getType();
//   auto ids = node.getIdList()->getAllIds();
//   std::string llvmType = symbolType2LLVMStr(*type);
//   for (const auto &id : ids) {
//     m_outputBuffer +=
//         std::string("@") + id->getValStr() + " = global " + llvmType + "
//         0\n";
//   }
// }

// void LLVMLangGenerator::visit(
//     class CompoundStatementNode_Begin_StatementList_End &node) {
//   m_outputBuffer += "; compound statement\n";
//   node.getStatementList()->accept(*this);
// }
//  */