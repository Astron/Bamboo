// Filename: Value.h
#pragma once
#include <string> // std::string
#include <vector> // std::vector
#include <map>    // std::map
#include "../module/Type.h"
#include "../module/Field.h"
#include "../module/Parameter.h"
namespace bamboo   // open namespace bamboo
{


// An InvalidValue error is thrown when a provided value is invalid for the given type.
class InvalidValue : public std::runtime_error
{
  public:
    InvalidValue(const std::string& what) : std::runtime_error(what) {}
};

// An InvalidCast error is thrown when
class InvalidCast : public std::exception
{
  public:
    InvalidCast(const Type *from, Subtype to) : m_from(from), m_to(to) {
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
// Value's constructor throws null_error if the type is null or invalid_type if the type is invalid.
class Value {
  public:
    explicit Value(const Type *);
    Value(const Value&);
    Value& operator=(Value rhs);
    friend void swap(Value&, Value&);
    ~Value();

    typedef std::vector<Value> array_t;
    typedef std::map<const Field *, Value, Field::sort_by_id> struct_t;
    typedef std::map<const Parameter *, Value, Parameter::sort_by_position> method_t;

    // parse reads a Value from a formatted string; throws InvalidValue
    static Value parse(const Type *, const std::string& formatted);
    // unpack reads a Value from a packed buffer; throws InvalidValue
    static Value unpack(const Type *, const std::vector<uint8_t>& packed, size_t offset = 0);
    // unpack32 reads a Value (with 32-bit size tags) from a packed buffer; throws InvalidValue
    static Value unpack32(const Type *, const std::vector<uint8_t>& packed, size_t offset = 0);

    // format serializes the Value as a formatted string with human-readable values
    std::string format() const;
    // pack serializes the Value to a buffer with native-endian data
    std::vector<uint8_t> pack() const;
    // pack32 serializes the Value (with 32-bit size tags) to a buffer with native-endian data
    std::vector<uint8_t> pack32() const;

    inline const Type *type() const;

    // as_t returns the appropriate cast for a value
    inline char as_char() const;
    inline int8_t as_int8() const;
    inline int16_t as_int16() const;
    inline int32_t as_int32() const;
    inline int64_t as_int64() const;
    inline uint8_t as_uint8() const;
    inline uint16_t as_uint16() const;
    inline uint32_t as_uint32() const;
    inline uint64_t as_uint64() const;
    inline float as_float32() const;
    inline double as_float64() const;
    inline double as_fixed() const;
    inline const std::string& as_string() const;
    inline const std::vector<uint8_t>& as_blob() const;
    inline const array_t& as_array() const;
    inline const struct_t& as_struct() const;
    inline const method_t& as_method() const;

  private:
    Value() {}

    const Type *m_type;
    union {
        char m_char;
        int8_t m_int8;
        int16_t m_int16;
        int32_t m_int32;
        int64_t m_int64;
        uint8_t m_uint8;
        uint16_t m_uint16;
        uint32_t m_uint32;
        uint64_t m_uint64;
        float m_float32;
        double m_float64;
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

};

void swap(Value&, Value&);


} // close namespace bamboo
#include "Value.ipp"
