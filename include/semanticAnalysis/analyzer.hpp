#pragma once
#include "ast/ast.hpp"
#include "exception.hpp"
#include "symbolTable/stackLinkedSymbolTable.hpp"
#include <iostream>
#include <memory>
#include <stack>
#include <string>
// TODO: 补充各个节点的visit函数
namespace XYZ {
using namespace std;
class Analyzer : public ASTVisitor {
  using ErrType = SemanticException::ErrorType;
  std::shared_ptr<ProgramStructNode> root = nullptr;
  std::shared_ptr<SymbolTable> symbolTable = nullptr;

public:
  Analyzer() {}

  void analyze(std::shared_ptr<ProgramStructNode> root) {
    this->root = root;
    // 语义分析起点
    // 初始化符号表
    symbolTable = std::make_shared<StackLinkedSymbolTable>();
    symbolTable->enterBlock();
    root->accept(*this);
    // 语义分析结束
    symbolTable->exitBlock();
  }

  virtual void visit(class TerminalNode &node) {
    // nothing to do
    // 信息已经在词法分析阶段处理过了
  };

  virtual void visit(class ProgramStructNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ProgramStructNode should not be Null");
  };

  virtual void
  visit(class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node) {
    node.getProgramHead()->accept(*this);
    node.getProgramBody()->accept(*this);
  };

  virtual void visit(class ProgramHeadNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ProgramHeadNode should not be Null");
  };

  virtual void
  visit(class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen &node) {
    throw SemanticException(
        ErrType::UNSUPPORTED,
        "Unexpected ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen");
    // node.getId()->accept(*this);
    // node.getIdList()->accept(*this);
  };

  virtual void visit(class ProgramHeadNode_Program_Id &node) {
    node.getId()->accept(*this);
    std::shared_ptr<TerminalNode> id = node.getId();
    // 插入程序名到符号表
    // type 为 UNDEFINED
    unique_ptr<SymbolRecord> record =
        make_unique<SymbolRecord>(id->get<string>(), id->getLine());
    record->setType(std::make_shared<SymbolType>());
    symbolTable->insert(std::move(record));
  };

  virtual void visit(class ProgramBodyNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ProgramBodyNode should not be Null");
  };

  virtual void
  visit(class
        ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
            &node) {
    node.getConstDecls()->accept(*this);
    node.getVarDecls()->accept(*this);
    node.getSubprogramDecls()->accept(*this);
    node.getCompoundStatement()->accept(*this);
  };

  virtual void visit(class IdListNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "IdListNode should not be Null");
  };

  virtual void visit(class IdListNode_Id &node) {
    // nothing need to do
  };

  virtual void visit(class IdListNode_IdList_Comma_Id &node) {
    node.getIdList()->accept(*this);
    node.getId()->accept(*this);
  };

  virtual void visit(class ConstDeclsNode &node) {
    // TODO:使用日志
    cout << "no const decls" << endl;
  };

  virtual void visit(class ConstDeclsNode_Const_ConstDecl &node) {
    node.getConstDecl()->accept(*this);
  };

  virtual void visit(class ConstDeclNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ConstDeclNode should not be Null");
  };

  virtual void visit(class ConstDeclNode_Id_Relop_ConstVal_Semicolon &node) {
    node.getId()->accept(*this);
    node.getConstVal()->accept(*this);
    unique_ptr<SymbolRecord> record = make_unique<SymbolRecord>(
        node.getId()->get<string>(), node.getId()->getLine());
    auto type = node.getConstVal()->getType();
    record->setType(type);
    symbolTable->insert(std::move(record));
  };

  virtual void
  visit(class ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon &node) {
    node.getConstDecl()->accept(*this);
    node.getId()->accept(*this);
    node.getConstVal()->accept(*this);
    unique_ptr<SymbolRecord> record = make_unique<SymbolRecord>(
        node.getId()->get<string>(), node.getId()->getLine());
    auto type = node.getConstVal()->getType();
    record->setType(type);
    symbolTable->insert(std::move(record));
  };

  virtual void visit(class ConstValNode &node) {
    // nothing to do
  };
  virtual void visit(class ConstValNode_Plus_Number &node) {
    // nothing to do
  };
  virtual void visit(class ConstValNode_Minus_Number &node) {
    // nothing to do
  };
  virtual void visit(class ConstValNode_Number &node) {
    // nothing to do
  };
  virtual void visit(class ConstValNode_CharLiteral &node) {
    // nothing to do
  };

