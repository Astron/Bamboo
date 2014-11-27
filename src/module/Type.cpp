// Filename: Type.cpp
#include "Type.h"
#include "TypeAlias.h"
#include "Array.h"
#include "Numeric.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{


const char *format_subtype(Subtype subtype)
{
    switch(subtype) {
    case Subtype_Numeric:
        return "numeric";
    case Subtype_String:
        return "string";
    case Subtype_Blob:
        return "blob";
    case Subtype_Array:
        return "array";
    case Subtype_Struct:
        return "struct";
    case Subtype_Method:
        return "method";
    case Subtype_None:
        return "none";
    }

    return "unknown";
}


Type *Type::None = new Type();
Type *Type::Int8 = new Numeric(Numeric_Int8);
Type *Type::Int16 = new Numeric(Numeric_Int16);
Type *Type::Int32 = new Numeric(Numeric_Int32);
Type *Type::Int64 = new Numeric(Numeric_Int64);
Type *Type::Uint8 = new Numeric(Numeric_Uint8);
Type *Type::Uint16 = new Numeric(Numeric_Uint16);
Type *Type::Uint32 = new Numeric(Numeric_Uint32);
Type *Type::Uint64 = new Numeric(Numeric_Uint64);
Type *Type::Float32 = new Numeric(Numeric_Float32);
Type *Type::Float64 = new Numeric(Numeric_Float64);

Type *Type::Char = new TypeAlias("char", Type::Uint8, false);
Type *Type::Byte = new TypeAlias("byte", Type::Uint8, false);
Type *Type::String = new TypeAlias("string", new Array(Type::Char, false), true);
Type *Type::Blob = new TypeAlias("blob", new Array(Type::Byte, false), true);

TypeAlias *Type::as_aliased()
{
    return nullptr;
}

const TypeAlias *Type::as_aliased() const
{
    return nullptr;
}

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
    return format_subtype(m_subtype);
}


} // close namespace bamboo
