#pragma once
#include "codeGenerate/exception.hpp"
#include "generator.hpp"
#include "symbolTable/stackLinkedSymbolTable.hpp"
#include "symbolTable/type.hpp"
namespace XYZ {
/**
 * @brief c language code generator
 * from the AST
 */
class CLangGenerator : public Generator {
  std::string m_outputFile;
  std::string m_outputBuffer;
  bool m_isRefParam = false;
  using ErrType = CodeGenerateException::ErrorCode;
  std::shared_ptr<SymbolTable> symbolTable;
  std::string m_expList_split = ", ";
  SymbolType::ParamsType params = {};

public:
  CLangGenerator();
  ~CLangGenerator() override;
  /**
   * @brief generate code from AST
   * if set output file, the code will be write to the file
   * if not, the code will be
   * print to the console
   * @param root the root of the AST
   */
  void generateCode(ASTNode::ASTNodePtr root) override;

  virtual void setOutputFile(const std::string &filename) override;

  virtual void visit(class TerminalNode &node) override;

  virtual void visit(class ProgramStructNode &node) override;
  virtual void
  visit(class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node)
      override;

  virtual void visit(class ProgramHeadNode &node) override;
  virtual void
  visit(class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen &node) override;
  virtual void visit(class ProgramHeadNode_Program_Id &node) override;

  virtual void visit(class ProgramBodyNode &node) override;
  virtual void
  visit(class
        ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
            &node) override;

  virtual void visit(class IdListNode &node) override;
  virtual void visit(class IdListNode_Id &node) override;
  virtual void visit(class IdListNode_IdList_Comma_Id &node) override;

  virtual void visit(class ConstDeclsNode &node) override;
  virtual void visit(class ConstDeclsNode_Const_ConstDecl &node) override;

  virtual void visit(class ConstDeclNode &node) override;
  virtual void
  visit(class ConstDeclNode_Id_Relop_ConstVal_Semicolon &node) override;
  virtual void visit(
      class ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon &node) override;

  virtual void visit(class ConstValNode &node) override;
  virtual void visit(class ConstValNode_Plus_Number &node) override;
  virtual void visit(class ConstValNode_Minus_Number &node) override;
  virtual void visit(class ConstValNode_Number &node) override;
  virtual void visit(class ConstValNode_CharLiteral &node) override;

  virtual void visit(class TypeNode &node) override;
  virtual void visit(class TypeNode_BasicType &node) override;
  virtual void visit(class TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType
                         &node) override;

  virtual void visit(class BasicTypeNode &node) override;
  virtual void visit(class BasicTypeNode_Integer &node) override;
  virtual void visit(class BasicTypeNode_Real &node) override;
  virtual void visit(class BasicTypeNode_Boolean &node) override;
  virtual void visit(class BasicTypeNode_Char &node) override;

  virtual void visit(class PeriodNode &node) override;
  virtual void visit(class PeriodNode_Number_Dot_Dot_Number &node) override;
  virtual void
  visit(class PeriodNode_Period_Comma_Number_Dot_Dot_Number &node) override;

  virtual void visit(class VarDeclsNode &node) override;
  virtual void visit(class VarDeclsNode_Var_VarDecl_Semicolon &node) override;

  virtual void visit(class VarDeclNode &node) override;
  virtual void visit(class VarDeclNode_IdList_Colon_Type &node) override;
  virtual void
  visit(class VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type &node) override;

  virtual void visit(class SubprogramDeclsNode &node) override;
  virtual void
  visit(class SubprogramDeclsNode_SubprogramDecls_Subprogram &node) override;

  virtual void visit(class SubprogramNode &node) override;
  virtual void
  visit(class SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON
            &node) override;

  virtual void visit(class SubprogramHeadNode &node) override;
  virtual void
  visit(class SubprogramHeadNode_Procedure_Id_FormalParameter &node) override;
  virtual void
  visit(class SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType
            &node) override;

  virtual void visit(class FormalParameterNode &node) override;
  virtual void
  visit(class FormalParameterNode_Lparen_ParameterList_Rparen &node) override;

  virtual void visit(class ParameterListNode &node) override;
  virtual void visit(class ParameterListNode_Parameter &node) override;
  virtual void visit(
      class ParameterListNode_ParameterList_Semicolon_Parameter &node) override;

  virtual void visit(class ParameterNode &node) override;
  virtual void visit(class ParameterNode_VarParameter &node) override;
  virtual void visit(class ParameterNode_ValueParameter &node) override;