  virtual void visit(class TypeNode &node) {
    throw SemanticException(ErrType::UNDEFINED, "TypeNode should not be Null");
  };

  virtual void visit(class TypeNode_BasicType &node) {
    node.getBasicType()->accept(*this);
    node.setType(node.getBasicType()->getType());
  };

  virtual void
  visit(class TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType &node) {
    node.getPeriod()->accept(*this);
    node.getBasicType()->accept(*this);
    auto curType = node.getBasicType()->getType();
    auto periods = node.getPeriod()->getPeriods();
    for (auto it = periods->rbegin(); it != periods->rend(); ++it) {
      const auto &[l, h] = **it;
      auto type = SymbolType::MakeArray(curType, l, h);
      curType = std::make_shared<SymbolType>(type);
    }
    node.setType(curType);
  };

  virtual void visit(class BasicTypeNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "BasicTypeNode should not be Null");
  };
  virtual void visit(class BasicTypeNode_Integer &node) {
    auto type = SymbolType::MakeBasic(BasicType::INTEGER);
    node.setType(std::make_shared<SymbolType>(type));
  };
  virtual void visit(class BasicTypeNode_Real &node) {
    auto type = SymbolType::MakeBasic(BasicType::REAL);
    node.setType(std::make_shared<SymbolType>(type));
  };
  virtual void visit(class BasicTypeNode_Boolean &node) {
    auto type = SymbolType::MakeBasic(BasicType::BOOLEAN);
    node.setType(std::make_shared<SymbolType>(type));
  };
  virtual void visit(class BasicTypeNode_Char &node) {
    auto type = SymbolType::MakeBasic(BasicType::CHAR);
    node.setType(std::make_shared<SymbolType>(type));
  };

  virtual void visit(class PeriodNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "PeriodNode should not be Null");
  };

  virtual void visit(class PeriodNode_Number_Dot_Dot_Number &node) {
    // nothing to do
  };

  virtual void
  visit(class PeriodNode_Period_Comma_Number_Dot_Dot_Number &node) {
    node.getPeriod()->accept(*this);
  };

  virtual void visit(class VarDeclsNode &node) {
    cout << "no var decls" << endl;
  };
  virtual void visit(class VarDeclsNode_Var_VarDecl_Semicolon &node) {
    node.getVarDecl()->accept(*this);
  };

  virtual void visit(class VarDeclNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "VarDeclNode should not be Null");
  };

  virtual void visit(class VarDeclNode_IdList_Colon_Type &node) {
    node.getIdList()->accept(*this);
    node.getType()->accept(*this);
    auto ids = node.getIdList()->getAllIds();
    auto type = node.getType()->getType();
    for (const auto &id : ids) {
      auto record =
          std::make_unique<SymbolRecord>(id->get<string>(), id->getLine());
      record->setType(type);
      symbolTable->insert(std::move(record));
    }
  };

  virtual void
  visit(class VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type &node) {
    node.getVarDecl()->accept(*this);
    node.getIdList()->accept(*this);
    node.getType()->accept(*this);
    auto ids = node.getIdList()->getAllIds();
    auto type = node.getType()->getType();
    for (const auto &id : ids) {
      auto record =
          std::make_unique<SymbolRecord>(id->get<string>(), id->getLine());
      record->setType(type);
      symbolTable->insert(std::move(record));
    }
  };

  virtual void visit(class SubprogramDeclsNode &node) {
    cout << "no subprogram decls" << endl;
  };

  virtual void
  visit(class SubprogramDeclsNode_SubprogramDecls_Subprogram &node) {
    node.getSubprogramDecls()->accept(*this);
    node.getSubprogram()->accept(*this);
  };

  virtual void visit(class SubprogramNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "SubprogramNode should not be Null");
  };

  virtual void
  visit(class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON
            &node) {
    // 进入block 的逻辑在 programhead 中(function or procedure)
    node.getSubprogramHead()->accept(*this);
    node.getSubprogramBody()->accept(*this);
    // 退出当前块
    symbolTable->exitBlock();
  };

  virtual void visit(class SubprogramHeadNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "SubprogramHeadNode should not be Null");
  };

  virtual void
  visit(class SubprogramHeadNode_Procedure_Id_FormalParameter &node) {
    node.getId()->accept(*this);
    node.getFormalParameter()->accept(*this);
    auto params = node.getFormalParameter()->getParams();
    auto id = node.getId();
    unique_ptr<SymbolRecord> record =
        make_unique<SymbolRecord>(id->get<string>(), id->getLine());
    auto type = SymbolType::MakeProcedure(params);
    record->setType(std::make_shared<SymbolType>(type));
    symbolTable->insert(std::move(record));
    // 进入新的块 (要在把函数名插入符号表之后,否则 function名会被删除)
    symbolTable->enterBlock();
    // 将 ids及其类型信息插入到符号表中
    for (const auto &param : params) {
      auto record = make_unique<SymbolRecord>(param->second, id->getLine());
      record->setType(std::make_shared<SymbolType>(param->first));
      symbolTable->insert(std::move(record));
    }
  };

  virtual void
  visit(class SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType
            &node) {
    node.getId()->accept(*this);
    node.getFormalParameter()->accept(*this);
    node.getBasicType()->accept(*this);
    // TODO: params 的顺序反了
    auto params = node.getFormalParameter()->getParams();
    auto id = node.getId();
    unique_ptr<SymbolRecord> record =
        make_unique<SymbolRecord>(id->get<string>(), id->getLine());
    auto type =
        SymbolType::MakeFunction(node.getBasicType()->getType(), params);
    record->setType(std::make_shared<SymbolType>(type));
    symbolTable->insert(std::move(record));
    // 进入新的块 (要在把函数名插入符号表之后,否则 function名会被删除)
    symbolTable->enterBlock();
    // 将 ids及其类型信息插入到符号表中
    for (const auto &param : params) {
      auto record = make_unique<SymbolRecord>(param->second, id->getLine());
      record->setType(std::make_shared<SymbolType>(param->first));
      symbolTable->insert(std::move(record));
    }
  };

  virtual void visit(class FormalParameterNode &node) {
    cout << "no formal parameter" << endl;
  };

  virtual void
  visit(class FormalParameterNode_Lparen_ParameterList_Rparen &node) {
    node.getParameterList()->accept(*this);
  };

  virtual void visit(class ParameterListNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ParameterListNode should not be Null");
  };

  virtual void visit(class ParameterListNode_Parameter &node) {
    node.getParameter()->accept(*this);
  };

  virtual void
  visit(class ParameterListNode_ParameterList_Semicolon_Parameter &node) {
    node.getParameterList()->accept(*this);
    node.getParameter()->accept(*this);
  };

  virtual void visit(class ParameterNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ParameterNode should not be Null");
  };

  virtual void visit(class ParameterNode_VarParameter &node) {
    node.getVarParameter()->accept(*this);
  };

  virtual void visit(class ParameterNode_ValueParameter &node) {
    node.getValueParameter()->accept(*this);
  };

  virtual void visit(class VarParameterNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "VarParameterNode should not be Null");
  };
  virtual void visit(class VarParameterNode_Var_ValueParameter &node) {
    node.getValueParameter()->accept(*this);
  };

  virtual void visit(class ValueParameterNode &node) {
    cout << "Visiting ValueParameterNode" << endl;
  };

  virtual void visit(class ValueParameterNode_IdList_Colon_BasicType &node) {
    node.getIdList()->accept(*this);
    node.getBasicType()->accept(*this);
  };

  virtual void visit(class SubprogramBodyNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "SubprogramBodyNode should not be Null");
  };
  virtual void
  visit(class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement &node) {
    node.getConstDecls()->accept(*this);
    node.getVarDecls()->accept(*this);
    node.getCompoundStatement()->accept(*this);
  };

  virtual void visit(class CompoundStatementNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "CompoundStatementNode should not be Null");
  };
  virtual void
  visit(class CompoundStatementNode_Begin_StatementList_End &node) {
    // 进入新的块
    symbolTable->enterBlock();
    node.getStatementList()->accept(*this);
    // 退出当前块
    symbolTable->exitBlock();
  };

  virtual void visit(class StatementListNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "StatementListNode should not be Null");
  };
  virtual void visit(class StatementListNode_Statement &node) {
    node.getStatement()->accept(*this);
  };
  virtual void
  visit(class StatementListNode_StatementList_Semicolon_Statement &node) {
    node.getStatementList()->accept(*this);
    node.getStatement()->accept(*this);
  };

  virtual void visit(class StatementNode &node) {
    cout << "statement is null" << endl;
  };
  virtual void visit(class StatementNode_Variable_Assignop_Expression &node) {
    node.getVariable()->accept(*this);
    node.getExpression()->accept(*this);
    auto leftType = node.getVariable()->getType();
    auto rightType = node.getExpression()->getType();
    if (!leftType->strictEq(*rightType)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in assignment");
    }
  };
  virtual void visit(class StatementNode_Id_Assignop_Expression &node) {
    node.getId()->accept(*this);
    node.getExpression()->accept(*this);
    auto id = node.getId();
    auto record = symbolTable->lookup(id->get<string>());
    if (record == nullptr) {
      throw SemanticException(ErrType::UNDEFINED,
                              "Undefined variable: " + id->get<string>());
    }
    auto type = record->getType();
    auto expType = node.getExpression()->getType();

    auto funcType = type->get_if<SymbolType::Function>();
    if (funcType) {
      //   是函数类型
      auto retType = funcType->return_type;
      if (retType->strictEq(*expType) == false) {
        throw SemanticException(ErrType::UNSUPPORTED,
                                "Incompatible types in assignment to function");
      }
      return;
    }
    // 不是函数类型，是变量类型
    if (type->strictEq(*expType) == false) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in assignment");
    }
  };
  virtual void visit(class StatementNode_ProcedureCall &node) {
    node.getProcedureCall()->accept(*this);
  };

  virtual void
  visit(class StatementNode_If_Expression_Then_Statement_ElsePart &node) {
    node.getExpression()->accept(*this);
    auto type = node.getExpression()->getType();
    if (type->strictEq(BasicType::BOOLEAN) == false) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in if statement");
    }
    node.getStatement()->accept(*this);
    node.getElsePart()->accept(*this);
  };

  virtual void visit(
      class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
          &node) {
    node.getId()->accept(*this);
    node.getExpression1()->accept(*this);
    node.getExpression2()->accept(*this);
    node.getStatement()->accept(*this);
    auto id = node.getId();
    auto record = symbolTable->lookup(id->get<string>());
    if (record == nullptr) {
      throw SemanticException(ErrType::UNDEFINED,
                              "Undefined variable: " + id->get<string>());
    }
    // TODO: 循环变量暂时只支持整数
    auto type = record->getType();
    if (type->strictEq(BasicType::INTEGER) == false) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in for statement");
    }
    auto exp1Type = node.getExpression1()->getType();
    auto exp2Type = node.getExpression2()->getType();
    if (exp1Type->strictEq(BasicType::INTEGER) == false ||
        exp2Type->strictEq(BasicType::INTEGER) == false) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in for statement");
    }
  };

  virtual void
  visit(class StatementNode_Read_Lparen_VariableList_Rparen &node) {
    node.getVariableList()->accept(*this);
    // TODO: 读取文件类型检查
  };

  virtual void
  visit(class StatementNode_Write_Lparen_ExpressionList_Rparen &node) {
    node.getExpressionList()->accept(*this);
    // TODO: 写入文件类型检查
  };
  virtual void visit(class StatementNode_CompoundStatement &node) {
    node.getCompoundStatement()->accept(*this);
  };

  virtual void visit(class VariableListNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "VariableListNode should not be Null");
  };
  virtual void visit(class VariableListNode_Variable &node) {
    node.getVariable()->accept(*this);
  };
  virtual void visit(class VariableListNode_VariableList_Comma_Variable &node) {
    node.getVariableList()->accept(*this);
    node.getVariable()->accept(*this);
  };

  virtual void visit(class VariableNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "VariableNode should not be Null");
  };
  virtual void visit(class VariableNode_Id_IdVarpart &node) {
    node.getId()->accept(*this);
    node.getIdVarpart()->accept(*this);
    auto id = node.getId();
    auto record = symbolTable->lookup(id->get<string>());
    if (record == nullptr) {
      throw SemanticException(ErrType::UNDEFINED,
                              "Undefined variable: " + id->get<string>());
    }
    auto type = record->getType();

    auto expTypes = node.getIdVarpart()->getTypeList();
    if (0 == expTypes.size()) {
      node.setValType(*type);
      return;
    }
    if (!type->is_array()) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Not an array: " + id->get<string>() +
                                  " in variable node");
    }
    auto arrayType = type->get_if<SymbolType::Array>();
    if (arrayType == nullptr) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Not an array: " + id->get<string>() +
                                  " in variable node");
    }
    auto subType = arrayType->getSubType(expTypes);
    if (subType == nullptr) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in array subscript");
    }
    node.setValType(*subType);
  };

  virtual void visit(class IdVarPartNode &node) {
    cout << "IdVarPartNode is null" << endl;
  };
  virtual void
  visit(class IdVarPartNode_Lbracket_ExpressionList_Rbracket &node) {
    node.getExpressionList()->accept(*this);
  };

  virtual void visit(class ProcedureCallNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ProcedureCallNode should not be Null");
  };
  virtual void visit(class ProcedureCallNode_Id &node) {
    node.getId()->accept(*this);
    auto id = node.getId();
    auto record = symbolTable->lookup(id->get<string>());
    if (record == nullptr) {
      throw SemanticException(ErrType::UNDEFINED,
                              "Undefined procedure: " + id->get<string>());
    }
  };

  virtual void
  visit(class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen &node) {
    node.getId()->accept(*this);
    node.getExpressionList()->accept(*this);
    auto id = node.getId();
    auto record = symbolTable->lookup(id->get<string>());
    if (record == nullptr) {
      throw SemanticException(ErrType::UNDEFINED,
                              "Undefined procedure: " + id->get<string>());
    }
    auto procedureType = record->getType()->get_if<SymbolType::Procedure>();
    if (procedureType == nullptr) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Not a procedure: " + id->get<string>());
    }
    auto expTypes = node.getExpressionList()->getTypeList();
    auto params = procedureType->param_types;
    // TODO: 将下面的代码提取到一个函数中
    // 检查参数个数
    if (expTypes.size() != params.size()) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible number of parameters");
    }
    // 检查参数类型
    for (size_t i = 0; i < expTypes.size(); ++i) {
      auto expType = expTypes[i];
      auto paramType = params[i];
      if (paramType->first.strictEq(*expType) == false) {
        throw SemanticException(ErrType::UNSUPPORTED,
                                "Incompatible types in procedure call");
      }
    }
  };

  virtual void visit(class ElsePartNode &node) {
    cout << "no else part" << endl;
  };
  virtual void visit(class ElsePartNode_Else_Statement &node) {
    node.getStatement()->accept(*this);
  };

  virtual void visit(class ExpressionListNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ExpressionListNode should not be Null");
  };
  virtual void visit(class ExpressionListNode_Expression &node) {
    node.getExpression()->accept(*this);
  };
  virtual void
  visit(class ExpressionListNode_ExpressionList_Comma_Expression &node) {
    node.getExpressionList()->accept(*this);
    node.getExpression()->accept(*this);
  };

  virtual void visit(class ExpressionNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "ExpressionNode should not be Null");
  };
  virtual void visit(class ExpressionNode_SimpleExpression &node) {
    node.getSimpleExpression()->accept(*this);
  };
  virtual void
  visit(class ExpressionNode_SimpleExpression_Relop_SimpleExpression &node) {
    node.getSimpleExpression1()->accept(*this);
    node.getSimpleExpression2()->accept(*this);
    auto type1 = node.getSimpleExpression1()->getType();
    auto type2 = node.getSimpleExpression2()->getType();
    // 假设没有隐式类型转换
    if (!type1->strictEq(*type2)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in relational operation");
    }
    if (!type1->strictEq(BasicType::INTEGER) &&
        !type1->strictEq(BasicType::REAL) &&
        !type1->strictEq(BasicType::CHAR)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in relational operation");
    }
  };

  virtual void visit(class SimpleExpressionNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "SimpleExpressionNode should not be Null");
  };
  virtual void visit(class SimpleExpressionNode_Term &node) {
    node.getTerm()->accept(*this);
  };
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Plus_Term &node) {
    node.getSimpleExpression()->accept(*this);
    node.getTerm()->accept(*this);
    auto simpleType = node.getSimpleExpression()->getType();
    auto termType = node.getTerm()->getType();
    // 假设没有隐式类型转换
    if (!simpleType->strictEq(*termType)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in addition");
    }
    if (!simpleType->strictEq(BasicType::INTEGER) &&
        !simpleType->strictEq(BasicType::REAL)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in unary plus");
    }
  };
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Minus_Term &node) {
    node.getSimpleExpression()->accept(*this);
    node.getTerm()->accept(*this);
    auto simpleType = node.getSimpleExpression()->getType();
    auto termType = node.getTerm()->getType();
    // 假设没有隐式类型转换
    if (!simpleType->strictEq(*termType)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in subtraction");
    }
    if (!simpleType->strictEq(BasicType::INTEGER) &&
        !simpleType->strictEq(BasicType::REAL)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in unary minus");
    }
  };
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Or_Term &node) {
    node.getSimpleExpression()->accept(*this);
    node.getTerm()->accept(*this);
    auto simpleType = node.getSimpleExpression()->getType();
    auto termType = node.getTerm()->getType();
    if (!simpleType->strictEq(BasicType::BOOLEAN) ||
        !termType->strictEq(BasicType::BOOLEAN)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in or operation");
    }
  };

  virtual void visit(class TermNode &node) {
    throw SemanticException(ErrType::UNDEFINED, "TermNode should not be Null");
  };
  virtual void visit(class TermNode_Factor &node) {
    node.getFactor()->accept(*this);
  };
  virtual void visit(class TermNode_Term_Mulop_Factor &node) {
    node.getTerm()->accept(*this);
    node.getFactor()->accept(*this);
    auto termType = node.getTerm()->getType();
    auto factorType = node.getFactor()->getType();
    // TODO: 类型检查 (这里假设没有隐式类型转换)
    if (!termType->strictEq(*factorType)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in multiplication");
    }
    // TODO: 除0检查
  };

  virtual void visit(class FactorNode &node) {
    throw SemanticException(ErrType::UNDEFINED,
                            "FactorNode should not be Null");
  };
  virtual void visit(class FactorNode_Number &node) {
    // nothing to do
    // 因为在词法分析阶段已经处理过了
    node.getNumber()->accept(*this);
  };
  virtual void visit(class FactorNode_CharLiteral &node) {
    // TODO: charliteral 失败貌似不正确
    // nothing to do
    // 因为在词法分析阶段已经处理过了
    node.getCharLiteral()->accept(*this);
  };
  virtual void visit(class FactorNode_Variable &node) {
    // 转发到变量节点
    node.getVariable()->accept(*this);
  };
  virtual void visit(class FactorNode_Lparen_Expression_Rparen &node) {
    // 括号表达式
    // 无需校验，转发到exp即可
    node.getExpression()->accept(*this);
  };
  virtual void visit(class FactorNode_ID_Lparen_ExpressionList_Rparen &node) {
    // 函数调用
    node.getID()->accept(*this);
    node.getExpressionList()->accept(*this);
    auto id = node.getID();
    auto record = symbolTable->lookup(id->get<string>());
    if (record == nullptr) {
      throw SemanticException(ErrType::UNDEFINED,
                              "Undefined procedure: " + id->get<string>());
    }
    auto funcType = record->getType()->get_if<SymbolType::Function>();
    if (funcType == nullptr) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Not a function: " + id->get<string>());
    }
    auto expTypes = node.getExpressionList()->getTypeList();
    //  检查参数个数和类型
    auto params = funcType->param_types;
    if (params.size() != expTypes.size()) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible number of parameters");
    }
    for (size_t i = 0; i < params.size(); ++i) {
      if (!params[i]->first.strictEq(*expTypes[i])) {
        throw SemanticException(ErrType::UNSUPPORTED,
                                "Incompatible types in function call");
      }
    }
    //  设置类型
    auto retType = funcType->return_type;
    node.setRetType(*retType);
  }
  virtual void visit(class FactorNode_Not_Factor &node) {
    node.getFactor()->accept(*this);
    auto type = node.getFactor()->getType();
    if (!type->strictEq(BasicType::BOOLEAN)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in unary not");
    }
  };
  virtual void visit(class FactorNode_Minus_Factor &node) {
    node.getFactor()->accept(*this);
    auto type = node.getFactor()->getType();
    if (!type->strictEq(BasicType::INTEGER) &&
        !type->strictEq(BasicType::REAL)) {
      throw SemanticException(ErrType::UNSUPPORTED,
                              "Incompatible types in unary minus");
    }
  };
};
} // namespace XYZ
