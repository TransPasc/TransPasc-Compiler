%{
    #include <iostream>
    #include <cstdlib>
    #include "scanner.h"
    #include "driver.h"
    #include "parser.hpp"
    #include "location.hh"
    #include "ast.h"

    using namespace std;

    #define yyterminate() XYZ::Parser::make_END_FILE(XYZ::location());

    #define YY_USER_ACTION m_driver.increaseLocation(yyleng);

    using ASTNodePtr = std::shared_ptr<XYZ::ASTNode>;
%}

%option nodefault
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
ASSIGN ":="
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
ADDOP (\+|\-|or)
MULOP (div|mod|and|\*|\/)
ASSIGNOP :=

COMMA ,
SEMICOLON ;
COLON :
DOT \.
LPAREN \(
RPAREN \)
LBRACKET \[
RBRACKET \]

/* 注释 */

LINE_COMMENT \/\/[^\n]*
COMMENT_START (\{|\(\*)
COMMENT_END (\}|\*\))

%x COMMENT

%%
{LINE_COMMENT}    { /* 忽略行注释 */
    cout << "Scanner: line comment [" << yytext << "]" << endl;
    // m_driver.increaseLine();
}
{COMMENT_START}    { /* Ignore block comments */
    BEGIN(COMMENT);
    cout << "Scanner: block comment start [" << yytext << "]" << endl;
}
<COMMENT>{
{COMMENT_END}    { /* Ignore block comments */
    BEGIN(INITIAL);
    cout << "Scanner: block comment end [" << yytext << "]" << endl;
}
\n            { /* Ignore block comments */
    cout << "Scanner: block comment new line [" << yytext << "]" << endl;
    // m_driver.increaseLine();
}
{WS}            { /* Ignore whitespace */
    cout << "Scanner: block comment whitespace [" << yytext << "]" << endl;
}
[^}]*            { /* Ignore block comments */
    cout << "Scanner: block comment content [" << yytext << "]" << endl;
}
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
{ASSIGN}    {
    cout << "Scanner: assign [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_ASSIGN(ptr, XYZ::location());
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
{ADDOP}    {
    cout << "Scanner: addop [" << yytext << "]" << endl;
    auto ptr = make_shared<XYZ::ASTNode>();
    return XYZ::Parser::make_ADDOP(ptr, XYZ::location());
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
.   {
    cout << "词法错误[" << yytext << "]" << endl;
}

<<EOF>>     { return yyterminate(); }


%%
