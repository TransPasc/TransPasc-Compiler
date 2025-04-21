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
  shared_ptr<ProgramStructNode> root = nullptr;
  shared_ptr<SymbolTable> symbolTable = nullptr;

public:
  Analyzer() {}

  void analyze(shared_ptr<ProgramStructNode> root) {
    this->root = root;
    // 语义分析起点
    // 初始化符号表
    symbolTable = make_shared<StackLinkedSymbolTable>();
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
    shared_ptr<TerminalNode> id = node.getId();
    // 插入程序名到符号表
    // type 为 UNDEFINED
    unique_ptr<SymbolRecord> record =
        make_unique<SymbolRecord>(id->get<string>(), id->getLine());
    record->setType(make_shared<SymbolType>());
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
      curType = make_shared<SymbolType>(type);
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
    // 进入新的块
    symbolTable->enterBlock();
    node.getSubprogramHead()->accept(*this);
    // TODO: 获取 var_list, 并传递给 body
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
    record->setType(make_shared<SymbolType>(type));
    symbolTable->insert(std::move(record));
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
    record->setType(make_shared<SymbolType>(type));
    symbolTable->insert(std::move(record));
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
    auto ids = node.getIdList()->getAllIds();
    auto type = node.getBasicType()->getType();
    for (const auto &id : ids) {
      auto record =
          std::make_unique<SymbolRecord>(id->get<string>(), id->getLine());
      record->setType(type);
      symbolTable->insert(std::move(record));
    }
  };

  virtual void visit(class SubprogramBodyNode &node) {};
  virtual void
  visit(class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement &node) {
  };

  virtual void visit(class CompoundStatementNode &node) {};
  virtual void
  visit(class CompoundStatementNode_Begin_StatementList_End &node) {};

  virtual void visit(class StatementListNode &node) {};
  virtual void visit(class StatementListNode_Statement &node) {};
  virtual void
  visit(class StatementListNode_StatementList_Semicolon_Statement &node) {};

  virtual void visit(class StatementNode &node) {};
  virtual void visit(class StatementNode_Variable_Assignop_Expression &node) {};
  virtual void visit(class StatementNode_Id_Assignop_Expression &node) {};
  virtual void visit(class StatementNode_ProcedureCall &node) {};
  virtual void
  visit(class StatementNode_If_Expression_Then_Statement_ElsePart &node) {};
  virtual void visit(
      class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
          &node) {};
  virtual void
  visit(class StatementNode_Read_Lparen_VariableList_Rparen &node) {};
  virtual void
  visit(class StatementNode_Write_Lparen_ExpressionList_Rparen &node) {};

  virtual void visit(class VariableListNode &node) {};
  virtual void visit(class VariableListNode_Variable &node) {};
  virtual void visit(class VariableListNode_VariableList_Comma_Variable &node) {
  };

  virtual void visit(class VariableNode &node) {};
  virtual void visit(class VariableNode_Id_IdVarpart &node) {};

  virtual void visit(class IdVarPartNode &node) {};
  virtual void visit(class IdVarPartNode_Lbracket_Expression_Rbracket &node) {};

  virtual void visit(class ProcedureCallNode &node) {};
  virtual void visit(class ProcedureCallNode_Id &node) {};
  virtual void
  visit(class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen &node) {};

  virtual void visit(class ElsePartNode &node) {};
  virtual void visit(class ElsePartNode_Else_Statement &node) {};

  virtual void visit(class ExpressionListNode &node) {};
  virtual void visit(class ExpressionListNode_Expression &node) {};
  virtual void
  visit(class ExpressionListNode_ExpressionList_Comma_Expression &node) {};

  virtual void visit(class ExpressionNode &node) {};
  virtual void visit(class ExpressionNode_SimpleExpression &node) {};
  virtual void
  visit(class ExpressionNode_SimpleExpression_Relop_SimpleExpression &node) {};

  virtual void visit(class SimpleExpressionNode &node) {};
  virtual void visit(class SimpleExpressionNode_Term &node) {};
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Plus_Term &node) {};
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Minus_Term &node) {};
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Or_Term &node) {};

  virtual void visit(class TermNode &node) {};
  virtual void visit(class TermNode_Factor &node) {};
  virtual void visit(class TermNode_Term_Mulop_Factor &node) {};

  virtual void visit(class FactorNode &node) {};
  virtual void visit(class FactorNode_Number &node) {};
  virtual void visit(class FactorNode_CharLiteral &node) {};
  virtual void visit(class FactorNode_Variable &node) {};
  virtual void visit(class FactorNode_Lparen_Expression_Rparen &node) {};
  virtual void visit(class FactorNode_Not_Factor &node) {};
  virtual void visit(class FactorNode_Minus_Factor &node) {};
};
} // namespace XYZ
