// Filename: format.cpp
#include "format.h"
#include <cctype>  // std::isprint()
#include <sstream> // std::ostringstream
#include "../module/Type.h"
#include "../module/Array.h"
#include "../module/Struct.h"
#include "../module/Field.h"
#include "../module/Method.h"
#include "../module/Parameter.h"
using namespace std;

#if defined(_MSC_VER) && _MSC_VER <= 1800
#define snprintf sprintf_s
#endif

namespace bamboo   // open namespace bamboo
{


// format_value steps through a Value and outputs it as a .dc file parameter value.
//     This is used to produce default values when outputting a distributed class to a file.
string format_value(const Value& value)
{
    return format_value(&value);
}
string format_value(const Value *value)
{
    ostringstream ss;
    format_value(value, ss);
    return ss.str();
}
void format_value(const Value& value, ostream& out)
{
    return format_value(&value, out);
}
void format_value(const Value *value, ostream& out)
{
    const Type *type = value->type;
    Subtype subtype = type->subtype();
    switch(subtype) {
    case kTypeNone:
        out << "<invalid>";
        break;
    case kTypeInt8:
        out << int(value->int_);
        break;
    case kTypeInt16:
        out << int(value->int_);
        break;
    case kTypeInt32:
        out << int(value->int_);
        break;
    case kTypeInt64:
        out << int64_t(value->int_);
        break;
    case kTypeUint8:
        out << uint32_t(value->uint_);
        break;
    case kTypeUint16:
        out << uint32_t(value->uint_);
        break;
    case kTypeUint32:
        out << uint32_t(value->uint_);
        break;
    case kTypeUint64:
        out << uint64_t(value->uint_);
        break;
    case kTypeFloat32:
        out << value->float_;
        break;
    case kTypeFloat64:
        out << value->double_;
        break;
    case kTypeChar:
        format_quoted('\'', string(1, value->char_), out);
        break;
    case kTypeString:
    case kTypeVarstring: {
        // If we have a string alias format as a quoted string
        if(type->has_alias() && type->alias() == "string") {
            // Enquoute and escape string then output
            format_quoted('"', value->string_, out);
        } else {
            // Otherwise format as an array of chars
            out << '[';
            if(value->string_.size() > 0) {
                format_quoted('\'', string(1, value->string_[0]), out);
            }
            for(size_t i = 1; i < value->string_.size(); ++i) {
                out << ", ";
                format_quoted('\'', string(1, value->string_[i]), out);
            }
            out << ']';
        }
        break;
    }
    case kTypeBlob:
    case kTypeVarblob: {
        // If we have a blob alias format as a hex constant
        if(type->has_alias() && type->alias() == "blob") {
            // Format blob as a hex constant then output
            format_hex(value->blob_, out);
        } else {
            // Otherwise format as an array of uint8
            out << '[';
            if(value->blob_.size() > 0) {
                out << uint32_t(value->blob_[0]);
            }
            for(size_t i = 1; i < value->string_.size(); ++i) {
                out << ", " << uint32_t(value->blob_[i]);
            }
            out << ']';
        }

        break;
    }
    case kTypeArray:
    case kTypeVararray: {
        out << '[';
        if(value->elements_.size() > 0) {
            format_value(value->elements_[0], out);
        }
        for(size_t i = 1; i < value->elements_.size(); ++i) {
            out << ", ";
            format_value(value->elements_[i], out);
        }
        out << ']';
        break;
    }
    case kTypeStruct: {
        out << '{';
        const Struct *struct_ = type->as_struct();
        size_t num_fields = struct_->num_fields();
        if(num_fields > 0) {
            const Field *field = struct_->nth_field(0);
            if(field->as_molecular() == nullptr) {
                format_value(value->fields_.at(field), out);
            }
        }
        for(unsigned int i = 1; i < num_fields; ++i) {
            out << ", ";
            const Field *field = struct_->nth_field(i);
            if(field->as_molecular() == nullptr) {
                format_value(value->fields_.at(field), out);
            }
        }
        out << '}';
        break;
    }
    case kTypeMethod: {
        out << '(';
        const Method *method_ = type->as_method();
        size_t num_params = method_->num_params();
        if(num_params > 0) {
            const Parameter *param = method_->nth_param(0);
            format_value(value->arguments_.at(param), out);
        }
        for(unsigned int i = 1; i < num_params; ++i) {
            const Parameter *param = method_->nth_param(i);
            format_value(value->arguments_.at(param), out);
        }
        out << ')';
        break;
    }
    default:
        out << "<error>";
    }
}

// format_hex outputs bytes to <out> as a hexidecimal constant enclosed in angle-brackets (<>).
void format_hex(const vector<uint8_t>& bytes, ostream& out)
{
    out << '<';
    for(auto it = bytes.begin(); it != bytes.end(); ++it) {
        char infer[10];
        snprintf(infer, 10, "%02x", (uint8_t)(*it));
        out << infer;
    }
    out << '>';
}
string format_hex(const vector<uint8_t>& bytes)
{
    ostringstream ss;
    format_hex(bytes, ss);
    return ss.str();
}

// format_quoted outputs <str> to <out> quoted with the character <quote_mark>.
//     Any instances of backslash (\) or the quoute character in the string are escaped.
//     Non-printable characters are replaced with an escaped hexidecimal constant.
void format_quoted(char quote_mark, const string& str, ostream& out)
{
    out << quote_mark;
    for(auto it = str.begin(); it != str.end(); ++it) {
        char c = *it;
        if(c == quote_mark || c == '\\') {
            // escape the character
            out << '\\' << c;

        } else if(!isprint(c)) { // character is not a printable ascii character
            // print the character as an escaped hexidecimal character constant
            char infer[10];
            snprintf(infer, 10, "%02x", (unsigned char)c);
            out << "\\x" << infer;
        } else {
            out << c;
        }
    }
    out << quote_mark;
}
string format_quoted(char quote_mark, const string& str)
{
    ostringstream ss;
    format_quoted(quote_mark, str, ss);
    return ss.str();
}


} // close namespace bamboo
