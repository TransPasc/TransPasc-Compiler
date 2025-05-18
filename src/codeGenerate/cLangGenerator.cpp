#include "codeGenerate/cLangGenerator.hpp"
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
  symbolTable = std::make_shared<StackLinkedSymbolTable>();
  //   默认 block
  symbolTable->enterBlock();

  m_stateStack.push(State::NORMAL);

  std::cout << "Generating code..." << std::endl;
  root->accept(*this);
  symbolTable->exitBlock();

  //   将生成的代码写入文件
  if (m_outputFile.empty()) {
    // 如果没有设置输出文件，则打印到控制台
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
void CLangGenerator::visit(class TerminalNode &node) {
  // 处理终结符节点
  if (node.isRelOp()) {
    m_outputBuffer += std::format("{} ", relop2cStyle(node.getValStr()));
    return;
  }
  if (node.isMulOp()) {
    m_outputBuffer += std::format("{} ", mulop2cStyle(node.getValStr()));
    return;
  }
  m_outputBuffer += std::format("{} ", node.getValStr());
}

void CLangGenerator::visit(class ProgramStructNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProgramStructNode should not be visited");
};
void CLangGenerator::visit(
    class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node) {
  // 处理程序结构节点
  writeln("// Generated C code by Pascal Compiler");
  writeln("#include <stdio.h>");
  writeln("#include <stdlib.h>");
  writeln("#include <stdbool.h>");
  node.getProgramHead()->accept(*this);
  node.getSemicolon()->accept(*this);
  node.getProgramBody()->accept(*this);
};

void CLangGenerator::visit(class ProgramHeadNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProgramHeadNode should not be visited");
};
void CLangGenerator::visit(
    class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen &node) {
  // TODO:暂不支持
  throw CodeGenerateException(
      ErrType::UNREACH_CODE,
      "ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen should not be visited, "
      "because we don't support this");
};
void CLangGenerator::visit(class ProgramHeadNode_Program_Id &node) {
  // nothing need to do
};

void CLangGenerator::visit(class ProgramBodyNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProgramBodyNode should not be visited");
};
void CLangGenerator::visit(
    class ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
        &node) {

  node.getConstDecls()->accept(*this);
  node.getVarDecls()->accept(*this);
  node.getSubprogramDecls()->accept(*this);
  //   main func
  m_outputBuffer += "int main() {\n";
  node.getCompoundStatement()->accept(*this);
  m_outputBuffer += "}\n";
};

void CLangGenerator::visit(class IdListNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "IdListNode should not be visited");
};
void CLangGenerator::visit(class IdListNode_Id &node) {
  node.getId()->accept(*this);
};
void CLangGenerator::visit(class IdListNode_IdList_Comma_Id &node) {
  node.getIdList()->accept(*this);
  node.getComma()->accept(*this);
  node.getId()->accept(*this);
};

void CLangGenerator::visit(class ConstDeclsNode &node) {
  writeln("// no const decls");
};
void CLangGenerator::visit(class ConstDeclsNode_Const_ConstDecl &node) {
  writeln("// const decls");
  node.getConstDecl()->accept(*this);
};

void CLangGenerator::visit(class ConstDeclNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ConstDeclNode should not be visited");
};
void CLangGenerator::visit(
    class ConstDeclNode_Id_Relop_ConstVal_Semicolon &node) {
  auto type = node.getConstVal()->getType();
  std::string buf = std::format("const {} {} = ", symbolType2Str(*type),
                                node.getId()->get<std::string>());
  m_outputBuffer += buf;
  node.getConstVal()->accept(*this);
  m_outputBuffer += ";\n";
};
void CLangGenerator::visit(
    class ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon &node) {
  node.getConstDecl()->accept(*this);

  auto type = node.getConstVal()->getType();
  m_outputBuffer += std::format("const {} {} = ", symbolType2Str(*type),
                                node.getId()->get<std::string>());

  node.getConstVal()->accept(*this);
  m_outputBuffer += ";\n";
};

