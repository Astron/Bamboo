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

// Subtypes are numeric constants representing the layout of a Type.
enum Subtype {
    /* Numeric Types */
    kTypeChar, // equivalent to uint8, except that it should be printed as a string
    // @TODO(Kevin): Add kTypeByte without breaking legacy_hash
    kTypeInt8, kTypeInt16, kTypeInt32, kTypeInt64,
    kTypeUint8, kTypeUint16, kTypeUint32, kTypeUint64,
    kTypeFloat32, kTypeFloat64,

    // @TODO(Kevin): Merge all numeric types into single subtype kTypeNumeric,
    //       use separate numeric typedef in Numeric.h

    kTypeFixed, // FIXME: Need a better model for loading and storing bounded numeric types

    /* Array Types */
    kTypeString, // a human-readable string
    kTypeBlob,   // any binary data
    kTypeArray,  // any array with fixed byte-length (fixed array-size and element-length)

    /* User-defined types */
    kTypeStruct, // user-defined structure
    kTypeMethod, // user-defined function prototype

    kTypeNone
};

std::string format_subtype(Subtype subtype);

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

    Subtype m_subtype = kTypeNone;
    size_t m_size = 0; // a value of 0 indicates the size is variable
};


} // close namespace bamboo
