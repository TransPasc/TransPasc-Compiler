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
    using TerminalNode = XYZ::TerminalNode;
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
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::NUMBER, yytext, m_driver.location());
    return XYZ::Parser::make_NUMBER(ptr, XYZ::location());
}
{CHAR_LITERAL} {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::CHAR_LITERAL, yytext, m_driver.location());
    return XYZ::Parser::make_CHAR_LITERAL(ptr, XYZ::location());
}
{PROGRAM}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_PROGRAM(ptr, XYZ::location());
}
{VAR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_VAR(ptr, XYZ::location());
}
{BEGIN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_BEGIN(ptr, XYZ::location());
}
{END}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_END(ptr, XYZ::location());
}
{IF}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_IF(ptr, XYZ::location());
}
{THEN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_THEN(ptr, XYZ::location());
}
{ELSE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_ELSE(ptr, XYZ::location());
}
{WHILE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_WHILE(ptr, XYZ::location());
}
{DO}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_DO(ptr, XYZ::location());
}
{INTEGER}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_INTEGER(ptr, XYZ::location());
}
{REAL}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_REAL(ptr, XYZ::location());
}
{CONST}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_CONST(ptr, XYZ::location());
}
{ARRAY}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_ARRAY(ptr, XYZ::location());
}
{OF}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_OF(ptr, XYZ::location());
}
{PROCEDURE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_PROCEDURE(ptr, XYZ::location());
}
{FUNCTION}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_FUNCTION(ptr, XYZ::location());
}
{FOR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_FOR(ptr, XYZ::location());
}
{RECORD}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_RECORD(ptr, XYZ::location());
}
{TYPE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_TYPE(ptr, XYZ::location());
}
{LABEL}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_LABEL(ptr, XYZ::location());
}
{CASE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_CASE(ptr, XYZ::location());
}
{GOTO}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_GOTO(ptr, XYZ::location());
}
{TO}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_TO(ptr, XYZ::location());
}
{READ}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_READ(ptr, XYZ::location());
}
{WRITE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_WRITE(ptr, XYZ::location());
}
{NOT}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_NOT(ptr, XYZ::location());
}
{CHAR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_CHAR(ptr, XYZ::location());
}
{BOOLEAN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_BOOLEAN(ptr, XYZ::location());
}
{STRING}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_STRING(ptr, XYZ::location());
}
{OR}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::KEYWORD, yytext, m_driver.location());
    return XYZ::Parser::make_OR(ptr, XYZ::location());
}

{ID}    {
    /* 注: KEYWORD 需要在 ID 之前，否则会被匹配为 ID */
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ID, yytext, m_driver.location());
    return XYZ::Parser::make_ID(ptr, XYZ::location());
}
{RELOP}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::RELOP, yytext, m_driver.location());
    return XYZ::Parser::make_RELOP(ptr, XYZ::location());
}
{PLUS}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ADDOP, yytext, m_driver.location());
    return XYZ::Parser::make_PLUS(ptr, XYZ::location());
}
{MINUS}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ADDOP, yytext, m_driver.location());
    return XYZ::Parser::make_MINUS(ptr, XYZ::location());
}
{MULOP}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::MULOP, yytext, m_driver.location());
    return XYZ::Parser::make_MULOP(ptr, XYZ::location());
}
{ASSIGNOP}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::ASSIGNOP, yytext, m_driver.location());
    return XYZ::Parser::make_ASSIGNOP(ptr, XYZ::location());
}
{COMMA}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::COMMA, yytext, m_driver.location());
    return XYZ::Parser::make_COMMA(ptr, XYZ::location());
}
{SEMICOLON}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::SEMICOLON, yytext, m_driver.location());
    return XYZ::Parser::make_SEMICOLON(ptr, XYZ::location());
}
{COLON}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::COLON, yytext, m_driver.location());
    return XYZ::Parser::make_COLON(ptr, XYZ::location());
}
{DOT}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::DOT, yytext, m_driver.location());
    return XYZ::Parser::make_DOT(ptr, XYZ::location());
}
{QUOTE}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::QUOTE, yytext, m_driver.location());
    return XYZ::Parser::make_QUOTE(ptr, XYZ::location());
}
{LPAREN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::LPAREN, yytext, m_driver.location());
    return XYZ::Parser::make_LPAREN(ptr, XYZ::location());
}
{RPAREN}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::RPAREN, yytext, m_driver.location());
    return XYZ::Parser::make_RPAREN(ptr, XYZ::location());
}
{LBRACKET}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::LBRACKET, yytext, m_driver.location());
    return XYZ::Parser::make_LBRACKET(ptr, XYZ::location());
}
{RBRACKET}    {
    auto ptr = std::make_shared<TerminalNode>(TerminalNode::Type::RBRACKET, yytext, m_driver.location());
    return XYZ::Parser::make_RBRACKET(ptr, XYZ::location());
}


<<EOF>>     { return yyterminate(); }

.           {
    cout << "Scanner: unrecognized character [" << yytext << "]" << endl;
}

%%
