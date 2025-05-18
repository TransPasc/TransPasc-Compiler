#pragma once
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
// 分发到所有节点
#define dispatch_all_nodes(dispatch)                                           \
  dispatch(TerminalNode);                                                      \
  dispatch(ProgramStructNode);                                                 \
  dispatch(ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot);           \
                                                                               \
  dispatch(ProgramHeadNode);                                                   \
  dispatch(ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen);                   \
  dispatch(ProgramHeadNode_Program_Id);                                        \
                                                                               \
  dispatch(ProgramBodyNode);                                                   \
                                                                               \
  dispatch(                                                                    \
      ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement);  \
                                                                               \
  dispatch(IdListNode);                                                        \
  dispatch(IdListNode_Id);                                                     \
  dispatch(IdListNode_IdList_Comma_Id);                                        \
                                                                               \
  dispatch(ConstDeclsNode);                                                    \
  dispatch(ConstDeclsNode_Const_ConstDecl);                                    \
                                                                               \
  dispatch(ConstDeclNode);                                                     \
  dispatch(ConstDeclNode_Id_Relop_ConstVal_Semicolon);                         \
  dispatch(ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon);               \
                                                                               \
  dispatch(ConstValNode);                                                      \
  dispatch(ConstValNode_Plus_Number);                                          \
  dispatch(ConstValNode_Minus_Number);                                         \
  dispatch(ConstValNode_Number);                                               \
  dispatch(ConstValNode_StringLiteral);                                        \
  dispatch(ConstValNode_CharLiteral);                                          \
                                                                               \
  dispatch(TypeNode);                                                          \
  dispatch(TypeNode_BasicType);                                                \
                                                                               \
  dispatch(TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType);              \
  dispatch(TypeNode_String);                                                   \
                                                                               \
  dispatch(BasicTypeNode);                                                     \
  dispatch(BasicTypeNode_Integer);                                             \
  dispatch(BasicTypeNode_Real);                                                \
  dispatch(BasicTypeNode_Boolean);                                             \
  dispatch(BasicTypeNode_Char);                                                \
                                                                               \
  dispatch(PeriodNode);                                                        \
  dispatch(PeriodNode_Number_Dot_Dot_Number);                                  \
  dispatch(PeriodNode_Period_Comma_Number_Dot_Dot_Number);                     \
                                                                               \
  dispatch(VarDeclsNode);                                                      \
  dispatch(VarDeclsNode_Var_VarDecl_Semicolon);                                \
                                                                               \
  dispatch(VarDeclNode);                                                       \
  dispatch(VarDeclNode_IdList_Colon_Type);                                     \
  dispatch(VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type);                   \
                                                                               \
  dispatch(SubprogramDeclsNode);                                               \
  dispatch(SubprogramDeclsNode_SubprogramDecls_Subprogram);                    \
                                                                               \
  dispatch(SubprogramNode);                                                    \
                                                                               \
  dispatch(SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON);  \
                                                                               \
  dispatch(SubprogramHeadNode);                                                \
  dispatch(SubprogramHeadNode_Procedure_Id_FormalParameter);                   \
                                                                               \
  dispatch(SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType);    \
                                                                               \
  dispatch(FormalParameterNode);                                               \
  dispatch(FormalParameterNode_Lparen_ParameterList_Rparen);                   \
                                                                               \
  dispatch(ParameterListNode);                                                 \
  dispatch(ParameterListNode_Parameter);                                       \
  dispatch(ParameterListNode_ParameterList_Semicolon_Parameter);               \
                                                                               \
  dispatch(ParameterNode);                                                     \
  dispatch(ParameterNode_VarParameter);                                        \
  dispatch(ParameterNode_ValueParameter);                                      \
                                                                               \
  dispatch(VarParameterNode);                                                  \
  dispatch(VarParameterNode_Var_ValueParameter);                               \
                                                                               \
  dispatch(ValueParameterNode);                                                \
  dispatch(ValueParameterNode_IdList_Colon_BasicType);                         \
                                                                               \
  dispatch(SubprogramBodyNode);                                                \
  dispatch(SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement);          \
                                                                               \
  dispatch(CompoundStatementNode);                                             \
  dispatch(CompoundStatementNode_Begin_StatementList_End);                     \
                                                                               \
  dispatch(StatementListNode);                                                 \
  dispatch(StatementListNode_Statement);                                       \
  dispatch(StatementListNode_StatementList_Semicolon_Statement);               \
                                                                               \
  dispatch(StatementNode);                                                     \
  dispatch(StatementNode_Variable_Assignop_Expression);                        \
  dispatch(StatementNode_Id_Assignop_Expression);                              \
  dispatch(StatementNode_ProcedureCall);                                       \
  dispatch(StatementNode_If_Expression_Then_Statement_ElsePart);               \
  dispatch(                                                                    \
      StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement);    \
  dispatch(StatementNode_While_Expression_Do_Statement);                       \
  dispatch(StatementNode_Read_Lparen_VariableList_Rparen);                     \
  dispatch(StatementNode_Write_Lparen_ExpressionList_Rparen);                  \
  dispatch(StatementNode_CompoundStatement);                                   \
  dispatch(StatementNode_Break);                                               \
  dispatch(StatementNode_Continue);                                            \
                                                                               \
  dispatch(VariableListNode);                                                  \
  dispatch(VariableListNode_Variable);                                         \
  dispatch(VariableListNode_VariableList_Comma_Variable);                      \
                                                                               \
  dispatch(VariableNode);                                                      \
  dispatch(VariableNode_Id_IdVarpart);                                         \
                                                                               \
  dispatch(IdVarPartNode);                                                     \
  dispatch(IdVarPartNode_Lbracket_ExpressionList_Rbracket);                    \
                                                                               \
  dispatch(ProcedureCallNode);                                                 \
  dispatch(ProcedureCallNode_Id);                                              \
  dispatch(ProcedureCallNode_Id_Lparen_Rparen);                                \
  dispatch(ProcedureCallNode_Id_Lparen_ExpressionList_Rparen);                 \
                                                                               \
  dispatch(ElsePartNode);                                                      \
  dispatch(ElsePartNode_Else_Statement);                                       \
                                                                               \
  dispatch(ExpressionListNode);                                                \
  dispatch(ExpressionListNode_Expression);                                     \
  dispatch(ExpressionListNode_ExpressionList_Comma_Expression);                \
                                                                               \
  dispatch(ExpressionNode);                                                    \
  dispatch(ExpressionNode_SimpleExpression);                                   \
                                                                               \
  dispatch(ExpressionNode_SimpleExpression_Relop_SimpleExpression);            \
                                                                               \
  dispatch(SimpleExpressionNode);                                              \
  dispatch(SimpleExpressionNode_Term);                                         \
  dispatch(SimpleExpressionNode_SimpleExpression_Plus_Term);                   \
  dispatch(SimpleExpressionNode_SimpleExpression_Minus_Term);                  \
  dispatch(SimpleExpressionNode_SimpleExpression_Or_Term);                     \
                                                                               \
  dispatch(TermNode);                                                          \
  dispatch(TermNode_Factor);                                                   \
  dispatch(TermNode_Term_Mulop_Factor);                                        \
                                                                               \
  dispatch(FactorNode);                                                        \
  dispatch(FactorNode_Number);                                                 \
  dispatch(FactorNode_CharLiteral);                                            \
  dispatch(FactorNode_BoolLiteral);                                            \
  dispatch(FactorNode_Variable);                                               \
  dispatch(FactorNode_Lparen_Expression_Rparen);                               \
  dispatch(FactorNode_Not_Factor);                                             \
  dispatch(FactorNode_Plus_Factor);                                            \
  dispatch(FactorNode_Minus_Factor);                                           \
  dispatch(FactorNode_ID_Lparen_ExpressionList_Rparen);

#define dispatch_override(production)                                          \
  virtual void visit(class production &node) override

#define dispatch_virtual(production)                                           \
  virtual void visit(class production &node) = 0

#define impl_visit(class_type, node_type)                                      \
  void class_type::visit(class node_type &node)