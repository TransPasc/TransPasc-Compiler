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
using Section = XYZ::OutputBuffer::Section;
namespace XYZ {

LLVMIrGenerator::LLVMIrGenerator() : Generator() {}
LLVMIrGenerator::~LLVMIrGenerator() {}
void LLVMIrGenerator::setOutputFile(const std::string &filename) {
  m_outputFile = filename;
}

void LLVMIrGenerator::generateCode(ASTNode::ASTNodePtr root) {
  symbolTable = std::make_shared<StackLinkedSymbolTable<LLVMIRSymbolRecord>>();
  symbolTable->enterBlock();
  std::cout << "Generating LLVM IR..." << std::endl;
  root->accept(*this);
  symbolTable->exitBlock();
  if (m_outputFile.empty()) {
    std::cout << m_outputBuffer.toString();
    return;
  }
  std::ofstream outFile(m_outputFile);
  if (outFile.is_open()) {
    outFile << m_outputBuffer.toString();
    outFile.close();
  } else {
    std::cerr << "Unable to open file: " << m_outputFile << std::endl;
  }
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
std::string LLVMIrGenerator::getDefaultValue(const SymbolType &type) {
  static const auto printer =
      overloaded{[](std::monostate) -> std::string { return "0"; },
                 [](BasicType basic) -> std::string {
                   switch (basic) {
                   case BasicType::INTEGER:
                     return "0";
                   case BasicType::REAL:
                     return "0.0";
                   case BasicType::BOOLEAN:
                     return "false";
                   case BasicType::CHAR:
                     return "'\\00'";
                   default:
                     return "0";
                   }
                 },
                 [](auto &&) -> std::string { return "0"; }};
  return type.visit(printer);
}
std::string LLVMIrGenerator::getFmtStrName(const std::string &fmt) {
  std::string fmtStr = "$fmt_";
  for (char ch : fmt) {
    fmtStr += std::format("{:02x}", static_cast<int>(ch));
  }
  return fmtStr;
}
std::string LLVMIrGenerator::getUnNameIdStr() {
  std::string id = std::format("%{}", unname_id_num);
  unname_id_num++;
  return id;
}
std::string LLVMIrGenerator::getLLVMStyleIOFormatStr(
    const std::vector<std::shared_ptr<SymbolType>> &types) {
  static const auto visitor = overloaded{
      [](std::monostate) -> std::string {
        throw CodeGenerateException(ErrType::INVALID_INPUT,
                                    "Invalid type for LLVM-IR style IO format");
      },
      [](BasicType basic) -> std::string {
        using std::literals::operator""s;
        switch (basic) {
        case BasicType::INTEGER:
          return "%d"s;
        case BasicType::REAL:
          return "%lf"s;
        case BasicType::BOOLEAN:
          return "%d"s;
        case BasicType::CHAR:
          return "%c"s;
        case BasicType::STRING:
          return "%s"s;
        default:
          assert(false && "Unhandled BasicType");
        }
      },
      [this](const SymbolType::Array &array) -> std::string {
        throw CodeGenerateException(
            ErrType::INVALID_INPUT,
            "Array type is not supported for LLVM-IR style IO format");
      },
      [this](const SymbolType::Record &record) -> std::string {
        throw CodeGenerateException(
            ErrType::INVALID_INPUT,
            "Record type is not supported for LLVM-IR style IO format");
      },
      [this](const SymbolType::Function &func) -> std::string {
        return getLLVMStyleIOFormatStr({func.return_type});
      },
      [this](const SymbolType::Procedure &proc) -> std::string {
        throw CodeGenerateException(
            ErrType::INVALID_INPUT,
            "Procedure type is not supported for LLVM-IR style IO format");
      },
      [](auto &&) -> std::string {
        throw CodeGenerateException(
            ErrType::INVALID_INPUT,
            "Unsupported type for LLVM-IR style IO format");
      } // 兜底
  };
  std::string formatStr;
  for (size_t i = 0; i < types.size(); ++i) {
    formatStr += types[i]->visit(visitor);
  }
  return formatStr;
}

inline void LLVMIrGenerator::storeSymbolName(const std::string &name) {
  auto record = std::make_unique<LLVMIRSymbolRecord>(name);
  record->setGlobal(m_stateStack.top() == State::Global);
  symbolTable->insert(std::move(record));
}
inline void LLVMIrGenerator::updateSymbolName(const std::string &name) {
  auto record = symbolTable->lookup(name);
  if (record == nullptr) {
    throw CodeGenerateException(ErrType::INVALID_INPUT,
                                "Symbol not found in symbol table");
  }
  if (record->isGlobal()) {
    // 不能更新全局变量的名字
    return;
  }
  record->setCurrentName(getUnNameIdStr());
}

inline std::string
LLVMIrGenerator::getCurrentSymbolName(const std::string &name) {
  auto record = symbolTable->lookup(name);
  if (record == nullptr) {
    throw CodeGenerateException(ErrType::INVALID_INPUT,
                                "Symbol not found in symbol table");
  }
  if (record->isGlobal()) {
    return std::format("@{}", record->getCurrentName());
  }
  return std::format("%{}", record->getCurrentName());
}

impl(TerminalNode) {
  if (node.isRelOp()) {
    m_outputBuffer.write(std::format("{} ", relop2LLVMStr(node.getValStr())));
    return;
  }
  if (node.isMulOp()) {
    m_outputBuffer.write(std::format("{} ", mulop2LLVMStr(node.getValStr())));
    return;
  }
  m_outputBuffer.write(std::format("{} ", node.getValStr()));
}
impl(ProgramStructNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProgramStructNode should not be visited");
}
impl(ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot) {
  m_outputBuffer.enterSection(Section::TARGET_SPEC);
  m_outputBuffer.writeln(
      "; Generated LLVM IR by The King Of Pascal Compiler(kpc)");
  // macosx
  m_outputBuffer.writeln(
      "target datalayout = \"e-m:o-i64:64-f80:128-n8:16:32:64-S128\"");
  m_outputBuffer.writeln("target triple = \"x86_64-apple-macosx10.15.0\"");
  m_outputBuffer.writeln();
  // linux
  m_outputBuffer.writeln(
      "; target datalayout = "
      "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
      "S128\"");
  m_outputBuffer.writeln("; target triple = \"x86_64-unknown-linux-gnu\"");
  m_outputBuffer.writeln();
  // windows
  m_outputBuffer.writeln(
      "; target datalayout = \"e-m:e-i64:64-f80:128-n8:16:32:64-S128\"");
  m_outputBuffer.writeln("; target triple = \"x86_64-pc-windows-msvc\"");
  m_outputBuffer.writeln();
  m_outputBuffer.exitSection();

  m_outputBuffer.enterSection(Section::FUNC_DECLS);
  m_outputBuffer.writeln("; program struct");
  m_outputBuffer.writeln("declare i32 @printf(i8*, ...)");
  m_outputBuffer.writeln("declare i32 @scanf(i8*, ...)");
  m_outputBuffer.exitSection();

  m_outputBuffer.enterSection(Section::TYPE_DECLS);
  node.getProgramHead()->accept(*this);
  node.getSemicolon()->accept(*this);
  node.getProgramBody()->accept(*this);
  m_outputBuffer.exitSection();

  m_outputBuffer.enterSection(Section::ATTRIBUTE_GROUPS);
  m_outputBuffer.writeln(
      "; attributes #0 = { noinline nounwind optnone ssp uwtable "
      "\"correctly-rounded-divide-sqrt-fp-math\"=\"false\" "
      "\"darwin-stkchk-strong-link\" \"disable-tail-calls\"=\"false\" "
      "\"frame-pointer\"=\"all\" \"less-precise-fpmad\"=\"false\" "
      "\"min-legal-vector-width\"=\"0\" \"no-infs-fp-math\"=\"false\" "
      "\"no-jump-tables\"=\"false\" \"no-nans-fp-math\"=\"false\" "
      "\"no-signed-zeros-fp-math\"=\"false\" "
      "\"no-trapping-math\"=\"false\" \"probe-stack\"=\"___chkstk_darwin\" "
      "\"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"penryn\" "
      "\"target-features\"=\"+cx16,+cx8,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+"
      "sse4.1,+ssse3,+x87\" \"unsafe-fp-math\"=\"false\" "
      "\"use-soft-float\"=\"false\" }");
  m_outputBuffer.exitSection();
  m_outputBuffer.enterSection(Section::NAMED_METADATA);
  m_outputBuffer.writeln("!llvm.module.flags = !{!0, !1, !2, !3}");
  m_outputBuffer.writeln("!llvm.ident = !{!4}");
  m_outputBuffer.exitSection();
  m_outputBuffer.enterSection(Section::METADATA_NODES);
  m_outputBuffer.writeln("!0 = !{i32 1, !\"wchar_size\", i32 4}");
  m_outputBuffer.writeln("!1 = !{i32 8, !\"PIC Level\", i32 2}");
  m_outputBuffer.writeln("!2 = !{i32 7, !\"PIE Level\", i32 2}");
  m_outputBuffer.writeln("!3 = !{i32 7, !\"uwtable\", i32 2}");
  // TODO: 这里的 clang 版本号需要改成自己的
  m_outputBuffer.writeln("!4 = !{!\"Homebrew clang version 16.0.6\"}");
  m_outputBuffer.writeln("\n; generate code end, enjoy it");
  m_outputBuffer.exitSection();
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
impl(ProgramHeadNode_Program_Id) {
  m_stateStack.push(State::NORMAL);
  m_outputBuffer.setSourceFileName(node.getId()->getValStr());
  m_stateStack.pop();
}

impl(ProgramBodyNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProgramBodyNode should not be visited");
}

impl(ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement) {
  m_stateStack.push(State::Global);
  m_outputBuffer.enterSection(Section::GLOBAL_CONST_DECLS);
  node.getConstDecls()->accept(*this);
  m_outputBuffer.exitSection();

  m_outputBuffer.enterSection(Section::GLOBAL_VAR_DECLS);
  node.getVarDecls()->accept(*this);
  m_outputBuffer.exitSection();
  m_stateStack.pop();

  m_stateStack.push(State::FunctionDef);
  m_outputBuffer.enterSection(Section::FUNC_DEFS);
  node.getSubprogramDecls()->accept(*this);
  m_stateStack.pop();
  //   main func
  m_outputBuffer.writeln("\ndefine dso_local i32 @main() {\n");
  node.getCompoundStatement()->accept(*this);
  m_outputBuffer.writeln("  ret i32 0\n");
  m_outputBuffer.writeln("}\n");
  m_outputBuffer.exitSection();
}

impl(IdListNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "IdListNode should not be visited");
}
impl(IdListNode_Id) { node.getId()->accept(*this); }

impl(IdListNode_IdList_Comma_Id) {
  node.getIdList()->accept(*this);
  m_outputBuffer.writeln(", ");
  node.getComma()->accept(*this);
  node.getId()->accept(*this);
}

impl(ConstDeclsNode) { m_outputBuffer.writeln("; const decls is empty"); }
impl(ConstDeclsNode_Const_ConstDecl) {
  m_outputBuffer.writeln("; const decls");
  node.getConstDecl()->accept(*this);
}

impl(ConstDeclNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ConstDeclNode should not be visited");
}

impl(ConstDeclNode_Id_Relop_ConstVal_Semicolon) {
  std::string name = node.getId()->getValStr();
  auto type = node.getConstVal()->getType();
  m_outputBuffer.write(std::format("@{} = private constant ", name));
  storeSymbolName(name);
  node.getConstVal()->accept(*this);
  m_outputBuffer.writeln();
}
impl(ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon) {
  node.getConstDecl()->accept(*this);
  std::string name = node.getId()->getValStr();
  m_outputBuffer.write(std::format("@{} = private constant ", name));
  storeSymbolName(name);
  node.getConstVal()->accept(*this);
  m_outputBuffer.writeln();
}

impl(ConstValNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ConstValNode should not be visited");
}
impl(ConstValNode_Plus_Number) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.write(std::format("{} +", typeStr));
  node.getNumber()->accept(*this);
}
impl(ConstValNode_Minus_Number) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.write(std::format("{} -", typeStr));
  node.getNumber()->accept(*this);
}
impl(ConstValNode_Number) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.write(std::format("{} ", typeStr));
  node.getNumber()->accept(*this);
}
impl(ConstValNode_StringLiteral) {
  auto str = node.getStringLiteral()->getValStr();
  // TODO: 也许要加个 global output buffer 才能处理字符串constant
  // [12 x i8] c"Hello world\00"
  m_outputBuffer.write(
      std::format("[{} x i8] c\"{}\\00\"\n", str.size() + 1, str));
}
impl(ConstValNode_CharLiteral) {
  m_outputBuffer.write("i8 ");
  node.getCharLiteral()->accept(*this);
}

