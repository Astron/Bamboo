// Filename: Type.cpp
#include "Type.h"
#include "Array.h"
#include "Numeric.h"
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
    case kTypeNone:
        return "none";
    default:
        return "<invalid>";
    }
}

Type *Type::None = new Type();
Type *Type::Char = new Numeric(kTypeChar);
Type *Type::Int8 = new Numeric(kTypeInt8);
Type *Type::Int16 = new Numeric(kTypeInt16);
Type *Type::Int32 = new Numeric(kTypeInt32);
Type *Type::Int64 = new Numeric(kTypeInt64);
Type *Type::Uint8 = new Numeric(kTypeUint8);
Type *Type::Uint16 = new Numeric(kTypeUint16);
Type *Type::Uint32 = new Numeric(kTypeUint32);
Type *Type::Uint64 = new Numeric(kTypeUint64);
Type *Type::Float32 = new Numeric(kTypeFloat32);
Type *Type::Float64 = new Numeric(kTypeFloat64);
Type *Type::String = new Array(Type::Char, "string");
Type *Type::Blob = new Array(Type::Uint8, "blob");

Numeric *Type::as_numeric()
{
    return nullptr;
}

const Numeric *Type::as_numeric() const
{
    return nullptr;
}

Array *Type::as_array()
{
    return nullptr;
}

const Array *Type::as_array() const
{
    return nullptr;
}

Struct *Type::as_struct()
{
    return nullptr;
}

const Struct *Type::as_struct() const
{
    return nullptr;
}

Method *Type::as_method()
{
    return nullptr;
}

const Method *Type::as_method() const
{
    return nullptr;
}

std::string Type::to_string() const
{
    if(has_alias()) {
        return alias();
    } else {
        return format_subtype(m_subtype);
    }
}


} // close namespace bamboo
