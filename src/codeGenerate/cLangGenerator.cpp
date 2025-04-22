#include "codeGenerate/cLangGenerator.hpp"
#include "ast/ast.hpp"
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
  std::cout << "Generating code..." << std::endl;
  root->accept(*this);
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
  m_outputBuffer += node.get<std::string>();
  m_outputBuffer += " ";
};

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
  node.getCompoundStatement()->accept(*this);
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
  std::string buf = std::format("const {} {} = ", symbolType2Str(*type),
                                node.getId()->get<std::string>());
  m_outputBuffer += buf;
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
void CLangGenerator::visit(class ConstValNode_CharLiteral &node) {
  m_outputBuffer += std::format("'{}'", node.getCharLiteral()->getValStr());
};

void CLangGenerator::visit(class TypeNode &node) {
  throw CodeGenerateException(ErrType::UNREACH_CODE,
                              "TypeNode should not be visited");
};
void CLangGenerator::visit(class TypeNode_BasicType &node) {
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
    // 现在 typeStr 是 type[range1][range2]...的格式
    // get basic type str and rangeStr by f
    std::string rangeStr = typeStr.substr(typeStr.find('['));
    typeStr = typeStr.substr(0, typeStr.find('['));

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
          return "float";
        case BasicType::BOOLEAN:
          return "bool";
        case BasicType::CHAR:
          return "char";
        case BasicType::STRING:
          return "string";
        default:
          assert(false && "Unhandled BasicType");
          __builtin_unreachable();
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
        return element_type_str + "[" + std::to_string(array_size) + "]";
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
} // namespace XYZ