void CLangGenerator::visit(class ConstValNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ConstValNode should not be visited");
};
void CLangGenerator::visit(class ConstValNode_Plus_Number &node) {
  m_outputBuffer += std::format("+{}", node.getNumber()->getValStr());
};
void CLangGenerator::visit(class ConstValNode_Minus_Number &node) {
  m_outputBuffer += std::format("-{}", node.getNumber()->getValStr());
};
void CLangGenerator::visit(class ConstValNode_Number &node) {
  m_outputBuffer += std::format("{}", node.getNumber()->getValStr());
};
void CLangGenerator::visit(class ConstValNode_StringLiteral &node) {
  m_outputBuffer += std::format("\"{}\"", node.getStringLiteral()->getValStr());
};
void CLangGenerator::visit(class ConstValNode_CharLiteral &node) {
  m_outputBuffer += std::format("{}", node.getCharLiteral()->getValStr());
};

void CLangGenerator::visit(class TypeNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "TypeNode should not be visited");
};
void CLangGenerator::visit(class TypeNode_BasicType &node) {
  //   nothing need to do
};
void CLangGenerator::visit(class TypeNode_String &node) {
  //   nothing need to do
};
void CLangGenerator::visit(
    class TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType &node) {
  //   c 语言数组是 type id[][]
  //   不好直接在这里生成代码
  //   在上层处理好了
};

void CLangGenerator::visit(class BasicTypeNode &node) {
  // nothing need to do
};
void CLangGenerator::visit(class BasicTypeNode_Integer &node) {
  // nothing need to do
};
void CLangGenerator::visit(class BasicTypeNode_Real &node) {
  // nothing need to do
};
void CLangGenerator::visit(class BasicTypeNode_Boolean &node) {
  // nothing need to do
};
void CLangGenerator::visit(class BasicTypeNode_Char &node) {
  // nothing need to do
};

void CLangGenerator::visit(class PeriodNode &node) {
  // nothing need to do
};
void CLangGenerator::visit(class PeriodNode_Number_Dot_Dot_Number &node) {
  // nothing need to do
};
void CLangGenerator::visit(
    class PeriodNode_Period_Comma_Number_Dot_Dot_Number &node) {
  // nothing need to do
};

void CLangGenerator::visit(class VarDeclsNode &node) {
  writeln("// no var decls");
};
void CLangGenerator::visit(class VarDeclsNode_Var_VarDecl_Semicolon &node) {
  writeln("// var decls");
  node.getVarDecl()->accept(*this);
};

void CLangGenerator::visit(class VarDeclNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "VarDeclNode should not be visited");
};
void CLangGenerator::g_IdList_Type(std::shared_ptr<IdListNode> idListNode,
                                   std::shared_ptr<TypeNode> typeNode) {
  auto type = typeNode->getType();
  auto typeStr = symbolType2Str(*type);
  auto ids = idListNode->getAllIds();
  if (type->is_array()) {
    // 数组类型
    // 现在 typeStr 是 [range1][range2]...[rangeN]type 的格式
    // get basic type str and rangeStr

    std::string rangeStr = typeStr.substr(0, typeStr.find_last_of(']') + 1);
    typeStr = typeStr.substr(typeStr.find_last_of(']') + 1);

    for (const auto &id : ids) {
      m_outputBuffer +=
          std::format("{} {}{};\n", typeStr, id->getValStr(), rangeStr);
    }
    return;
  }
  for (const auto &id : ids) {
    m_outputBuffer += std::format("{} {};\n", typeStr, id->getValStr());
  }
}

void CLangGenerator::visit(class VarDeclNode_IdList_Colon_Type &node) {
  // 调用辅助函数
  g_IdList_Type(node.getIdList(), node.getType());
};
void CLangGenerator::visit(
    class VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type &node) {
  node.getVarDecl()->accept(*this);
  g_IdList_Type(node.getIdList(), node.getType());
};

void CLangGenerator::visit(class SubprogramDeclsNode &node) {
  // nothing need to do
};
void CLangGenerator::visit(
    class SubprogramDeclsNode_SubprogramDecls_Subprogram &node) {
  node.getSubprogramDecls()->accept(*this);
  writeln("// subprogram decls");
  node.getSubprogram()->accept(*this);
};

