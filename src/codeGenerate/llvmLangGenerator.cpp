

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