%{
    #include <iostream>
    #include <cstdlib>
    #include "scanner.h"
    #include "driver.h"
    #include "parser.hpp"
    #include "location.hh"
    #include "ast/ast.h"

    using namespace std;

    #define yyterminate() XYZ::Parser::make_END_FILE(XYZ::location());

    #define YY_USER_ACTION m_driver.increaseLocation(yyleng);

    using ASTNodePtr = std::shared_ptr<XYZ::ASTNode>;
%}

/* %option nodefault */
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="XYZ_"
/* 忽略大小写 */
%option caseless

/* 词法规则定义 */
WS [ \t\r\v\f]

LETTER [a-zA-Z]
DIGIT [0-9]
ID {LETTER}({LETTER}|{DIGIT})*

DIGITS {DIGIT}+

OPTIONAL_FRACTION (\.{DIGITS})?
NUM {DIGITS}{OPTIONAL_FRACTION}
CHAR_LITERAL ('{LETTER}')

/* KEY WORD */
PROGRAM "program"
VAR "var"
BEGIN "begin"
END "end"
IF "if"
THEN "then"
ELSE "else"
WHILE "while"
DO "do"
INTEGER "integer"
REAL "real"
CHAR "char"
BOOLEAN "boolean"
STRING "string"
CONST "const"
ARRAY "array"
OF "of"
PROCEDURE "procedure"
FUNCTION "function"
FOR "for"
RECORD "record"
TYPE "type"
LABEL "label"
CASE "case"
GOTO "goto"
TO "to"
READ "read"
WRITE "write"
NOT "not"


RELOP (<=|>=|<>|<|>|=)
PLUS \+
MINUS \-
OR or
ADDOP ({PLUS}|{MINUS}|{OR})
MULOP (div|mod|and|\*|\/)
ASSIGNOP :=

COMMA ,
SEMICOLON ;
COLON :
DOT \.
QUOTE '
LPAREN \(
RPAREN \)
LBRACKET \[
RBRACKET \]

/* 注释 */

LINE_COMMENT \/\/[^\n]*

%x BRACE_COMMENT STAR_COMMENT

%%
{LINE_COMMENT}    { /* 忽略行注释 */
    // cout << "Scanner: line comment [" << yytext << "]" << endl;
    // m_driver.increaseLine();
}
"{"                { /* Start brace comment */
    BEGIN(BRACE_COMMENT);
    // cout << "Scanner: brace comment start {" << endl;
}
"(*"               { /* Start star comment */
    BEGIN(STAR_COMMENT);
    // cout << "Scanner: star comment start (*" << endl;
}

<BRACE_COMMENT>{
"}"             { BEGIN(INITIAL); /* cout << "Scanner: brace comment end }" << endl; */ }
[^{}\n]+        { /* Ignore content within brace comment */ /* cout << "Scanner: brace comment content [" << yytext << "]" << endl; */ }
\n              { m_driver.increaseLine(); /* cout << "Scanner: brace comment newline" << endl; */ }
<<EOF>>         {  BEGIN(INITIAL); }
}

<STAR_COMMENT>{
"*)"            { BEGIN(INITIAL); /* cout << "Scanner: star comment end *)" << endl; */ }
[^*\n]+         { /* Ignore non-* content within star comment */ /* cout << "Scanner: star comment content [" << yytext << "]" << endl; */ }
\n              { m_driver.increaseLine(); /* cout << "Scanner: star comment newline" << endl; */ }
"*"+[^)*\n]     { /* Consume '*' followed by non-) non-* non-newline */ /* cout << "Scanner: star comment '*' sequence [" << yytext << "]" << endl; */}
"*"             { /* Consume single '*' not followed by ')' */ /* cout << "Scanner: star comment single '*'" << endl; */ }
<<EOF>>         {  BEGIN(INITIAL); }
}

