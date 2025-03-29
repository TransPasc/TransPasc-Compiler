%{
    #include <iostream>
    #include <cstdlib>
    #include "scanner.h"
    #include "driver.h"
    #include "parser.hpp"
    #include "location.hh"

    using namespace std;

    #define yyterminate() XYZ::Parser::make_END(XYZ::location());

    #define YY_USER_ACTION m_driver.increaseLocation(yyleng);
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="XYZ_"

NUM [0-9]+
WS [ \t\n\r]+

%%

{WS}    { /* Ignore whitespace */ }
{NUM}    {
    cout << "Scanner: number [" << yytext << "]" << endl;
    uint64_t number = strtoull(yytext, 0, 10);
    return XYZ::Parser::make_NUMBER(number, XYZ::location());
}
.           {
    cout << "Scanner: unknown character [" << yytext << "]" << endl;
}

<<EOF>>     { return yyterminate(); }


%%
