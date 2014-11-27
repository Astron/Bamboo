// Filename: Type.h
#pragma once
#include <string>
#include "../bits/sizetag.h"
namespace bamboo
{


class TypeAlias;
class Array;
class Method;
class Numeric;
class Struct;

enum Subtype {
    Subtype_None = 0,
    Subtype_Numeric, // any number
    Subtype_String,  // a human-readable string
    Subtype_Blob,    // any binary data
    Subtype_Array,   // any array with fixed byte-length (fixed array-size and element-length)
    Subtype_Struct,  // user-defined structure
    Subtype_Method,  // user-defined function prototype
};

const char *format_subtype(Subtype subtype);

// A Type defines the storage and wire format of a variable in the Bamboo type system
class Type
{
  public:
    Type() {}
    Type(const Type&) = delete;
    Type& operator=(const Type&) = delete;
    virtual ~Type() {}

    // Unbounded primitive data types
    static Type *None;
    static Type *Byte;
    static Type *Char;
    static Type *Int8;
    static Type *Int16;
    static Type *Int32;
    static Type *Int64;
    static Type *Uint8;
    static Type *Uint16;
    static Type *Uint32;
    static Type *Uint64;
    static Type *Float32;
    static Type *Float64;
    static Type *String;
    static Type *Blob;

    inline Subtype subtype() const { return m_subtype; }

    inline bool has_fixed_size() const { return m_size != 0; }
    inline size_t fixed_size() const { return m_size; }

    virtual TypeAlias *as_aliased();
    virtual const TypeAlias *as_aliased() const;
    virtual Numeric *as_numeric();
    virtual const Numeric *as_numeric() const;
    virtual Array *as_array();
    virtual const Array *as_array() const;
    virtual Struct *as_struct();
    virtual const Struct *as_struct() const;
    virtual Method *as_method();
    virtual const Method *as_method() const;

    virtual std::string to_string() const;

  protected:
    explicit Type(Subtype subtype) : m_subtype(subtype) {}

    Subtype m_subtype = Subtype_None;
    size_t m_size = 0; // a value of 0 indicates the size is variable
};


} // close namespace bamboo
