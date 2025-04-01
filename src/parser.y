%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines
%define api.parser.class { Parser }

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
    #include "ast.h"

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

    static XYZ::Parser::symbol_type yylex(XYZ::Scanner &scanner, XYZ::Driver &driver) {
        return scanner.get_next_token();
    }

    using namespace XYZ;
}

%lex-param { XYZ::Scanner &scanner }
%lex-param { XYZ::Driver &driver }
%parse-param { XYZ::Scanner &scanner }
%parse-param { XYZ::Driver &driver }
%locations
%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOKEN_}

/* TOKENS */
%token END_FILE 0 "end of file"


%token <ASTNodePtr> NUMBER "number";
%token <ASTNodePtr> ID "id";
%token <ASTNodePtr> RELOP "relational operator";
%token <ASTNodePtr> ADDOP "addition operator";
%token <ASTNodePtr> MULOP "multiplication operator";
%token <ASTNodePtr> ASSIGNOP "assignment operator";

%token <ASTNodePtr> COMMA "comma";
%token <ASTNodePtr> SEMICOLON "semicolon";
%token <ASTNodePtr> COLON "colon";
%token <ASTNodePtr> DOT "dot";
%token <ASTNodePtr> TO "to";
%token <ASTNodePtr> READ "read";
%token <ASTNodePtr> WRITE "write";
%token <ASTNodePtr> NOT "not";
%token <ASTNodePtr> UMINUS "unary minus";

%token <ASTNodePtr> LPAREN "left parenthesis";
%token <ASTNodePtr> RPAREN "right parenthesis";
%token <ASTNodePtr> LBRACKET "left bracket";
%token <ASTNodePtr> RBRACKET "right bracket";

%token <ASTNodePtr> PROGRAM VAR BEGIN END IF THEN ELSE
%token <ASTNodePtr> WHILE DO ASSIGN CONST
%token <ASTNodePtr> ARRAY OF PROCEDURE FUNCTION
%token <ASTNodePtr> FOR RECORD TYPE LABEL CASE GOTO
%token <ASTNodePtr> CHAR BOOLEAN STRING INTEGER REAL
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
%left ADDOP
%left MULOP
%left COMMA COLON DOT
%left LBRACKET RBRACKET
%left LPAREN RPAREN
%nonassoc END IF THEN ELSE WHILE DO

%%
program_struct :
    program_head SEMICOLON program_body DOT {
        cout << "program_struct -> program_head SEMICOLON program_body DOT" << endl;
    }
;
program_head :
    PROGRAM ID LPAREN idlist RPAREN {
        cout << "program_head -> PROGRAM ID LPAREN idlist RPAREN" << endl;
    }|
    PROGRAM ID {
        cout << "program_head -> PROGRAM ID" << endl;
    }
;
program_body :
    const_decls var_decls subprogram_decls compound_statement {
        cout << "program_body -> const_decls var_decls subprogram_decls compound_statement" << endl;
    }
;
idlist:
    ID {
        cout << "idlist -> ID" << endl;
    }|
    idlist COMMA ID {
        cout << "idlist -> idlist COMMA ID" << endl;
    }
;
const_decls : {
        cout << "const_decls -> EMPTY" << endl;
    }|
    CONST const_decl {
        cout << "const_decls -> CONST const_decl" << endl;
    }
;
const_decl :
    ID ASSIGN const_val {
        cout << "const_decl -> ID ASSIGN const_val" << endl;
    }|
    const_decl SEMICOLON ID ASSIGN const_val {
        cout << "const_decl -> const_decl SEMICOLON ID ASSIGN const_val" << endl;
    }
;
const_val :
    ADDOP NUMBER {
        cout << "const_val -> ADDOP NUMBER" << endl;
    }|
    NUMBER {
        cout << "const_val -> NUMBER" << endl;
    }
    /* TODO: 查阅 pascal const letter 定义 */
    /* |' letter ' {
        cout << "' letter '" << endl;
    } */
;
var_decls :
    {
        cout << "var_decls -> EMPTY" << endl;
    }|
    VAR var_decl SEMICOLON{
        cout << "var_decls -> VAR var_decl" << endl;
    }
;
var_decl :
    idlist COLON type {
        cout << "var_decl -> idlist COLON type" << endl;
    }|
    var_decl SEMICOLON idlist COLON type {
        cout << "var_decl -> var_decl SEMICOLON idlist COLON type" << endl;
    }
;
type :
    basic_type {
        cout << "type -> basic_type" << endl;
    }|
    ARRAY LBRACKET period RBRACKET OF basic_type {
        cout << "type -> ARRAY LBRACKET period RBRACKET OF basic_type" << endl;
    }
;
basic_type :
    INTEGER {
        cout << "basic_type -> INTEGER" << endl;
    }|
    REAL {
        cout << "basic_type -> REAL" << endl;
    }|
    BOOLEAN {
        cout << "basic_type -> BOOLEAN" << endl;
    }|
    CHAR {
        cout << "basic_type -> CHAR" << endl;
    }
;
period :
    NUMBER DOT DOT NUMBER {
        cout << "period -> NUMBER DOT DOT NUMBER" << endl;
    }|
    period COMMA NUMBER DOT DOT NUMBER {
        cout << "period -> period COMMA NUMBER DOT DOT NUMBER" << endl;
    }
;
subprogram_decls :
    {
        cout << "subprogram_decls -> EMPTY" << endl;
    }|
    subprogram_decls subprogram {
        cout << "subprogram_decls -> subprogram_decls subprogram" << endl;
    }
