// Filename: Type.cpp
#include "Type.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{


string format_subtype(Subtype subtype)
{
    switch(subtype) {
    case kTypeChar:
        return "char";
    case kTypeInt8:
        return "int8";
    case kTypeInt16:
        return "int16";
    case kTypeInt32:
        return "int32";
    case kTypeInt64:
        return "int64";
    case kTypeUint8:
        return "uint8";
    case kTypeUint16:
        return "uint16";
    case kTypeUint32:
        return "uint32";
    case kTypeUint64:
        return "uint64";
    case kTypeFloat32:
        return "float32";
    case kTypeFloat64:
        return "float64";
    case kTypeFixed:
        return "fixed";
    case kTypeString:
        return "string";
    case kTypeBlob:
        return "blob";
    case kTypeArray:
        return "array";
    case kTypeStruct:
        return "struct";
    case kTypeMethod:
        return "method";
    case kTypeInvalid:
        return "invalid";
    default:
        return "error";
    }
}

Type *Type::invalid = new Type();

// as_number returns this as a Numeric if it is numeric, or nullptr otherwise.
Numeric *Type::as_numeric()
{
    return nullptr;
}
const Numeric *Type::as_numeric() const
{
    return nullptr;
}

// as_array returns this as an Array if it is an array, or nullptr otherwise.
Array *Type::as_array()
{
    return nullptr;
}
const Array *Type::as_array() const
{
    return nullptr;
}

// as_struct returns this as a Struct if it is a struct, or nullptr otherwise.
Struct *Type::as_struct()
{
    return nullptr;
}
const Struct *Type::as_struct() const
{
    return nullptr;
}

// as_method returns this as a Method if it is a method, or nullptr otherwise.
Method *Type::as_method()
{
    return nullptr;
}
const Method *Type::as_method() const
{
    return nullptr;
}

// to_string returns a short string representation suitable for debug/development output.
std::string Type::to_string() const
{
    if(has_alias()) {
        return alias();
    } else {
        return format_subtype(m_subtype);
    }
}


} // close namespace bamboo
