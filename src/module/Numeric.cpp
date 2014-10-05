// Filename: Numeric.cpp
#include "Numeric.h"
#include <limits> // for fixed-width integer limits
using namespace std;
namespace bamboo   // open namespace bamboo
{


// Type constructor
Numeric::Numeric(Subtype type)
{
    m_subtype = type;
    switch(type) {
    case kTypeChar:
    case kTypeInt8:
    case kTypeUint8:
        m_size = sizeof(int8_t);
        break;
    case kTypeInt16:
    case kTypeUint16:
        m_size = sizeof(int16_t);
        break;
    case kTypeInt32:
    case kTypeUint32:
        m_size = sizeof(int32_t);
        break;
    case kTypeInt64:
    case kTypeUint64:
        m_size = sizeof(int64_t);
        break;
    case kTypeFloat32:
        m_size = sizeof(float);
        break;
    case kTypeFloat64:
        m_size = sizeof(double);
        break;
    default:
        m_subtype = kTypeInvalid;
    }
}

// as_numeric returns this as a Numeric if it is numeric, or nullptr otherwise.
Numeric *Numeric::as_numeric()
{
    return this;
}
const Numeric *Numeric::as_numeric() const
{
    return this;
}

// set_divisor sets a divisor for the numeric type, typically to represent fixed-point.
//     Returns false if the divisor is not valid for this type.
bool Numeric::set_divisor(unsigned int divisor)
{
    if(divisor == 0) {
        return false;
    }

    m_divisor = divisor;

    if(has_range()) {
        set_range(m_orig_range);
    }
    if(has_modulus()) {
        set_modulus(m_orig_modulus);
    }

    return true;
}

// set_modulus sets a modulus value of the numeric type.
//     Returns false if the modulus is not valid for this type.
bool Numeric::set_modulus(double modulus)
{
    if(modulus <= 0.0) {
        return false;
    }

    double float_modulus = modulus * m_divisor;
    uint64_t uint_modulus = uint64_t(floor(float_modulus + 0.5));

    // Check the range.  A valid range for the modulus is 1 to (maximum_value + 1) after scaling.
    switch(m_subtype) {
    case kTypeChar:
    case kTypeUint8:
        if(uint_modulus < 1 || uint16_t(numeric_limits<uint8_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeUint16:
        if(uint_modulus < 1 || uint32_t(numeric_limits<uint16_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeUint32:
        if(uint_modulus < 1 || uint64_t(numeric_limits<uint32_t>::max()) + 1ull < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeUint64:
        if(uint_modulus < 1) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeInt8:
        if(uint_modulus < 1 || uint16_t(numeric_limits<int8_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeInt16:
        if(uint_modulus < 1 || uint32_t(numeric_limits<int16_t>::max()) + 1u < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeInt32:
        if(uint_modulus < 1 || uint64_t(numeric_limits<int32_t>::max()) + 1ull < uint_modulus) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeInt64:
        if(uint_modulus < 1) {
            return false;
        }
        m_modulus = uint_modulus;
        break;
    case kTypeFloat32:
    case kTypeFloat64:
        m_modulus = float_modulus;
        break;
    default:
        return false;
    }

    m_orig_modulus = modulus;
    return true;
}

// set_range sets a valid range of the numeric type.
//     Returns false if the range is not valid for this type.
bool Numeric::set_range(const NumericRange& range)
{
    // TODO: Accept integer ranges
    if(range.type != Number::kFloat) {
        return false;
    }

    m_orig_range = range;
    switch(m_subtype) {
    case kTypeInt8:
    case kTypeInt16:
    case kTypeInt32:
    case kTypeInt64: {
        int64_t min = (int64_t)floor(range.min.floating * m_divisor + 0.5);
        int64_t max = (int64_t)floor(range.max.floating * m_divisor + 0.5);
        m_range = NumericRange(min, max);
        // TODO: Validate range, i.e. => min and max within (INT[N]_MIN - INT[N]MAX)
        break;
    }
    case kTypeChar:
    case kTypeUint8:
    case kTypeUint16:
    case kTypeUint32:
    case kTypeUint64: {
        uint64_t min = (uint64_t)floor(range.min.floating * m_divisor + 0.5);
        uint64_t max = (uint64_t)floor(range.max.floating * m_divisor + 0.5);
        m_range = NumericRange(min, max);
        // TODO: Validate range, i.e. => min and max within (UINT[N]_MIN - UINT[N]MAX)
        break;
    }
    case kTypeFloat32:
    case kTypeFloat64: {
        double min = range.min.floating * m_divisor;
        double max = range.max.floating * m_divisor;
        m_range = NumericRange(min, max);
        break;
    }
    default: {
        return false;
    }
    }

    return true;
}


} // close namespace bamboo