void CLangGenerator::visit(class SubprogramNode &node) {
  writeln("// subprogram node");
};
void CLangGenerator::visit(
    class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON
        &node) {
  // 处理子程序节点, 在 head 中 enter block
  node.getSubprogramHead()->accept(*this);
  m_outputBuffer += "{\n/* subprogram body */\n";
  node.getSubprogramBody()->accept(*this);
  symbolTable->exitBlock();
  m_outputBuffer += "}\n";
};

void CLangGenerator::visit(class SubprogramHeadNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "SubprogramHeadNode should not be visited");
};
void CLangGenerator::visit(
    class SubprogramHeadNode_Procedure_Id_FormalParameter &node) {
  // 将 func id 加入符号表
  auto params = node.getFormalParameter()->getParams();
  auto type = SymbolType::MakeFunction(nullptr, params);
  auto record = std::make_unique<SymbolRecord>(node.getId()->getValStr());
  record->setType(std::make_shared<SymbolType>(type));
  symbolTable->insert(std::move(record));
  // void func
  m_outputBuffer += std::format("void {}(", node.getId()->getValStr());
  symbolTable->enterBlock();
  node.getFormalParameter()->accept(*this);
  m_outputBuffer += ")";
};
void CLangGenerator::visit(
    class SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType
        &node) {
  m_stateStack.push(State::FunctionDef);
  // 将 func id 加入符号表
  auto params = node.getFormalParameter()->getParams();
  auto type = SymbolType::MakeFunction(nullptr, params);
  auto record = std::make_unique<SymbolRecord>(node.getId()->getValStr());
  record->setType(std::make_shared<SymbolType>(type));
  symbolTable->insert(std::move(record));
  // retType func
  m_outputBuffer +=
      std::format("{} {}(", symbolType2Str(*node.getBasicType()->getType()),
                  node.getId()->getValStr());
  symbolTable->enterBlock();
  node.getFormalParameter()->accept(*this);
  m_outputBuffer += ")";
  m_returnTypeStack.push(node.getBasicType()->getType());
};

void CLangGenerator::visit(class FormalParameterNode &node) {
  m_outputBuffer += "/* no params */";
};
void CLangGenerator::visit(
    class FormalParameterNode_Lparen_ParameterList_Rparen &node) {
  node.getParameterList()->accept(*this);
};

void CLangGenerator::visit(class ParameterListNode &node) {
  // do nothing
};
void CLangGenerator::visit(class ParameterListNode_Parameter &node) {
  node.getParameter()->accept(*this);
};
void CLangGenerator::visit(
    class ParameterListNode_ParameterList_Semicolon_Parameter &node) {
  node.getParameterList()->accept(*this);
  m_outputBuffer += ", ";
  node.getParameter()->accept(*this);
};

void CLangGenerator::visit(class ParameterNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ParameterNode should not be visited");
};
void CLangGenerator::visit(class ParameterNode_VarParameter &node) {
  m_isRefParam = true;
  node.getVarParameter()->accept(*this);
  m_isRefParam = false;
};
void CLangGenerator::visit(class ParameterNode_ValueParameter &node) {
  node.getValueParameter()->accept(*this);
};

void CLangGenerator::visit(class VarParameterNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "VarParameterNode should not be visited");
};
void CLangGenerator::visit(class VarParameterNode_Var_ValueParameter &node) {

  node.getValueParameter()->accept(*this);
};

void CLangGenerator::visit(class ValueParameterNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ValueParameterNode should not be visited");
};
void CLangGenerator::visit(
    class ValueParameterNode_IdList_Colon_BasicType &node) {
  auto type = node.getBasicType()->getType();
  auto ids = node.getIdList()->getAllIds();
  auto typeStr = symbolType2Str(*type);
  if (m_isRefParam) {
    // 引用参数
    typeStr = std::format("{} *", typeStr);
  }
  for (const auto &id : ids) {
    // 将 id 加入符号表
    auto record = std::make_unique<SymbolRecord>(id->getValStr());
    auto type = std::make_shared<SymbolType>();
    if (m_isRefParam)
      type->set_ref();
    record->setType(type);
    symbolTable->insert(std::move(record));

    m_outputBuffer += std::format("{} {}", typeStr, id->getValStr());
    if (id != ids.back()) {
      m_outputBuffer += ", ";
    }
  }
};

