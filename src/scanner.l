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

/* 词法规则定义 */
WS [ \t\r\v\f]

LETTER [a-zA-Z]
DIGIT [0-9]
ID {LETTER}({LETTER}|{DIGIT})*

DIGITS {DIGIT}+

OPTIONAL_FRACTION (\.{DIGITS})?
NUM {DIGITS}{OPTIONAL_FRACTION}

/* KEY WORD */
/* program var procedure begin if else ... */

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
    // m_driver.increaseLine();
    cout << "换行" << endl;
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
{ID}    {
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