impl(TypeNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "TypeNode should not be visited");
}
impl(TypeNode_BasicType) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.writeln(std::format("{} ", typeStr));
}

impl(TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.writeln(std::format("{} ", typeStr));
  node.getPeriod()->accept(*this);
  m_outputBuffer.writeln(" ");
  node.getBasicType()->accept(*this);
}
impl(TypeNode_String) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.writeln(std::format("{} ", typeStr));
}

impl(BasicTypeNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "BasicTypeNode should not be visited");
}
impl(BasicTypeNode_Integer) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.writeln(std::format("{} ", typeStr));
}
impl(BasicTypeNode_Real) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.writeln(std::format("{} ", typeStr));
}
impl(BasicTypeNode_Boolean) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.writeln(std::format("{} ", typeStr));
}
impl(BasicTypeNode_Char) {
  auto type = node.getType();
  auto typeStr = symbolType2LLVMStr(*type);
  m_outputBuffer.writeln(std::format("{} ", typeStr));
}

impl(PeriodNode) {}
impl(PeriodNode_Number_Dot_Dot_Number) {}
impl(PeriodNode_Period_Comma_Number_Dot_Dot_Number) {}

impl(VarDeclsNode) { m_outputBuffer.writeln("\n; var decls is empty"); }
impl(VarDeclsNode_Var_VarDecl_Semicolon) {
  m_outputBuffer.writeln("\n; var decls");
  node.getVarDecl()->accept(*this);
}

