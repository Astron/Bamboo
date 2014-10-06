// Filename: Numeric.ipp
namespace bamboo { // open namespace


// signed returns whether the numeric is a signed type or not.
inline bool Numeric::is_signed() const {
    return m_signed;
}

// divisor returns the divisor of the numeric, with a default value of one.
inline unsigned int Numeric::divisor() const {
    return m_divisor;
}

// has_modulus returns true if the numeric is constrained by a modulus.
inline bool Numeric::has_modulus() const {
    return m_orig_modulus != 0.0;
}

// modulus returns a double representation of the modulus value.
inline double Numeric::modulus() const {
    return m_orig_modulus;
}

// has_range returns true if the numeric is constrained by a range.
inline bool Numeric::has_range() const {
    return !m_orig_range.is_nan();
}
// range returns the NumericRange that constrains the type's values.
inline NumericRange Numeric::range() const {
    return m_orig_range;
}
// clamp returns the value clamped by the Numeric's range.
inline double Numeric::clamp(double value) const {
    double min = double(m_orig_range.min);
    double max = double(m_orig_range.max);
    return value < min ? min : (value > max ? max : value);
}
// clamp_fixed returns the value clamped by Numeric's range scaled by the divisor.
inline double Numeric::clamp_fixed(double value) const {
    double min = double(m_range.min);
    double max = double(m_range.max);
    return value < min ? min : (value > max ? max : value);
}


// to_fixed packs a floating point value into a fixed-point representation.
// The value is clamped by the numeric's range.
#define TO_FIXED(inttype, suffix) \
    inline inttype Numeric::to_fixed_##suffix(double floating) const { \
        double real_value = floating * m_divisor; \
        if(has_modulus()) { \
            double fmodulus = double(m_modulus); \
            if(real_value < 0.0) { \
                real_value = fmodulus - std::fmod(-real_value, fmodulus); \
                if(real_value == fmodulus) { real_value = 0.0; } \
            } else { \
                real_value = fmod(real_value, fmodulus); \
            } \
        } \
        real_value = clamp_fixed(floor(real_value + 0.5)); \
        return inttype(real_value); \
    }

TO_FIXED(int8_t, i8);
TO_FIXED(int16_t, i16);
TO_FIXED(int32_t, i32);
TO_FIXED(int64_t, i64);
TO_FIXED(uint8_t, u8);
TO_FIXED(uint16_t, u16);
TO_FIXED(uint32_t, u32);
TO_FIXED(uint64_t, u64);

#undef TO_FIXED


// to_floating unpacks a fixed point value into a floating-point representation.
// The value is clamped by the numeric's range.
#define TO_FLOATING(inttype) \
    inline double Numeric::to_floating(inttype fixed) const { \
        double value = double(fixed); \
        if(m_divisor != 1) { value = value / m_divisor; } \
        return clamp(value); \
    }

TO_FLOATING(int8_t);
TO_FLOATING(int16_t);
TO_FLOATING(int32_t);
TO_FLOATING(int64_t);
TO_FLOATING(uint8_t);
TO_FLOATING(uint16_t);
TO_FLOATING(uint32_t);
TO_FLOATING(uint64_t);

#undef TO_FLOATING


} // close namespace bamboo
