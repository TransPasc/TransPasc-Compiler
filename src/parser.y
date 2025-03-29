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

    using namespace std;

    namespace XYZ {
        class Scanner;
        class Driver;
    }
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

%token END 0 "end of file"
%token <std::string> STRING  "string";
%token <uint64_t> NUMBER "number";
%token LEFTPAR "leftpar";
%token RIGHTPAR "rightpar";
%token SEMICOLON "semicolon";
%token COMMA "comma";
%%

program :
{
    cout << "*** RUN ***" << endl;
    cout << endl << "prompt> ";
    driver.clear();
} | program NUMBER {
    cout << "Number: " << $2 << endl;
} | {
    cout << "Empty program" << endl;
};

%%

// Bison expects us to provide implementation - otherwise linker complains
void XYZ::Parser::error(const location &loc , const std::string &message) {
    cout << "Error: " << message << endl << "Error location: " << driver.location() << endl;
}
