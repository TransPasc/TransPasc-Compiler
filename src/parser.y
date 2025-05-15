%skeleton "lalr1.cc" /* -*- C++ -*- */
/* %require "3.0" */
%defines
%define api.parser.class { Parser }
/* %define api.parser.class "Parser" */

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { XYZ }

%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>
    #include "ast/ast.hpp"

    using namespace std;

    namespace XYZ {
        class Scanner;
        class Driver;
    }
    using namespace XYZ;

    using ASTNodePtr = std::shared_ptr<ASTNode>;

}

%code top
{
    #include <iostream>
    #include "scanner.h"
    #include "parser.hpp"
    #include "driver.h"
    #include "location.hh"
    #include "ast/ast.hpp"

    static XYZ::Parser::symbol_type yylex(XYZ::Scanner &scanner, XYZ::Driver &driver) {
        return scanner.get_next_token();
    }

    using namespace XYZ;
    using ASTNodePtr = std::shared_ptr<ASTNode>;

    int yydebug = 1;
}

%lex-param { XYZ::Scanner &scanner }
%lex-param { XYZ::Driver &driver }
%parse-param { XYZ::Scanner &scanner }
%parse-param { XYZ::Driver &driver }
%locations
/* %define parse.trace */
%define parse.error verbose

%define api.token.prefix {TOKEN_}

/* TOKENS */
%token END_FILE 0 "end of file"


%token <ASTNodePtr> NUMBER "number";
%token <ASTNodePtr> CHAR_LITERAL "char_literal";
%token <ASTNodePtr> ID "id";
%token <ASTNodePtr> RELOP "relational operator";
%token <ASTNodePtr> PLUS "plus";
%token <ASTNodePtr> MINUS "minus";
%token <ASTNodePtr> OR "or";
%token <ASTNodePtr> MULOP "multiplication operator";
%token <ASTNodePtr> ASSIGNOP "assignment operator";

%token <ASTNodePtr> COMMA "comma";
%token <ASTNodePtr> SEMICOLON "semicolon";
%token <ASTNodePtr> COLON "colon";
%token <ASTNodePtr> DOT "dot";
%token <ASTNodePtr> QUOTE "quote";
%token <ASTNodePtr> TO "to";
%token <ASTNodePtr> READ "read";
%token <ASTNodePtr> WRITE "write";
%token <ASTNodePtr> NOT "not";

%token <ASTNodePtr> LPAREN "left parenthesis";
%token <ASTNodePtr> RPAREN "right parenthesis";
%token <ASTNodePtr> LBRACKET "left bracket";
%token <ASTNodePtr> RBRACKET "right bracket";

%token <ASTNodePtr> PROGRAM VAR BEGIN END IF THEN ELSE
%token <ASTNodePtr> WHILE DO CONST
%token <ASTNodePtr> ARRAY OF PROCEDURE FUNCTION
%token <ASTNodePtr> FOR RECORD TYPE LABEL CASE GOTO
%token <ASTNodePtr> CHAR BOOLEAN STRING INTEGER REAL
%token <ASTNodePtr> BOOL_LITERAL

/* node */
%type <ASTNodePtr> program_struct
%type <ASTNodePtr> program_head
%type <ASTNodePtr> program_body
%type <ASTNodePtr> idlist

%type <ASTNodePtr> const_decls
%type <ASTNodePtr> const_decl
%type <ASTNodePtr> const_val

%type <ASTNodePtr> var_decls
%type <ASTNodePtr> var_decl

%type <ASTNodePtr> type
%type <ASTNodePtr> basic_type
%type <ASTNodePtr> period

%type <ASTNodePtr> subprogram_decls
%type <ASTNodePtr> subprogram
%type <ASTNodePtr> subprogram_head
%type <ASTNodePtr> subprogram_body

%type <ASTNodePtr> formal_parameter
%type <ASTNodePtr> parameter_list
%type <ASTNodePtr> parameter
%type <ASTNodePtr> var_parameter
%type <ASTNodePtr> value_parameter

%type <ASTNodePtr> compound_statement
%type <ASTNodePtr> statement_list
%type <ASTNodePtr> statement

%type <ASTNodePtr> variable_list
%type <ASTNodePtr> variable
%type <ASTNodePtr> id_varpart

%type <ASTNodePtr> procedure_call
%type <ASTNodePtr> else_part
%type <ASTNodePtr> expression_list
%type <ASTNodePtr> expression
%type <ASTNodePtr> simple_expression

%type <ASTNodePtr> term
%type <ASTNodePtr> factor

%start program_struct