\n {
    m_driver.increaseLine();
    // cout << "\n" << endl;
    // TODO: 处理换行
}
{WS}    { /* Ignore whitespace */
    // cout << "空白符" << endl;
}
{NUM}    {
    cout << "Scanner: number [" << yytext << "]" << endl;
    // uint64_t number = strtoull(yytext, 0, 10);
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_NUMBER(ptr,XYZ::location());
}
{CHAR_LITERAL} {
    cout << "Scanner: char_literal [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_CHAR_LITERAL(ptr,XYZ::location());
}
{PROGRAM}    {
    cout << "Scanner: program [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_PROGRAM(ptr, XYZ::location());
}
{VAR}    {
    cout << "Scanner: var [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_VAR(ptr, XYZ::location());
}
{BEGIN}    {
    cout << "Scanner: begin [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_BEGIN(ptr, XYZ::location());
}
{END}    {
    cout << "Scanner: end [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_END(ptr, XYZ::location());
}
{IF}    {
    cout << "Scanner: if [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_IF(ptr, XYZ::location());
}
{THEN}    {
    cout << "Scanner: then [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_THEN(ptr, XYZ::location());
}
{ELSE}    {
    cout << "Scanner: else [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_ELSE(ptr, XYZ::location());
}
{WHILE}    {
    cout << "Scanner: while [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_WHILE(ptr, XYZ::location());
}
{DO}    {
    cout << "Scanner: do [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_DO(ptr, XYZ::location());
}
{INTEGER}    {
    cout << "Scanner: integer [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_INTEGER(ptr, XYZ::location());
}
{REAL}    {
    cout << "Scanner: real [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_REAL(ptr, XYZ::location());
}
{CONST}    {
    cout << "Scanner: const [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_CONST(ptr, XYZ::location());
}
{ARRAY}    {
    cout << "Scanner: array [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_ARRAY(ptr, XYZ::location());
}
{OF}    {
    cout << "Scanner: of [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_OF(ptr, XYZ::location());
}
{PROCEDURE}    {
    cout << "Scanner: procedure [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_PROCEDURE(ptr, XYZ::location());
}
{FUNCTION}    {
    cout << "Scanner: function [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_FUNCTION(ptr, XYZ::location());
}
{FOR}    {
    cout << "Scanner: for [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_FOR(ptr, XYZ::location());
}
{RECORD}    {
    cout << "Scanner: record [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_RECORD(ptr, XYZ::location());
}
{TYPE}    {
    cout << "Scanner: type [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_TYPE(ptr, XYZ::location());
}
{LABEL}    {
    cout << "Scanner: label [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_LABEL(ptr, XYZ::location());
}
{CASE}    {
    cout << "Scanner: case [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_CASE(ptr, XYZ::location());
}
{GOTO}    {
    cout << "Scanner: goto [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_GOTO(ptr, XYZ::location());
}
{TO}    {
    cout << "Scanner: to [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_TO(ptr, XYZ::location());
}
{READ}    {
    cout << "Scanner: read [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_READ(ptr, XYZ::location());
}
{WRITE}    {
    cout << "Scanner: write [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_WRITE(ptr, XYZ::location());
}
{NOT}    {
    cout << "Scanner: not [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_NOT(ptr, XYZ::location());
}
{CHAR}    {
    cout << "Scanner: char [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_CHAR(ptr, XYZ::location());
}
{BOOLEAN}    {
    cout << "Scanner: boolean [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_BOOLEAN(ptr, XYZ::location());
}
{STRING}    {
    cout << "Scanner: string [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_STRING(ptr, XYZ::location());
}
{OR}    {
    cout << "Scanner: or [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_OR(ptr, XYZ::location());
}

{ID}    {
    /* 注: KEYWORD 需要在 ID 之前，否则会被匹配为 ID */
    cout << "Scanner: id [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_ID(ptr, XYZ::location());
}
{RELOP}    {
    cout << "Scanner: relop [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_RELOP(ptr, XYZ::location());
}
{PLUS}    {
    cout << "Scanner: plus [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_PLUS(ptr, XYZ::location());
}
{MINUS}    {
    cout << "Scanner: minus [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_MINUS(ptr, XYZ::location());
}
{MULOP}    {
    cout << "Scanner: mulop [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_MULOP(ptr, XYZ::location());
}
{ASSIGNOP}    {
    cout << "Scanner: assignop [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_ASSIGNOP(ptr, XYZ::location());
}
{COMMA}    {
    cout << "Scanner: comma [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_COMMA(ptr, XYZ::location());
}
{SEMICOLON}    {
    cout << "Scanner: semicolon [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_SEMICOLON(ptr, XYZ::location());
}
{COLON}    {
    cout << "Scanner: colon [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_COLON(ptr, XYZ::location());
}
{DOT}    {
    cout << "Scanner: dot [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_DOT(ptr, XYZ::location());
}
{QUOTE}    {
    cout << "Scanner: quote [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_QUOTE(ptr, XYZ::location());
}
{LPAREN}    {
    cout << "Scanner: lparen [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_LPAREN(ptr, XYZ::location());
}
{RPAREN}    {
    cout << "Scanner: rparen [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_RPAREN(ptr, XYZ::location());
}
{LBRACKET}    {
    cout << "Scanner: lbracket [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_LBRACKET(ptr, XYZ::location());
}
{RBRACKET}    {
    cout << "Scanner: rbracket [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_RBRACKET(ptr, XYZ::location());
}


<<EOF>>     { return yyterminate(); }

.           {
    cout << "Scanner: unrecognized character [" << yytext << "]" << endl;
}

%%
