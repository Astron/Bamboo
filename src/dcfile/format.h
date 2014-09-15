// Filename: format.h
#pragma once
#include "../module/Value.h"
namespace bamboo   // open namespace bamboo
{


// Forward declarations
class Value;

// format_value steps through a Value and outputs it as a .dc file parameter value.
std::string format_value(const Value& value);
std::string format_value(const Value *value);
void format_value(const Value& value, std::ostream& out);
void format_value(const Value *value, std::ostream& out);

// format_hex outputs a blob as a hexidecimal constant enclosed in angle-brackets (<>)
std::string format_hex(const std::vector<uint8_t>& blob);
void format_hex(const std::vector<uint8_t>& blob, std::ostream& out);

// format_quoted outputs a string enclosed in quotes after escaping the string.
//     Any instances of backslash (\) or the quoute character in the string are escaped.
//     Non-printable characters are replaced with an escaped hexidecimal constant.std::string format_quoted(const std::string &str);
std::string format_quoted(char quote_mark, const std::string& str);
void format_quoted(char quote_mark, const std::string& str, std::ostream& out);


} // close namespace bamboo
