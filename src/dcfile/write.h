// Filename: write.h
#pragma once
#include <iostream>
namespace bamboo   // open namespace bamboo
{

// indent outputs the indicated number of spaces to the given output stream, returning the
//     stream itself.  Useful for indenting a series of lines of text by a given amount.
std::ostream& indent(std::ostream& out, unsigned int indent_level);

// format_type outputs the numeric type constant as a string.
std::string format_type(unsigned int type);

} // close namespace bamboo
