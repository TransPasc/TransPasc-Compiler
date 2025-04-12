#pragma once

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer XYZ_FlexLexer // the trick with prefix; no namespace here :(
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL XYZ::Parser::symbol_type XYZ::Scanner::get_next_token()

#include "parser.hpp" // this is needed for symbol_type

namespace XYZ {

class Driver;

class Scanner : public yyFlexLexer {
public:
  Scanner(Driver &driver) : m_driver(driver) {}
  virtual ~Scanner() {}
  virtual XYZ::Parser::symbol_type get_next_token();

private:
  Driver &m_driver;
};

} // namespace XYZ