impl(VarDeclNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "VarDeclNode should not be visited");
}
void LLVMIrGenerator::g_IdList_Type(std::shared_ptr<IdListNode> idList,
                                    std::shared_ptr<TypeNode> typeNode) {
  auto type = typeNode->getType();
  auto typeStr = symbolType2LLVMStr(*type);
  auto ids = idList->getAllIds();
  if (type->is_array()) {
    // 数组类型
    // 现在 typeStr 是 [range1][range2]...[rangeN]type 的格式
    // get basic type str and rangeStr

    std::string rangeStr = typeStr.substr(0, typeStr.find_last_of(']') + 1);
    typeStr = typeStr.substr(typeStr.find_last_of(']') + 1);

    for (const auto &id : ids) {
      m_outputBuffer.writeln(
          std::format("{} {}{};\n", typeStr, id->getValStr(), rangeStr));
    }
    return;
  }

  for (const auto &id : ids) {
    m_outputBuffer.writeln(std::format("@{} = private global {} {};\n",
                                       id->getValStr(), typeStr,
                                       getDefaultValue(*type)));
  }
}

impl(VarDeclNode_IdList_Colon_Type) {
  g_IdList_Type(node.getIdList(), node.getType());
}
impl(VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type) {
  node.getVarDecl()->accept(*this);
  g_IdList_Type(node.getIdList(), node.getType());
}

