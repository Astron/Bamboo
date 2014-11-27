// Filename: NumericRange.h
#pragma once
#include <cstdint>
#include <limits>
#include <ostream>
namespace bamboo   // open namespace bamboo
{


enum NumberType {
    Number_NotANumber = 0,
    Number_Signed,
    Number_Unsigned,
    Number_Floating
};

// A Number represents any C++ numeric type.
struct Number
{
    NumberType type;
    union {
        int64_t sinteger;
        uint64_t uinteger;
        double floating;
    };

    Number()           : type(Number_NotANumber), sinteger(0)  {}
    Number(int32_t n)  : type(Number_Signed),     sinteger(n)  {}
    Number(int64_t n)  : type(Number_Signed),     sinteger(n)  {}
    Number(uint32_t n) : type(Number_Unsigned),   uinteger(n) {}
    Number(uint64_t n) : type(Number_Unsigned),   uinteger(n) {}
    Number(double n)   : type(Number_Floating),   floating(n) {}
    Number operator=(int64_t n)
    {
        return Number(n);
    }
    Number operator=(uint64_t n)
    {
        return Number(n);
    }
    Number operator=(double n)
    {
        return Number(n);
    }

#define CONVERSION(numtype) \
    operator numtype() const \
    { \
        switch(type) { \
        case Number_Signed: \
            return (numtype)sinteger; \
        case Number_Unsigned: \
            return (numtype)uinteger; \
        case Number_Floating: \
            return (numtype)floating; \
        case Number_NotANumber: \
        default: \
            return 0; \
        } \
    }

    explicit CONVERSION(char);
    explicit CONVERSION(int8_t);
    explicit CONVERSION(int16_t);
    explicit CONVERSION(int32_t);
    explicit CONVERSION(int64_t);
    explicit CONVERSION(uint8_t);
    explicit CONVERSION(uint16_t);
    explicit CONVERSION(uint32_t);
    explicit CONVERSION(uint64_t);
    explicit CONVERSION(float);
    explicit CONVERSION(double);

#undef CONVERSION

};

// @TODO(Kevin): Implement other comparison operators
// @FIXME(Kevin): Allow operator== to perform conversions and compare numerically
inline bool operator==(const Number& a, const Number& b)
{
    return a.type == b.type && a.uinteger == b.uinteger;
}

// A NumericRange represents a range of integer or floating-point values.
//     This is used to limit numeric types; or array, string, or blob sizes.
struct NumericRange {
    NumberType type;
    Number min;
    Number max;

    inline NumericRange() : type(Number_NotANumber)
    {
        min.type = max.type = Number_NotANumber;
        // lowest is the most negative float (min is smallest positive)
        min.floating = std::numeric_limits<double>::lowest();
        max.floating = std::numeric_limits<double>::max();
    }
    inline NumericRange(Number size)                : type(size.type),       min(size), max(size) {}
    inline NumericRange(int32_t min, int32_t max)   : type(Number_Signed),   min(min), max(max) {}
    inline NumericRange(int64_t min, int64_t max)   : type(Number_Signed),   min(min), max(max) {}
    inline NumericRange(uint32_t min, uint32_t max) : type(Number_Unsigned), min(min), max(max) {}
    inline NumericRange(uint64_t min, uint64_t max) : type(Number_Unsigned), min(min), max(max) {}
    inline NumericRange(double min, double max)     : type(Number_Floating), min(min), max(max) {}

    inline bool contains(Number num) const
    {
        switch(min.type) {
        case Number_NotANumber:
            return true;
        case Number_Signed:
            {
                int64_t val = int64_t(num);
                return (min.sinteger <= val && val <= max.sinteger);
            }
        case Number_Unsigned:
            {
                uint64_t val = uint64_t(num);
                return (min.uinteger <= val && val <= max.uinteger);
            }
        case Number_Floating:
            {
                double val = double(num);
                return (min.floating <= val && val <= max.floating);
            }
        }
    }

    inline bool is_nan() const
    {
        return (type == Number_NotANumber);
    }
};


} // close namespace bamboo

// Compatibility with namespace std primitives
namespace std
{


inline std::ostream& operator<<(std::ostream& lhs, const bamboo::Number& rhs)
{
    switch(rhs.type) {
    case bamboo::Number_NotANumber:
        lhs << "nan";
        break;
    case bamboo::Number_Signed:
        lhs << (int64_t)rhs;
        break;
    case bamboo::Number_Unsigned:
        lhs << (uint64_t)rhs;
        break;
    case bamboo::Number_Floating:
        lhs << (double)rhs;
        break;
    }

    return lhs;
}


} // close namespace std