  virtual void visit(class VarParameterNode &node) override;
  virtual void visit(class VarParameterNode_Var_ValueParameter &node) override;

  virtual void visit(class ValueParameterNode &node) override;
  virtual void
  visit(class ValueParameterNode_IdList_Colon_BasicType &node) override;

  virtual void visit(class SubprogramBodyNode &node) override;
  virtual void
  visit(class SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement &node)
      override;

  virtual void visit(class CompoundStatementNode &node) override;
  virtual void
  visit(class CompoundStatementNode_Begin_StatementList_End &node) override;

  virtual void visit(class StatementListNode &node) override;
  virtual void visit(class StatementListNode_Statement &node) override;
  virtual void visit(
      class StatementListNode_StatementList_Semicolon_Statement &node) override;

  virtual void visit(class StatementNode &node) override;
  virtual void
  visit(class StatementNode_Variable_Assignop_Expression &node) override;
  virtual void visit(class StatementNode_Id_Assignop_Expression &node) override;
  virtual void visit(class StatementNode_ProcedureCall &node) override;
  virtual void visit(
      class StatementNode_If_Expression_Then_Statement_ElsePart &node) override;
  virtual void visit(
      class StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement
          &node) override;
  virtual void
  visit(class StatementNode_Read_Lparen_VariableList_Rparen &node) override;
  virtual void
  visit(class StatementNode_Write_Lparen_ExpressionList_Rparen &node) override;
  virtual void visit(class StatementNode_CompoundStatement &node) override;

  virtual void visit(class VariableListNode &node) override;
  virtual void visit(class VariableListNode_Variable &node) override;
  virtual void
  visit(class VariableListNode_VariableList_Comma_Variable &node) override;

  virtual void visit(class VariableNode &node) override;
  virtual void visit(class VariableNode_Id_IdVarpart &node) override;

  virtual void visit(class IdVarPartNode &node) override;
  virtual void
  visit(class IdVarPartNode_Lbracket_ExpressionList_Rbracket &node) override;

  virtual void visit(class ProcedureCallNode &node) override;
  virtual void visit(class ProcedureCallNode_Id &node) override;
  virtual void
  visit(class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen &node) override;

  virtual void visit(class ElsePartNode &node) override;
  virtual void visit(class ElsePartNode_Else_Statement &node) override;

  virtual void visit(class ExpressionListNode &node) override;
  virtual void visit(class ExpressionListNode_Expression &node) override;
  virtual void visit(
      class ExpressionListNode_ExpressionList_Comma_Expression &node) override;

  virtual void visit(class ExpressionNode &node) override;
  virtual void visit(class ExpressionNode_SimpleExpression &node) override;
  virtual void
  visit(class ExpressionNode_SimpleExpression_Relop_SimpleExpression &node)
      override;

  virtual void visit(class SimpleExpressionNode &node) override;
  virtual void visit(class SimpleExpressionNode_Term &node) override;
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Plus_Term &node) override;
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Minus_Term &node) override;
  virtual void
  visit(class SimpleExpressionNode_SimpleExpression_Or_Term &node) override;

  virtual void visit(class TermNode &node) override;
  virtual void visit(class TermNode_Factor &node) override;
  virtual void visit(class TermNode_Term_Mulop_Factor &node) override;

  virtual void visit(class FactorNode &node) override;
  virtual void visit(class FactorNode_Number &node) override;
  virtual void visit(class FactorNode_CharLiteral &node) override;
  virtual void visit(class FactorNode_Variable &node) override;
  virtual void visit(class FactorNode_Lparen_Expression_Rparen &node) override;
  virtual void visit(class FactorNode_Not_Factor &node) override;
  virtual void visit(class FactorNode_Minus_Factor &node) override;
  virtual void
  visit(class FactorNode_ID_Lparen_ExpressionList_Rparen &node) override;

private:
  // 公有产生式处理
  void g_IdList_Type(std::shared_ptr<IdListNode> idList,
                     std::shared_ptr<TypeNode> type);

private:
  // 生成代码的辅助函数, 写入str到缓冲区
  void writeln(const std::string &str);
  template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
  };
  template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
  std::string symbolType2Str(const SymbolType &type);
  // 将 Pascal relop 转换为 C 语言的 relop
  std::string relop2cStyle(const std::string &relop);
};
} // namespace XYZ