/* 定义优先级，从低到高 */
/* TODO: 调整优先级顺序 */
%right ASSIGNOP
%left RELOP
%left PLUS MINUS OR
%left MULOP
%left COMMA COLON DOT
%left LBRACKET RBRACKET
%left LPAREN RPAREN
%nonassoc END IF THEN ELSE WHILE DO
%right UMINUS

%%
program_struct :
    program_head SEMICOLON program_body DOT {
        Driver::root = std::make_shared<ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot>(
            $1, $2, $3, $4, @1.begin.line);
        $$ = Driver::root;
    }
;
program_head :
    PROGRAM ID LPAREN idlist RPAREN {
        $$ = std::make_shared<ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen>(
            $1, $2, $3, $4, $5, @1.begin.line);
    } |
    PROGRAM ID {
        $$ = std::make_shared<ProgramHeadNode_Program_Id>(
            $1, $2, @1.begin.line);
    }
;
program_body :
    const_decls var_decls subprogram_decls compound_statement {
        $$ = std::make_shared<ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement>(
            $1, $2, $3, $4, @1.begin.line);
    }
;
idlist:
    ID {
        $$ = std::make_shared<IdListNode_Id>($1, @1.begin.line);
    } |
    idlist COMMA ID {
        $$ = std::make_shared<IdListNode_IdList_Comma_Id>(
            $1, $2, $3, @1.begin.line);
    }
;
const_decls : {
        $$ = std::make_shared<ConstDeclsNode>(@$.begin.line);
    } |
    CONST const_decl {
        $$ = std::make_shared<ConstDeclsNode_Const_ConstDecl>(
            $1, $2, @1.begin.line);
    }
;
const_decl :
    /*
        Pascal也太愚蠢了吧，初始化和 eq 混用 '=' 号
        综合考虑下，就先用 RELOP 吧
    */
    ID RELOP const_val SEMICOLON{
        $$ = std::make_shared<ConstDeclNode_Id_Relop_ConstVal_Semicolon>(
            $1, $2, $3, $4, @1.begin.line);
    } |
    const_decl ID RELOP const_val SEMICOLON{
        $$ = std::make_shared<ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon>(
            $1, $2, $3, $4, $5, @1.begin.line);
    }
;
const_val :
    PLUS NUMBER {
        $$ = std::make_shared<ConstValNode_Plus_Number>($1, $2, @1.begin.line);
    } |
    MINUS NUMBER {
        $$ = std::make_shared<ConstValNode_Minus_Number>($1, $2, @1.begin.line);
    } |
    NUMBER {
        $$ = std::make_shared<ConstValNode_Number>($1, @1.begin.line);
    } |
    CHAR_LITERAL {
        $$ = std::make_shared<ConstValNode_CharLiteral>($1, @1.begin.line);
    }
;
var_decls :
    {
        $$ = std::make_shared<VarDeclsNode>(@$.begin.line);
    } |
    VAR var_decl SEMICOLON {
        $$ = std::make_shared<VarDeclsNode_Var_VarDecl_Semicolon>(
            $1, $2, $3, @1.begin.line);
    }
;
var_decl :
    idlist COLON type {
        $$ = std::make_shared<VarDeclNode_IdList_Colon_Type>(
            $1, $2, $3, @1.begin.line);
    } |
    var_decl SEMICOLON idlist COLON type {
        $$ = std::make_shared<VarDeclNode_VarDecl_Semicolon_IdList_Colon_Type>(
            $1, $2, $3, $4, $5, @1.begin.line);
    }
;
type :
    basic_type {
        $$ = std::make_shared<TypeNode_BasicType>($1, @1.begin.line);
    } |
    ARRAY LBRACKET period RBRACKET OF basic_type {
        $$ = std::make_shared<TypeNode_Array_Lbracket_Period_Rbracket_Of_BasicType>(
            $1, $2, $3, $4, $5, $6, @1.begin.line);
    }
;
basic_type :
    INTEGER {
        $$ = std::make_shared<BasicTypeNode_Integer>($1, @1.begin.line);
    } |
    REAL {
        $$ = std::make_shared<BasicTypeNode_Real>($1, @1.begin.line);
    } |
    BOOLEAN {
        $$ = std::make_shared<BasicTypeNode_Boolean>($1, @1.begin.line);
    } |
    CHAR {
        $$ = std::make_shared<BasicTypeNode_Char>($1, @1.begin.line);
    }
;
period :
    NUMBER DOT DOT NUMBER {
        $$ = std::make_shared<PeriodNode_Number_Dot_Dot_Number>(
            $1, $2, $3, $4, @1.begin.line);
    } |
    period COMMA NUMBER DOT DOT NUMBER {
        $$ = std::make_shared<PeriodNode_Period_Comma_Number_Dot_Dot_Number>(
            $1, $2, $3, $4, $5, $6, @1.begin.line);
    }
