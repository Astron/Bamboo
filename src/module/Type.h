// Filename: Type.h
#pragma once
#include <string>
#include "../bits/sizetag.h"
namespace bamboo
{


class Array;
class Method;
class Numeric;
class Struct;

// Subtypes are numeric constants representing the layout of a Type.
enum Subtype {
    /* Numeric Types */
    kTypeChar, // equivalent to uint8, except that it should be printed as a string
    kTypeInt8, kTypeInt16, kTypeInt32, kTypeInt64,
    kTypeUint8, kTypeUint16, kTypeUint32, kTypeUint64,
    kTypeFloat32, kTypeFloat64,

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
    explicit Type(const std::string& alias) : m_alias(alias) {}
    virtual ~Type() {}

    // Unbounded primitive data types
    static Type *None;
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

    inline bool has_alias() const { return !m_alias.empty(); }
    inline const std::string& alias() const { return m_alias; }
    inline void set_alias(const std::string& alias) { m_alias = alias; }

    virtual Numeric *as_numeric();
    virtual const Numeric *as_numeric() const;
    virtual Array *as_array();
    virtual const Array *as_array() const;
    virtual Struct *as_struct();
    virtual const Struct *as_struct() const;
    virtual Method *as_method();
    virtual const Method *as_method() const;

    virtual std::string to_string() const; // representation for debug/development output

  protected:
    Subtype m_subtype = kTypeNone;
    size_t m_size = 0; // a value of 0 indicates the size is variable
    std::string m_alias;
};


} // close namespace bamboo