impl(SubprogramDeclsNode) {}
impl(SubprogramDeclsNode_SubprogramDecls_Subprogram) {}

impl(SubprogramNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "SubprogramNode should not be visited");
}

impl(SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON) {
  m_outputBuffer.enterSection(Section::FUNC_DECLS);

  m_outputBuffer.writeln("; subprogram head");
  node.getSubprogramHead()->accept(*this);
  m_outputBuffer.writeln("{");
  m_outputBuffer.writeln("; subprogram body");
  node.getSubprogramBody()->accept(*this);
  m_outputBuffer.writeln("; subprogram end");
  m_outputBuffer.writeln("}\n");

  m_outputBuffer.exitSection();
}

impl(SubprogramHeadNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "SubprogramHeadNode should not be visited");
}
impl(SubprogramHeadNode_Procedure_Id_FormalParameter) {
  unname_id_num = 0; // 重新计数
}

impl(SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType) {
  unname_id_num = 0; // 重新计数
}

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

impl(CompoundStatementNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "CompoundStatementNode should not be visited");
}
impl(CompoundStatementNode_Begin_StatementList_End) {
  symbolTable->enterBlock();
  node.getStatementList()->accept(*this);
  symbolTable->exitBlock();
}

impl(StatementListNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "StatementListNode should not be visited");
}
impl(StatementListNode_Statement) {
  node.getStatement()->accept(*this);
  m_outputBuffer.writeln();
}
impl(StatementListNode_StatementList_Semicolon_Statement) {
  node.getStatementList()->accept(*this);
  node.getStatement()->accept(*this);
}