;
subprogram :
    subprogram_head SEMICOLON subprogram_body {
        cout << "subprogram -> subprogram_head SEMICOLON subprogram_body" << endl;
    }
;
subprogram_head :
    PROCEDURE ID formal_parameter {
        cout << "subprogram_head -> PROCEDURE ID formal_parameter" << endl;
    }|
    FUNCTION ID formal_parameter COLON basic_type {
        cout << "subprogram_head -> FUNCTION ID formal_parameter COLON basic_type" << endl;
    }
;
formal_parameter :{
        cout << "formal_parameter -> EMPTY" << endl;
    }|
    LPAREN parameter_list RPAREN {
        cout << "formal_parameter -> LPAREN parameter_list RPAREN" << endl;
    }
;
parameter_list :
    parameter {
        cout << "parameter_list -> parameter" << endl;
    }|
    parameter_list SEMICOLON parameter {
        cout << "parameter_list -> parameter_list SEMICOLON parameter" << endl;
    }
;
parameter :
    var_parameter {
        cout << "parameter -> var_parameter" << endl;
    }|
    value_parameter {
        cout << "parameter -> value_parameter" << endl;
    }
;
var_parameter :
    VAR value_parameter {
        cout << "var_parameter -> VAR value_parameter" << endl;
    }
;
value_parameter :
    idlist COLON basic_type {
        cout << "value_parameter -> idlist COLON basic_type" << endl;
    }
;
subprogram_body :
    const_decls var_decls compound_statement {
        cout << "subprogram_body -> const_decls var_decls compound_statement" << endl;
    }
;
compound_statement :
    BEGIN statement_list END {
        cout << "compound_statement -> BEGIN statement_list END" << endl;
    }
;
statement_list :
    statement {
        cout << "statement_list -> statement" << endl;
    }|
    statement_list SEMICOLON statement {
        cout << "statement_list -> statement_list SEMICOLON statement" << endl;
    }
;
statement : {
        cout << "statement -> EMPTY" << endl;
    }|
    variable ASSIGNOP expression {
        cout << "statement -> variable ASSIGNOP expression" << endl;
    }|
    ID ASSIGNOP expression {
        cout << "statement -> ID ASSIGNOP expression" << endl;
    }|
    procedure_call {
        cout << "statement -> procedure_call" << endl;
    }|
    IF expression THEN statement else_part {
        cout << "statement -> IF expression THEN statement else_part" << endl;
    }|
    FOR ID ASSIGN expression TO expression DO statement {
        cout << "statement -> FOR ID ASSIGN expression TO expression DO statement" << endl;
    }|
    READ LPAREN variable_list RPAREN {
        cout << "statement -> READ LPAREN variable_list RPAREN" << endl;
    }|
    WRITE LPAREN expression_list RPAREN {
        cout << "statement -> WRITE LPAREN expression_list RPAREN" << endl;
    }
;
variable_list :
    variable {
        cout << "variable_list -> variable" << endl;
    }|
    variable_list COMMA variable {
        cout << "variable_list -> variable_list COMMA variable" << endl;
    }
;
variable :
    ID id_varpart {
        cout << "variable -> ID id_varpart" << endl;
    }
;
id_varpart :
    {
        cout << "id_varpart -> EMPTY" << endl;
    }|
    LBRACKET expression_list RBRACKET  {
        cout << "id_varpart -> LBRACKET expression_list RBRACKET" << endl;
    }
;
procedure_call :
    ID {
        cout << "procedure_call -> ID" << endl;
    }|
    ID LPAREN expression_list RPAREN {
        cout << "procedure_call -> ID LPAREN expression_list RPAREN" << endl;
    }
;
else_part :
    {
        cout << "else_part -> EMPTY" << endl;
    }|
    ELSE statement {
        cout << "else_part -> ELSE statement" << endl;
    }
;
expression_list :
    expression {
        cout << "expression_list -> expression" << endl;
    }|
    expression_list COMMA expression {
        cout << "expression_list -> expression_list COMMA expression" << endl;
    }
;
expression :
    simple_expression {
        cout << "expression -> simple_expression" << endl;
    }|
    simple_expression RELOP simple_expression {
        cout << "expression -> simple_expression RELOP simple_expression" << endl;
    }
;
simple_expression :
    term {
        cout << "simple_expression -> term" << endl;
    }|
    simple_expression ADDOP term {
        cout << "simple_expression -> simple_expression ADDOP term" << endl;
    }
;
term :
    factor {
        cout << "term -> factor" << endl;
    }|
    term MULOP factor {
        cout << "term -> term MULOP factor" << endl;
    }
;
factor :
    NUMBER {
        cout << "factor -> NUMBER" << endl;
    }|
    variable {
        cout << "factor -> variable" << endl;
    }|
    LPAREN expression RPAREN {
        cout << "factor -> LPAREN expression RPAREN" << endl;
    }|
    ID LPAREN expression_list RPAREN {
        cout << "factor -> ID LPAREN expression_list RPAREN" << endl;
    }|
    NOT factor {
        cout << "factor -> NOT factor" << endl;
    }|
    UMINUS factor {
        cout << "factor -> UMINUS factor" << endl;
    }
;
%%


// Bison expects us to provide implementation - otherwise linker complains
void XYZ::Parser::error(const location &loc , const std::string &message) {
    cout << "Error: " << message << endl << "Error location: " << driver.location() << endl;
}
