// Filename: DistributedType.h
#pragma once
#include <string> // std::string
#include "../bits/sizetag.h"
namespace bamboo { // open namespace bamboo


// Forward declaration
class ArrayType;
class Method;
class NumericType;
class Struct;
class HashGenerator;


// The Type enum are numeric constants representing the layout of the DistributedType
enum Subtype {
    /* Numeric Types */
    kTypeInt8, kTypeInt16, kTypeInt32, kTypeInt64,
    kTypeUint8, kTypeUint16, kTypeUint32, kTypeUint64,
    kTypeChar, // equivalent to uint8, except that it should be printed as a string
    kTypeFloat32, kTypeFloat64,

    /* Array Types */
    kTypeString,      // a human-printable string with fixed length
    kTypeVarstring,   // a human-printable string with variable length
    kTypeBlob,        // any binary data stored as a string, fixed length
    kTypeVarblob,     // any binary data stored as a varstring, variable length
    kTypeArray,       // any array with fixed byte-length (fixed array-size and element-length)
    kTypeVararray,    // any array with variable array-size or variable length elements

    /* Complex Types */
    kTypeStruct,
    kTypeMethod,

    // New additions should be added at the end to prevent the file hash from changing.

    kTypeInvalid
};

// A DistributedType is a shared type with a defined layout of data.
class DistributedType {
  protected:
    inline DistributedType();

  public:
    static DistributedType *invalid;
    virtual ~DistributedType();

    // get_type returns the type's fundamental type as an integer constant.
    inline Subtype get_subtype() const;

    // has_fixed_size returns true if the DistributedType has a fixed size in bytes.
    inline bool has_fixed_size() const;
    // get_size returns the size of the DistributedType in bytes or 0 if it is variable.
    inline sizetag_t get_size() const;

    // has_alias returns true if this type was defined the an aliased name.
    inline bool has_alias() const;
    // get_alias returns the name used to define the type, or the empty string.
    inline const std::string& get_alias() const;
    // set_alias gives this type the alternate name <alias>.
    inline void set_alias(const std::string& alias);

    // as_numeric returns this as a NumericType if it is numeric, or nullptr otherwise.
    virtual NumericType *as_numeric();
    virtual const NumericType *as_numeric() const;

    // as_array returns this as an ArrayType if it is an array, or nullptr otherwise.
    virtual ArrayType *as_array();
    virtual const ArrayType *as_array() const;

    // as_struct returns this as a Struct if it is a struct, or nullptr otherwise.
    virtual Struct *as_struct();
    virtual const Struct *as_struct() const;

    // as_method returns this as a Method if it is a method, or nullptr otherwise.
    virtual Method *as_method();
    virtual const Method *as_method() const;

    // to_string returns a short string representation suitable for debug/development output.
    virtual std::string to_string() const;

  protected:
    Subtype m_subtype;
    sizetag_t m_size;
    std::string m_alias;
};


} // close namespace bamboo
#include "DistributedType.ipp"
