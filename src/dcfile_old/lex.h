// Filename: lex.h
#pragma once
#include <iostream> // for std::istream
#include <string>   // for std::string
namespace bamboo   // open namespace bamboo
{


/* Published interface */
int dclex();
int dc_errors();
int dc_warnings();

/* parser-to-lexer interface */
void dclexer_init(std::istream& in, const std::string& filename, int initial_token);
void dcerror(const std::string& msg);
void dcwarning(const std::string& msg);

// we always read files
#define YY_NEVER_INTERACTIVE 1


} // close namespace bamboo
