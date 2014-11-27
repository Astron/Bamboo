// Filename: Numeric.cpp
#include "Numeric.h"
#include <cmath>   // std::floor
#include <limits>  // std::numeric_limits
#include <sstream> // std::stringstream
using namespace std;
namespace bamboo
{


const char *format_numeric_type(NumericType numeric_type)
{
    switch(numeric_type) {
    case Numeric_Invalid:
        return "invalid";
    case Numeric_Int8:
        return "int8";
    case Numeric_Int16:
        return "int16";
    case Numeric_Int32:
        return "int32";
    case Numeric_Int64:
        return "int64";
    case Numeric_Uint8:
        return "uint8";
    case Numeric_Uint16:
        return "uint16";
    case Numeric_Uint32:
        return "uint32";
    case Numeric_Uint64:
        return "uint64";
    case Numeric_Float32:
        return "float32";
    case Numeric_Float64:
        return "float64";
    }

    return "unknown";
}

Numeric::Numeric(NumericType packtype) : Type(Subtype_Numeric), m_packtype(packtype)
{
    switch(packtype) {
    case Numeric_Uint8:
    case Numeric_Int8:
        m_size = sizeof(int8_t);
        break;
    case Numeric_Uint16:
    case Numeric_Int16:
        m_size = sizeof(int16_t);
        break;
    case Numeric_Uint32:
    case Numeric_Int32:
        m_size = sizeof(int32_t);
        break;
    case Numeric_Uint64:
    case Numeric_Int64:
        m_size = sizeof(int64_t);
        break;
    case Numeric_Float32:
        m_size = sizeof(float);
        break;
    case Numeric_Float64:
        m_size = sizeof(double);
        break;
    case Numeric_Invalid:
        m_size = 0;
        break;
    }
}

Numeric *Numeric::as_numeric()
{
    return this;
}

const Numeric *Numeric::as_numeric() const
{
    return this;
}

bool Numeric::set_divisor(unsigned int divisor)
{
    if(divisor == 0) { return false; }

    m_divisor = divisor;
    if(has_range()) { set_range(m_orig_range); }
    if(has_modulus()) { set_modulus(m_orig_modulus); }

    return true;
}

bool Numeric::set_modulus(double modulus)
{
    if(modulus <= 0.0) { return false; }
    double float_modulus = modulus * m_divisor;
    uint64_t uint_modulus = uint64_t(floor(float_modulus + 0.5));

    // Check the range.  A valid range for the modulus is 1 to (maximum_value + 1) after scaling.
    switch(m_packtype) {
    case Numeric_Uint8:
        if(uint_modulus < 1 || uint16_t(numeric_limits<uint8_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Uint16:
        if(uint_modulus < 1 || uint32_t(numeric_limits<uint16_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Uint32:
        if(uint_modulus < 1 || uint64_t(numeric_limits<uint32_t>::max()) + 1ull < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Uint64:
        if(uint_modulus < 1) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Int8:
        if(uint_modulus < 1 || uint16_t(numeric_limits<int8_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Int16:
        if(uint_modulus < 1 || uint32_t(numeric_limits<int16_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Int32:
        if(uint_modulus < 1 || uint64_t(numeric_limits<int32_t>::max()) + 1ull < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Int64:
        if(uint_modulus < 1) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case Numeric_Float32:
    case Numeric_Float64:
        m_modulus = float_modulus;
        break;
    case Numeric_Invalid:
        return false;
    }

    m_orig_modulus = modulus;
    return true;
}

bool Numeric::set_range(const NumericRange& range)
{
    // @TODO(Kevin): Accept integer ranges
    if(range.type != Number_Floating) { return false; }

    m_orig_range = range;
    switch(m_packtype) {
    case Numeric_Int8:
    case Numeric_Int16:
    case Numeric_Int32:
    case Numeric_Int64:
        {
            // @TODO(Kevin): Validate range, i.e. => min and max within (INT[N]_MIN - INT[N]MAX)
            int64_t min = (int64_t)floor(range.min.floating * m_divisor + 0.5);
            int64_t max = (int64_t)floor(range.max.floating * m_divisor + 0.5);
            m_range = NumericRange(min, max);
        }
        break;
    case Numeric_Uint8:
    case Numeric_Uint16:
    case Numeric_Uint32:
    case Numeric_Uint64:
        {
            // @TODO(Kevin): Validate range, i.e. => min and max within (UINT[N]_MIN - UINT[N]MAX)
            uint64_t min = (uint64_t)floor(range.min.floating * m_divisor + 0.5);
            uint64_t max = (uint64_t)floor(range.max.floating * m_divisor + 0.5);
            m_range = NumericRange(min, max);
        }
        break;
    case Numeric_Float32:
    case Numeric_Float64:
        {
            double min = range.min.floating * m_divisor;
            double max = range.max.floating * m_divisor;
            m_range = NumericRange(min, max);
        }
        break;
    case Numeric_Invalid:
        return false;
    }

    return true;
}

string Numeric::to_string() const
{
    stringstream repr;
    repr << format_numeric_type(m_packtype);
    if(has_range()) {
        if(m_range.min == m_range.max) { repr << "(" << m_range.min << ")"; }
        else { repr << "(" << m_range.min << ", " << m_range.max << ")"; }
    }
    if(has_modulus()) { repr << " % " << m_orig_modulus; }
    if(has_divisor()) { repr << " / " << m_divisor; }
    return repr.str();
}


} // close namespace bamboo