void CLangGenerator::visit(class SubprogramBodyNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "SubprogramBodyNode should not be visited");
};
void CLangGenerator::visit(
    class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement &node) {
  // define FUNC_RES
  if (m_stateStack.top() == State::FunctionDef) {
    auto type = m_returnTypeStack.top();
    m_outputBuffer +=
        std::format("{} {} = 0;\n", symbolType2Str(*type), FUNC_RES);
    m_returnTypeStack.pop();
  }

  node.getConstDecls()->accept(*this);
  node.getVarDecls()->accept(*this);
  node.getCompoundStatement()->accept(*this);
  //   处理函数返回值
  if (m_stateStack.top() == State::FunctionDef) {
    m_outputBuffer += std::format("return {};\n", FUNC_RES);
    m_stateStack.pop();
  }
};

void CLangGenerator::visit(class CompoundStatementNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "CompoundStatementNode should not be visited");
};
void CLangGenerator::visit(
    class CompoundStatementNode_Begin_StatementList_End &node) {
  // 处理复合语句节点
  m_outputBuffer += "{\n";
  node.getStatementList()->accept(*this);
  m_outputBuffer += "}\n";
};

void CLangGenerator::visit(class StatementListNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "StatementListNode should not be visited");
};
void CLangGenerator::visit(class StatementListNode_Statement &node) {
  // 处理语句列表节点
  node.getStatement()->accept(*this);
};
void CLangGenerator::visit(
    class StatementListNode_StatementList_Semicolon_Statement &node) {
  node.getStatementList()->accept(*this);
  m_outputBuffer += ";\n";
  node.getStatement()->accept(*this);
};

void CLangGenerator::visit(class StatementNode &node) { writeln("// null"); };
void CLangGenerator::visit(
    class StatementNode_Variable_Assignop_Expression &node) {
  // 处理赋值语句节点
  node.getVariable()->accept(*this);
  m_outputBuffer += " = ";
  node.getExpression()->accept(*this);
  m_outputBuffer += ";\n";
};
void CLangGenerator::visit(class StatementNode_Id_Assignop_Expression &node) {
  // 处理赋值语句节点
  auto record = symbolTable->lookup(node.getId()->getValStr());
  // 处理 return 语句
  if (record && record->getType() && record->getType()->is_function()) {
    m_outputBuffer += std::format("{} = ", FUNC_RES);
    node.getExpression()->accept(*this);
    m_outputBuffer += ";\n";
    return;
  }
  //   处理需不需要解引用
  if (record && record->getType() && record->getType()->is_ref_type()) {
    m_outputBuffer += std::format("*{} = ", node.getId()->getValStr());

  } else {
    m_outputBuffer += std::format("{} = ", node.getId()->getValStr());
  }
  node.getExpression()->accept(*this);
  m_outputBuffer += ";\n";
};
void CLangGenerator::visit(class StatementNode_ProcedureCall &node) {
  // 处理过程调用语句节点
  m_outputBuffer += "/* procedure call */\n";
  node.getProcedureCall()->accept(*this);
  m_outputBuffer += ";\n";
};
void CLangGenerator::visit(
    class StatementNode_If_Expression_Then_Statement_ElsePart &node) {
  // 处理条件语句节点
  m_outputBuffer += "if (";
  node.getExpression()->accept(*this);
  m_outputBuffer += ") {\n";
  node.getStatement()->accept(*this);
  m_outputBuffer += "}\n";
  node.getElsePart()->accept(*this);
};
void CLangGenerator::visit(
    class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
        &node) {
  // 处理for循环语句节点

  // init
  m_outputBuffer += std::format("for ({} = ", node.getId()->getValStr());
  node.getExpression1()->accept(*this);
  m_outputBuffer += "; ";

  // condition
  m_outputBuffer += std::format("{} <= ", node.getId()->getValStr());
  node.getExpression2()->accept(*this);
  m_outputBuffer += "; ";

  // increment
  m_outputBuffer += std::format("{}++", node.getId()->getValStr());

  // statement
  m_outputBuffer += ") {\n";
  node.getStatement()->accept(*this);
  m_outputBuffer += "}\n";
};
void CLangGenerator::visit(
    class StatementNode_While_Expression_Do_Statement &node) {
  // 处理while循环语句节点
  m_outputBuffer += "while (";
  node.getExpression()->accept(*this);
  m_outputBuffer += ") {\n";
  node.getStatement()->accept(*this);
  m_outputBuffer += "}\n";
};