;
subprogram_decls :
    {
        $$ = std::make_shared<SubprogramDeclsNode>(@$.begin.line);
    } |
    subprogram_decls subprogram {
        $$ = std::make_shared<SubprogramDeclsNode_SubprogramDecls_Subprogram>(
            $1, $2, @1.begin.line);
    }
;
subprogram :
    /* 又给错误的文法规则 */
    subprogram_head SEMICOLON subprogram_body SEMICOLON {
        $$ = std::make_shared<SubprogramNode_SubprogramHead_Semicolon_SubprogramBody_SEMICOLON>(
            $1, $2, $3, $4, @1.begin.line);
    }
;
subprogram_head :
    PROCEDURE ID formal_parameter {
        $$ = std::make_shared<SubprogramHeadNode_Procedure_Id_FormalParameter>(
            $1, $2, $3, @1.begin.line);
    } |
    FUNCTION ID formal_parameter COLON basic_type {
        $$ = std::make_shared<SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType>(
            $1, $2, $3, $4, $5, @1.begin.line);
    }
;
formal_parameter :{
        $$ = std::make_shared<FormalParameterNode>(@$.begin.line);
    } |
    LPAREN parameter_list RPAREN {
        $$ = std::make_shared<FormalParameterNode_Lparen_ParameterList_Rparen>(
            $1, $2, $3, @1.begin.line);
    }
;
parameter_list :
    parameter {
        $$ = std::make_shared<ParameterListNode_Parameter>($1, @1.begin.line);
    } |
    parameter_list SEMICOLON parameter {
        $$ = std::make_shared<ParameterListNode_ParameterList_Semicolon_Parameter>(
            $1, $2, $3, @1.begin.line);
    }
;
parameter :
    var_parameter {
        $$ = std::make_shared<ParameterNode_VarParameter>($1, @1.begin.line);
    } |
    value_parameter {
        $$ = std::make_shared<ParameterNode_ValueParameter>($1, @1.begin.line);
    }
;
var_parameter :
    VAR value_parameter {
        $$ = std::make_shared<VarParameterNode_Var_ValueParameter>(
            $1, $2, @1.begin.line);
    }
;
value_parameter :
    idlist COLON basic_type {
        $$ = std::make_shared<ValueParameterNode_IdList_Colon_BasicType>(
            $1, $2, $3, @1.begin.line);
    }
;
subprogram_body :
    const_decls var_decls compound_statement {
        $$ = std::make_shared<SubprogramBodyNode_ConstDecls_VarDecls_CompoundStatement>(
            $1, $2, $3, @1.begin.line);
    }
;
compound_statement :
    BEGIN statement_list END{
        $$ = std::make_shared<CompoundStatementNode_Begin_StatementList_End>(
            $1, $2, $3, @1.begin.line);
    }
;
statement_list :
    statement {
        $$ = std::make_shared<StatementListNode_Statement>($1, @1.begin.line);
    } |
    statement_list SEMICOLON statement {
        $$ = std::make_shared<StatementListNode_StatementList_Semicolon_Statement>(
            $1, $2, $3, @1.begin.line);
    }
;
statement : {
        $$ = std::make_shared<StatementNode>(@$.begin.line);
    } |
    variable ASSIGNOP expression {
        $$ = std::make_shared<StatementNode_Variable_Assignop_Expression>(
            $1, $2, $3, @1.begin.line);
    } |
    ID ASSIGNOP expression {
        $$ = std::make_shared<StatementNode_Id_Assignop_Expression>(
            $1, $2, $3, @1.begin.line);
    } |
    procedure_call {
        $$ = std::make_shared<StatementNode_ProcedureCall>($1, @1.begin.line);
    } |
    IF expression THEN statement else_part {
        $$ = std::make_shared<StatementNode_If_Expression_Then_Statement_ElsePart>(
            $1, $2, $3, $4, $5, @1.begin.line);
    } |
    FOR ID ASSIGNOP expression TO expression DO statement {
        $$ = std::make_shared<StatementNode_For_Id_Assignop_Expression_To_Expression_Do_Statement>(
            $1, $2, $3, $4, $5, $6, $7, $8, @1.begin.line);
    } |
    READ LPAREN variable_list RPAREN {
        $$ = std::make_shared<StatementNode_Read_Lparen_VariableList_Rparen>(
            $1, $2, $3, $4, @1.begin.line);
    } |
    WRITE LPAREN expression_list RPAREN {
        $$ = std::make_shared<StatementNode_Write_Lparen_ExpressionList_Rparen>(
            $1, $2, $3, $4, @1.begin.line);
    } |
    /* 支持嵌套 */
    compound_statement {
        $$ = std::make_shared<StatementNode_CompoundStatement>($1, @1.begin.line);
    }