impl(StatementNode) { m_outputBuffer.writeln("; null"); }
impl(StatementNode_Variable_Assignop_Expression) {}
impl(StatementNode_Id_Assignop_Expression) {}
impl(StatementNode_ProcedureCall) {}
impl(StatementNode_If_Expression_Then_Statement_ElsePart) {}
impl(

    StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement) {}
impl(StatementNode_While_Expression_Do_Statement) {}
impl(StatementNode_Read_Lparen_VariableList_Rparen) {
  auto types = node.getVariableList()->getTypeList();
  auto fmtStr = getLLVMStyleIOFormatStr(types);
  auto fmtStrName = getFmtStrName(fmtStr);
  if (m_FmtStrSet.count(fmtStrName) == 0) {
    // declare fmt string in global
    m_outputBuffer.enterSection(Section::GLOBAL_CONST_DECLS);
    m_outputBuffer.writeln(
        std::format("@{} = private constant [{} x i8] c\"{}\\00\"\n",
                    fmtStrName, fmtStr.size() + 1, fmtStr));
    m_FmtStrSet.insert(fmtStrName);
    m_outputBuffer.exitSection();
  }
  auto localFmtStrName = getUnNameIdStr();
  m_outputBuffer.write(
      std::format("call i32 (i8*, ...) @scanf(i8* @{}, ", fmtStrName));
  m_stateStack.push(State::Scanf);
  node.getVariableList()->accept(*this);
  m_stateStack.pop();
  m_outputBuffer.writeln(")");
}
impl(StatementNode_Write_Lparen_ExpressionList_Rparen) {
  auto types = node.getExpressionList()->getTypeList();
  auto fmtStr = getLLVMStyleIOFormatStr(types);
  auto fmtStrName = getFmtStrName(fmtStr);
  if (m_FmtStrSet.count(fmtStrName) == 0) {
    // declare fmt string in global
    m_outputBuffer.enterSection(Section::GLOBAL_CONST_DECLS);
    m_outputBuffer.writeln(
        std::format("@{} = private constant [{} x i8] c\"{}\\00\"\n",
                    fmtStrName, fmtStr.size() + 1, fmtStr));
    m_FmtStrSet.insert(fmtStrName);
    m_outputBuffer.exitSection();
  }
  auto localFmtStrName = getUnNameIdStr();
  m_outputBuffer.write(
      std::format("call i32 (i8*, ...) @printf(i8* @{}, ", fmtStrName));
  m_stateStack.push(State::Printf);
  node.getExpressionList()->accept(*this);
  m_stateStack.pop();
  m_outputBuffer.writeln(")");
}
impl(StatementNode_CompoundStatement) {}
impl(StatementNode_Break) {}
impl(StatementNode_Continue) {}

impl(VariableListNode) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "VariableListNode should not be visited");
}
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