void CLangGenerator::visit(
    class StatementNode_Read_Lparen_VariableList_Rparen &node) {
  auto types = node.getVariableList()->getTypeList();
  auto fmtStr = getCStyleIOFormatStr(types);
  m_outputBuffer += std::format("scanf(\"{}\", ", fmtStr);
  m_stateStack.push(State::Scanf);
  node.getVariableList()->accept(*this);
  m_stateStack.pop();
  m_outputBuffer += ");\n";
};

void CLangGenerator::visit(
    class StatementNode_Write_Lparen_ExpressionList_Rparen &node) {
  /**
   * From: $Statement ::= $Write ( $ExpressionList )
   * To: printf(format, $ExpressionList);
   */
  auto types = node.getExpressionList()->getTypeList();
  auto fmtStr = getCStyleIOFormatStr(types);
  m_outputBuffer += std::format("printf(\"{}\", ", fmtStr);
  node.getExpressionList()->accept(*this);
  m_outputBuffer += ");\n";
}

void CLangGenerator::visit(class StatementNode_CompoundStatement &node) {
  // 处理复合语句节点
  m_outputBuffer += "{\n";
  node.getCompoundStatement()->accept(*this);
  m_outputBuffer += "}\n";
};
void CLangGenerator::visit(class StatementNode_Break &node) {
  // 处理break语句节点
  m_outputBuffer += "break;\n";
};
void CLangGenerator::visit(class StatementNode_Continue &node) {
  // 处理continue语句节点
  m_outputBuffer += "continue;\n";
};

void CLangGenerator::visit(class VariableListNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "VariableListNode should not be visited");
};
void CLangGenerator::visit(class VariableListNode_Variable &node) {
  node.getVariable()->accept(*this);
};
void CLangGenerator::visit(
    class VariableListNode_VariableList_Comma_Variable &node) {
  node.getVariableList()->accept(*this);
  m_outputBuffer += ", ";
  node.getVariable()->accept(*this);
};

void CLangGenerator::visit(class VariableNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "VariableNode should not be visited");
};
void CLangGenerator::visit(class VariableNode_Id_IdVarpart &node) {
  // 处理变量节点
  //   auto record = symbolTable->lookup(node.getId()->getValStr());
  auto type = node.getType();
  if (m_stateStack.top() == State::Scanf) {
    if (type->is_function())
      m_outputBuffer += std::format("&{}", FUNC_RES);
    else
      m_outputBuffer += std::format("&{}", node.getId()->getValStr());
    return node.getIdVarpart()->accept(*this);
  }
  if (type->is_function() || type->is_procedure()) {
    m_outputBuffer += std::format("{}()", node.getId()->getValStr());
    return node.getIdVarpart()->accept(*this);
  }
  if (type->is_ref_type()) {
    m_outputBuffer += std::format("*{}", node.getId()->getValStr());
    return node.getIdVarpart()->accept(*this);
  }

  m_outputBuffer += std::format("{}", node.getId()->getValStr());
  return node.getIdVarpart()->accept(*this);
};

void CLangGenerator::visit(class IdVarPartNode &node) {
  // nothing need to do
};
void CLangGenerator::visit(
    class IdVarPartNode_Lbracket_ExpressionList_Rbracket &node) {
  // 处理数组下标
  m_outputBuffer += "[";
  m_stateStack.push(State::IdVarPart);
  node.getExpressionList()->accept(*this);
  m_stateStack.pop();
  m_outputBuffer += "]";
};

