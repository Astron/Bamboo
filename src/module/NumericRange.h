// Filename: NumericRange.h
#pragma once
#include <limits>
#include "../bits/numbers.h"
namespace bamboo {


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
