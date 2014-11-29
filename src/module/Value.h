// Filename: Value.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include "../module/Type.h"
#include "../module/Field.h"
#include "../module/Parameter.h"
#include "../module/NumericRange.h"
namespace bamboo
{


// An InvalidValue error is thrown when a provided value is invalid for the given type.
class InvalidValue : public std::runtime_error
{
  public:
    InvalidValue(const std::string& what) : std::runtime_error(what) {}
};

// An InvalidCast error is thrown when a dynamic cast of a Value fails
class InvalidCast : public std::exception
{
  public:
    InvalidCast(const Type *from, Subtype to) : m_from(from), m_to(to)
    {
        m_what = "value for '" + from->to_string() + "' cannot be used as " + format_subtype(to);
    }

    const char *what() const noexcept override { return m_what.c_str(); }
    const Type *from() const { return m_from; }
    Subtype to() const { return m_to; }

  private:
    std::string m_what;
    const Type *m_from;
    Subtype m_to;
};

// A Value is a variant that can represent the value of any Bamboo::Type.
class Value {
  public:
    Value() {}
    Value(const Value&);
    Value& operator=(Value rhs);
    friend void swap(Value&, Value&);
    ~Value();

    explicit Value(const Type *);
    static Value Default(const Type *);

    typedef std::vector<Value> array_t;
    typedef std::map<const Field *, Value, Field::SortById> struct_t;
    typedef std::map<const Parameter *, Value, Parameter::SortByPosition> method_t;

    // parse and unpack variants that don't return an error throw InvalidValue
    static Value parse(const Type *, const std::string& formatted);
    static Value parse(const Type *, const std::string& formatted, bool& error);
    static Value unpack(const Type *, const std::vector<uint8_t>& packed, size_t offset = 0);
    static Value unpack(const Type *, const std::vector<uint8_t>& packed, size_t offset, bool &error);
    static Value unpack32(const Type *, const std::vector<uint8_t>& packed, size_t offset = 0);
    static Value unpack32(const Type *, const std::vector<uint8_t>& packed, size_t offset, bool &error);

    std::string format() const;
    std::vector<uint8_t> pack() const; // native-endiannes
    std::vector<uint8_t> pack32() const; // uses 32-bit max value bytesize

    inline const Type *type() const;

    // as_<T> performs a dynamic cast of the value to the given type; throws InvalidCast
    inline Number as_numeric() const;
    inline const std::string& as_string() const;
    inline const std::vector<uint8_t>& as_blob() const;
    inline const array_t& as_array() const;
    inline const struct_t& as_struct() const;
    inline const method_t& as_method() const;

    // Value member variables can be accessed directly to access the value without type-checking
    union {
        Number m_numeric = Number();

#ifndef _MSC_VER
        // Use C++11 relaxed union for compliant compilers
        std::string m_string;
        std::vector<uint8_t> m_blob;
        array_t m_array;
        struct_t m_struct;
        method_t m_method;
    };
#else
    };

    // Have a bloated Value for MSVC
    std::string m_string;
    std::vector<uint8_t> m_blob;
    array_t m_array;
    struct_t m_struct;
    method_t m_method;
#endif

  private:
    const Type *m_type = Type::None;
};

void swap(Value&, Value&);


} // close namespace bamboo
#include "Value.ipp"