void CLangGenerator::visit(class ProcedureCallNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ProcedureCallNode should not be visited");
};
void CLangGenerator::visit(class ProcedureCallNode_Id &node) {
  // 处理过程调用节点
  m_outputBuffer += std::format("{}();", node.getId()->getValStr());
};
void CLangGenerator::visit(class ProcedureCallNode_Id_Lparen_Rparen &node) {
  // 处理过程调用节点
  m_outputBuffer += std::format("{}();", node.getId()->getValStr());
};
void CLangGenerator::visit(
    class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen &node) {
  auto record = symbolTable->lookup(node.getId()->getValStr());
  auto type = record->getType();
  auto procedure = type->get_if<SymbolType::Function>();
  auto func = type->get_if<SymbolType::Procedure>();

  m_stateStack.push(State::FunctionCall);
  m_paramsStack.push(
      {procedure ? procedure->param_types : func->param_types, 0});

  m_outputBuffer += std::format("{}(", node.getId()->getValStr());
  node.getExpressionList()->accept(*this);
  m_outputBuffer += ");\n";

  m_paramsStack.pop();
  m_stateStack.pop();
};

void CLangGenerator::visit(class ElsePartNode &node) {
  m_outputBuffer += "/* no else part */";
};
void CLangGenerator::visit(class ElsePartNode_Else_Statement &node) {

  m_outputBuffer += "else {\n";
  node.getStatement()->accept(*this);
  m_outputBuffer += "}\n";
};

void CLangGenerator::visit(class ExpressionListNode &node) {
  // do nothing
};

void CLangGenerator::visit(class ExpressionListNode_Expression &node) {
  if (m_stateStack.top() == State::FunctionCall) {
    // func call
    auto param = m_paramsStack.top().first[m_paramsStack.top().second];
    auto type = param->first;
    if (type.is_ref_type()) {
      m_outputBuffer += "&";
    }
  }
  // 处理表达式列表节点
  node.getExpression()->accept(*this);
  if (m_stateStack.top() == State::FunctionCall)
    m_paramsStack.top().second++;
};
void CLangGenerator::visit(
    class ExpressionListNode_ExpressionList_Comma_Expression &node) {

  node.getExpressionList()->accept(*this);
  if (m_stateStack.top() == State::IdVarPart) {
    m_outputBuffer += "][";
  } else {
    m_outputBuffer += ", ";
  }
  if (m_stateStack.top() == State::FunctionCall) {
    // func call
    auto param = m_paramsStack.top().first[m_paramsStack.top().second];
    auto type = param->first;
    if (type.is_ref_type()) {
      m_outputBuffer += "&";
    }
  }
  node.getExpression()->accept(*this);
  if (m_stateStack.top() == State::FunctionCall)
    m_paramsStack.top().second++;
};

void CLangGenerator::visit(class ExpressionNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "ExpressionNode should not be visited");
};
void CLangGenerator::visit(class ExpressionNode_SimpleExpression &node) {
  // 处理简单表达式节点
  node.getSimpleExpression()->accept(*this);
};
void CLangGenerator::visit(
    class ExpressionNode_SimpleExpression_Relop_SimpleExpression &node) {
  // 处理关系表达式节点
  m_outputBuffer += "(";
  node.getSimpleExpression1()->accept(*this);
  m_outputBuffer += " ";
  node.getRelop()->accept(*this);
  m_outputBuffer += " ";
  node.getSimpleExpression2()->accept(*this);
  m_outputBuffer += ")";
};

void CLangGenerator::visit(class SimpleExpressionNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "SimpleExpressionNode should not be visited");
};
void CLangGenerator::visit(class SimpleExpressionNode_Term &node) {
  node.getTerm()->accept(*this);
};
void CLangGenerator::visit(
    class SimpleExpressionNode_SimpleExpression_Plus_Term &node) {
  // 处理加法运算符
  // m_outputBuffer += "(";
  node.getSimpleExpression()->accept(*this);
  m_outputBuffer += " + ";
  node.getTerm()->accept(*this);
  // m_outputBuffer += ")";
};
void CLangGenerator::visit(
    class SimpleExpressionNode_SimpleExpression_Minus_Term &node) {
  // 处理减法运算符
  // m_outputBuffer += "(";
  node.getSimpleExpression()->accept(*this);
  m_outputBuffer += " - ";
  node.getTerm()->accept(*this);
  // m_outputBuffer += ")";
};
void CLangGenerator::visit(
    class SimpleExpressionNode_SimpleExpression_Or_Term &node) {
  // 处理或运算符
  m_outputBuffer += "(";
  node.getSimpleExpression()->accept(*this);
  m_outputBuffer += " || ";
  node.getTerm()->accept(*this);
  m_outputBuffer += ")";
};

