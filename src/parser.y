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

%token <ASTNodePtr> LPAREN "left parenthesis";
%token <ASTNodePtr> RPAREN "right parenthesis";
%token <ASTNodePtr> LBRACKET "left bracket";
%token <ASTNodePtr> RBRACKET "right bracket";

%token <ASTNodePtr> PROGRAM VAR BEGIN END IF THEN ELSE
%token <ASTNodePtr> WHILE DO INTEGER REAL ASSIGN

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
{
    cout << "*** RUN ***" << endl;
    cout << endl << "prompt> ";
    driver.clear();
    $$ = std::make_shared<ASTNode>();
    /* driver.set_program($$); */
} | program_struct NUMBER {
    cout << "[Parse a Number] "<< endl;
} | {
    cout << "[Empty program]" << endl;
};

%%

// Bison expects us to provide implementation - otherwise linker complains
void XYZ::Parser::error(const location &loc , const std::string &message) {
    cout << "Error: " << message << endl << "Error location: " << driver.location() << endl;
}