;
variable_list :
    variable {
        $$ = std::make_shared<VariableListNode_Variable>($1, @1.begin.line);
    } |
    variable_list COMMA variable {
        $$ = std::make_shared<VariableListNode_VariableList_Comma_Variable>(
            $1, $2, $3, @1.begin.line);
    }
;
variable :
    ID id_varpart {
        $$ = std::make_shared<VariableNode_Id_IdVarpart>($1, $2, @1.begin.line);
    }
;
id_varpart :
    {
        $$ = std::make_shared<IdVarPartNode>(@$.begin.line);
    } |
    LBRACKET expression_list RBRACKET  {
        $$ = std::make_shared<IdVarPartNode_Lbracket_ExpressionList_Rbracket>(
            $1, $2, $3, @1.begin.line);
    }
;
procedure_call :
    ID {
        $$ = std::make_shared<ProcedureCallNode_Id>($1, @1.begin.line);
    }|
    ID LPAREN RPAREN {
        $$ = std::make_shared<ProcedureCallNode_Id_Lparen_Rparen>($1, $2, $3, @1.begin.line);
    } |
    ID LPAREN expression_list RPAREN {
        $$ = std::make_shared<ProcedureCallNode_Id_Lparen_ExpressionList_Rparen>(
            $1, $2, $3, $4, @1.begin.line);
    }
;
else_part :
    {
        $$ = std::make_shared<ElsePartNode>(@$.begin.line);
    } |
    ELSE statement {
        $$ = std::make_shared<ElsePartNode_Else_Statement>(
            $1, $2, @1.begin.line);
    }
;
expression_list :
    expression {
        $$ = std::make_shared<ExpressionListNode_Expression>($1, @1.begin.line);
    } |
    expression_list COMMA expression {
        $$ = std::make_shared<ExpressionListNode_ExpressionList_Comma_Expression>(
            $1, $2, $3, @1.begin.line);
    }
;
expression :
    simple_expression {
        $$ = std::make_shared<ExpressionNode_SimpleExpression>($1, @1.begin.line);
    } |
    simple_expression RELOP simple_expression {
        $$ = std::make_shared<ExpressionNode_SimpleExpression_Relop_SimpleExpression>(
            $1, $2, $3, @1.begin.line);
    }
;
simple_expression :
    term {
        $$ = std::make_shared<SimpleExpressionNode_Term>($1, @1.begin.line);
    } |
    simple_expression PLUS term {
        $$ = std::make_shared<SimpleExpressionNode_SimpleExpression_Plus_Term>(
            $1, $2, $3, @1.begin.line);
    } |
    simple_expression MINUS term {
        $$ = std::make_shared<SimpleExpressionNode_SimpleExpression_Minus_Term>(
            $1, $2, $3, @1.begin.line);
    } |
    simple_expression OR term {
        $$ = std::make_shared<SimpleExpressionNode_SimpleExpression_Or_Term>(
            $1, $2, $3, @1.begin.line);
    }
;
term :
    factor {
        $$ = std::make_shared<TermNode_Factor>($1, @1.begin.line);
    } |
    term MULOP factor {
        $$ = std::make_shared<TermNode_Term_Mulop_Factor>(
            $1, $2, $3, @1.begin.line);
    }
;
factor :
    NUMBER {
        $$ = std::make_shared<FactorNode_Number>($1, @1.begin.line);
    } |
    CHAR_LITERAL {
        $$ = std::make_shared<FactorNode_CharLiteral>($1, @1.begin.line);
    } |
    BOOL_LITERAL {
        $$ = std::make_shared<FactorNode_BoolLiteral>($1, @1.begin.line);
    } |
    variable {
        $$ = std::make_shared<FactorNode_Variable>($1, @1.begin.line);
    } |
    LPAREN expression RPAREN {
        $$ = std::make_shared<FactorNode_Lparen_Expression_Rparen>(
            $1, $2, $3, @1.begin.line);
    } |
    ID LPAREN expression_list RPAREN {
        $$ = std::make_shared<FactorNode_ID_Lparen_ExpressionList_Rparen>(
            $1, $2, $3, $4, @1.begin.line);
    } |
    NOT factor {
        $$ = std::make_shared<FactorNode_Not_Factor>($1, $2, @1.begin.line);
    } |
    MINUS factor %prec UMINUS {
        $$ = std::make_shared<FactorNode_Minus_Factor>(
            $1, $2, @1.begin.line);
    }
;
%%


// Bison expects us to provide implementation - otherwise linker complains
void XYZ::Parser::error(const location &loc , const std::string &message) {

    driver.handleError(message, loc);
}