void CLangGenerator::visit(class TermNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "TermNode should not be visited");
};
void CLangGenerator::visit(class TermNode_Factor &node) {
  node.getFactor()->accept(*this);
};
void CLangGenerator::visit(class TermNode_Term_Mulop_Factor &node) {
  // 处理乘法运算符
  // m_outputBuffer += "(";
  node.getTerm()->accept(*this);
  node.getMulop()->accept(*this);
  node.getFactor()->accept(*this);
  // m_outputBuffer += ")";
};

void CLangGenerator::visit(class FactorNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "FactorNode should not be visited");
};
void CLangGenerator::visit(class FactorNode_Number &node) {
  node.getNumber()->accept(*this);
};
void CLangGenerator::visit(class FactorNode_CharLiteral &node) {
  node.getCharLiteral()->accept(*this);
};
void CLangGenerator::visit(class FactorNode_BoolLiteral &node) {
  auto str = node.getBoolLiteral()->getValStr();
  // 转为小写
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  m_outputBuffer += str == "true" ? "1" : "0";
};
void CLangGenerator::visit(class FactorNode_Variable &node) {
  node.getVariable()->accept(*this);
};
void CLangGenerator::visit(class FactorNode_Lparen_Expression_Rparen &node) {
  // 处理括号表达式
  m_outputBuffer += "(";
  node.getExpression()->accept(*this);
  m_outputBuffer += ")";
};
void CLangGenerator::visit(class FactorNode_Not_Factor &node) {
  m_outputBuffer += "(";
  m_outputBuffer += "~";
  node.getFactor()->accept(*this);
  m_outputBuffer += ")";
};
void CLangGenerator::visit(class FactorNode_Plus_Factor &node) {
  m_outputBuffer += "(";
  m_outputBuffer += "+";
  node.getFactor()->accept(*this);
  m_outputBuffer += ")";
};
void CLangGenerator::visit(class FactorNode_Minus_Factor &node) {
  m_outputBuffer += "(";
  m_outputBuffer += "-";
  node.getFactor()->accept(*this);
  m_outputBuffer += ")";
};
void CLangGenerator::visit(
    class FactorNode_ID_Lparen_ExpressionList_Rparen &node) {
  // 处理函数调用节点
  m_outputBuffer += std::format("{}(", node.getID()->getValStr());
  auto record = symbolTable->lookup(node.getID()->getValStr());
  auto type = record->getType();
  auto funcType = type->get<SymbolType::Function>();

  m_stateStack.push(State::FunctionCall);
  m_paramsStack.push({funcType.param_types, 0});

  node.getExpressionList()->accept(*this);

  m_paramsStack.pop();
  m_stateStack.pop();
  m_outputBuffer += ")";
};

