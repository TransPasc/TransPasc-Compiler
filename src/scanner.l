%{
    #include <iostream>
    #include <cstdlib>
    #include "scanner.h"
    #include "driver.h"
    #include "parser.hpp"
    #include "location.hh"
    #include "ast/ast.h"

    using namespace std;

    #define yyterminate() XYZ::Parser::make_END_FILE(m_driver.getLocation());

    #define YY_USER_ACTION m_driver.increaseLocation(yyleng);

    using ASTNodePtr = std::shared_ptr<XYZ::ASTNode>;
    using TerminalNode = XYZ::TerminalNode;
%}

%option debug

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

%{
  // C++ 兼容的词法分析器的规则，step函数把位置的起始值设置为与结束值相等，这样位置就指向了上一个极少的结束位置。
  m_driver.step();
%}

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
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::NUMBER, TerminalNode::makeNum(yytext), m_driver.getLine());
    return XYZ::Parser::make_NUMBER(ptr, m_driver.getLocation());
}
{CHAR_LITERAL} {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::CHAR_LITERAL, yytext, m_driver.getLine());
    return XYZ::Parser::make_CHAR_LITERAL(ptr, m_driver.getLocation());
}
{PROGRAM}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_PROGRAM(ptr, m_driver.getLocation());
}
{VAR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_VAR(ptr, m_driver.getLocation());
}
{BEGIN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_BEGIN(ptr, m_driver.getLocation());
}
{END}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_END(ptr, m_driver.getLocation());
}
{IF}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_IF(ptr, m_driver.getLocation());
}
{THEN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_THEN(ptr, m_driver.getLocation());
}
{ELSE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_ELSE(ptr, m_driver.getLocation());
}
{WHILE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_WHILE(ptr, m_driver.getLocation());
}
{DO}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_DO(ptr, m_driver.getLocation());
}
{INTEGER}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_INTEGER(ptr, m_driver.getLocation());
}
{REAL}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_REAL(ptr, m_driver.getLocation());
}
{CONST}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_CONST(ptr, m_driver.getLocation());
}
{ARRAY}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_ARRAY(ptr, m_driver.getLocation());
}
{OF}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_OF(ptr, m_driver.getLocation());
}
{PROCEDURE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_PROCEDURE(ptr, m_driver.getLocation());
}
{FUNCTION}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_FUNCTION(ptr, m_driver.getLocation());
}
{FOR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_FOR(ptr, m_driver.getLocation());
}
{RECORD}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_RECORD(ptr, m_driver.getLocation());
}
{TYPE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_TYPE(ptr, m_driver.getLocation());
}
{LABEL}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_LABEL(ptr, m_driver.getLocation());
}
{CASE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_CASE(ptr, m_driver.getLocation());
}
{GOTO}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_GOTO(ptr, m_driver.getLocation());
}
{TO}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_TO(ptr, m_driver.getLocation());
}
{READ}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_READ(ptr, m_driver.getLocation());
}
{WRITE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_WRITE(ptr, m_driver.getLocation());
}
{NOT}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_NOT(ptr, m_driver.getLocation());
}
{CHAR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_CHAR(ptr, m_driver.getLocation());
}
{BOOLEAN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_BOOLEAN(ptr, m_driver.getLocation());
}
{STRING}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_STRING(ptr, m_driver.getLocation());
}
{OR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.getLine());
    return XYZ::Parser::make_OR(ptr, m_driver.getLocation());
}

{ID}    {
    /* 注: KEYWORD 需要在 ID 之前，否则会被匹配为 ID */
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ID, yytext, m_driver.getLine());
    return XYZ::Parser::make_ID(ptr, m_driver.getLocation());
}
{RELOP}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::RELOP, yytext, m_driver.getLine());
    return XYZ::Parser::make_RELOP(ptr, m_driver.getLocation());
}
{PLUS}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ADDOP, yytext, m_driver.getLine());
    return XYZ::Parser::make_PLUS(ptr, m_driver.getLocation());
}
{MINUS}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ADDOP, yytext, m_driver.getLine());
    return XYZ::Parser::make_MINUS(ptr, m_driver.getLocation());
}
{MULOP}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::MULOP, yytext, m_driver.getLine());
    return XYZ::Parser::make_MULOP(ptr, m_driver.getLocation());
}
{ASSIGNOP}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ASSIGNOP, yytext, m_driver.getLine());
    return XYZ::Parser::make_ASSIGNOP(ptr, m_driver.getLocation());
}
{COMMA}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::COMMA, yytext, m_driver.getLine());
    return XYZ::Parser::make_COMMA(ptr, m_driver.getLocation());
}
{SEMICOLON}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::SEMICOLON, yytext, m_driver.getLine());
    return XYZ::Parser::make_SEMICOLON(ptr, m_driver.getLocation());
}
{COLON}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::COLON, yytext, m_driver.getLine());
    return XYZ::Parser::make_COLON(ptr, m_driver.getLocation());
}
{DOT}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::DOT, yytext, m_driver.getLine());
    return XYZ::Parser::make_DOT(ptr, m_driver.getLocation());
}
{QUOTE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::QUOTE, yytext, m_driver.getLine());
    return XYZ::Parser::make_QUOTE(ptr, m_driver.getLocation());
}
{LPAREN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::LPAREN, yytext, m_driver.getLine());
    return XYZ::Parser::make_LPAREN(ptr, m_driver.getLocation());
}
{RPAREN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::RPAREN, yytext, m_driver.getLine());
    return XYZ::Parser::make_RPAREN(ptr, m_driver.getLocation());
}
{LBRACKET}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::LBRACKET, yytext, m_driver.getLine());
    return XYZ::Parser::make_LBRACKET(ptr, m_driver.getLocation());
}
{RBRACKET}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::RBRACKET, yytext, m_driver.getLine());
    return XYZ::Parser::make_RBRACKET(ptr, m_driver.getLocation());
}


<<EOF>>     { return yyterminate(); }

.           {
    m_driver.handleError("unrecognized character");
}

%%
