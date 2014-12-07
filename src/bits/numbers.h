// Filename: numbers.h
#pragma once
#include <cstdint>
#include <ostream>
namespace bamboo
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


} // close namespace bamboo

namespace std
{


// Support output to std::stream
inline ostream& operator<<(ostream& lhs, const bamboo::Number& rhs)
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
