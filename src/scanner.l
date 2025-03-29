%{
    #include <iostream>
    #include <cstdlib>
    #include "scanner.h"
    #include "driver.h"
    #include "parser.hpp"
    #include "location.hh"

    using namespace std;

    #define yyterminate() XYZ::Parser::make_END_FILE(XYZ::location());

    #define YY_USER_ACTION m_driver.increaseLocation(yyleng);
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
ADDOP (\+|-| or)
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

LINE_COMMENT (//[^\n]*\n)
COMMENT_START (\{|\(\*)
COMMENT_END (\}|\*\))

%x COMMENT

%%
{WS}    { /* Ignore whitespace */ }
{NUM}    {
    cout << "Scanner: number [" << yytext << "]" << endl;
    uint64_t number = strtoull(yytext, 0, 10);
    return XYZ::Parser::make_NUMBER(number, XYZ::location());
}
.   {
    cout << "词法错误[" << yytext << "]" << endl;
}

<<EOF>>     { return yyterminate(); }


%%