void CLangGenerator::writeln(const std::string &str) {
  m_outputBuffer += str + "\n";
}
std::string CLangGenerator::symbolType2Str(const SymbolType &type) {
  static const auto printer = overloaded{
      [](std::monostate) -> std::string { return "undefined"; },
      [](BasicType basic) -> std::string {
        switch (basic) {
        case BasicType::INTEGER:
          return "int";
        case BasicType::REAL:
          return "double";
        case BasicType::BOOLEAN:
          return "bool";
        case BasicType::CHAR:
          return "char";
        case BasicType::STRING:
          return "const char*";
        default:
          assert(false && "Unhandled BasicType");
        }
      },
      [this](const SymbolType::Array &array) -> std::string {
        if (!array.element_type)
          throw CodeGenerateException(
              ErrType::UNREACH_CODE,
              "Array element type is not set, this should not happen");

        // 计算数组长度（C风格索引从0开始）
        int32_t array_size = array.range.second - array.range.first + 1;

        // 递归获取元素类型的字符串表示
        std::string element_type_str = symbolType2Str(*array.element_type);

        // 组合成C风格数组类型
        return "[" + std::to_string(array_size) + "]" + element_type_str;
      },
      [this](const SymbolType::Record &record) -> std::string {
        std::ostringstream oss;
        oss << "record{";
        bool is_first = true;
        for (const auto &field : record.fields) {
          if (!is_first)
            oss << "; ";
          oss << field.name << ": " << symbolType2Str(*field.type);
          is_first = false;
        }
        oss << "}";
        return oss.str();
      },
      [this](const SymbolType::Function &func) -> std::string {
        std::ostringstream oss;
        oss << "function(";
        bool is_first = true;
        for (const auto &param : func.param_types) {
          if (!is_first)
            oss << ", ";
          oss << symbolType2Str(param->first);
          is_first = false;
        }
        oss << ") -> " << symbolType2Str(*func.return_type);
        return oss.str();
      },
      [this](const SymbolType::Procedure &proc) -> std::string {
        std::ostringstream oss;
        oss << "procedure(";
        bool is_first = true;
        for (const auto &param : proc.param_types) {
          if (!is_first)
            oss << ", ";
          oss << symbolType2Str(param->first);
          is_first = false;
        }
        oss << ")";
        return oss.str();
      },
      [](auto &&) -> std::string { return "unsupported_type"; } // 兜底
  };
  return type.visit(printer);
}
std::string CLangGenerator::relop2cStyle(const std::string &relop) {
  const static std::unordered_map<std::string, std::string> relop_map = {
      {"<", "<"},   {">", ">"},   {"<=", "<="}, {"=", "=="},
      {"!=", "!="}, {">=", ">="}, {"<>", "!="}};
  auto it = relop_map.find(relop);
  if (it != relop_map.end()) {
    return it->second;
  }
  throw CodeGenerateException(ErrType::UNREACH_CODE, "Unknown relop: " + relop);
}
std::string CLangGenerator::mulop2cStyle(const std::string &mulop) {
  const static std::unordered_map<std::string, std::string> mulop_map = {
      {"div", "/"}, {"mod", "%"}, {"and", "&&"}, {"*", "*"}, {"/", "/"}};
  auto it = mulop_map.find(mulop);
  if (it != mulop_map.end()) {
    return it->second;
  }
  throw CodeGenerateException(ErrType::UNREACH_CODE, "Unknown mulop: " + mulop);
}
std::string CLangGenerator::getCStyleIOFormatStr(
    const std::vector<std::shared_ptr<SymbolType>> &types) {
  static const auto visitor = overloaded{
      [](std::monostate) -> std::string {
        throw CodeGenerateException(ErrType::INVALID_INPUT,
                                    "Invalid type for C-style IO format");
      },
      [](BasicType basic) -> std::string {
        using std::literals::operator""s;
        switch (basic) {
        case BasicType::INTEGER:
          return "%d"s;
        case BasicType::REAL:
          // 2.0000000000000000E+000
          // we should use scientific format
          // so use %E with a length of 20
          // and exponential should be of length 3
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
            "Array type is not supported for C-style IO format");
      },
      [this](const SymbolType::Record &record) -> std::string {
        throw CodeGenerateException(
            ErrType::INVALID_INPUT,
            "Record type is not supported for C-style IO format");
      },
      [this](const SymbolType::Function &func) -> std::string {
        return getCStyleIOFormatStr({func.return_type});
      },
      [this](const SymbolType::Procedure &proc) -> std::string {
        throw CodeGenerateException(
            ErrType::INVALID_INPUT,
            "Procedure type is not supported for C-style IO format");
      },
      [](auto &&) -> std::string {
        throw CodeGenerateException(ErrType::INVALID_INPUT,
                                    "Unsupported type for C-style IO format");
      } // 兜底
  };
  std::string formatStr;
  for (size_t i = 0; i < types.size(); ++i) {
    formatStr += types[i]->visit(visitor);
    // Pascal write 无分隔符
  }
  return formatStr;
}
} // namespace XYZ
