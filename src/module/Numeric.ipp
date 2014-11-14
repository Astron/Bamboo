// Filename: Numeric.ipp
namespace bamboo
{


inline bool Numeric::is_signed() const
{
    return m_signed;
}

inline unsigned int Numeric::divisor() const
{
    return m_divisor;
}

inline bool Numeric::has_modulus() const
{
    return m_orig_modulus != 0.0;
}

inline double Numeric::modulus() const
{
    return m_orig_modulus;
}

inline bool Numeric::has_range() const
{
    return !m_orig_range.is_nan();
}

inline NumericRange Numeric::range() const
{
    return m_orig_range;
}

inline double Numeric::clamp(double value) const
{
    double min = double(m_orig_range.min);
    double max = double(m_orig_range.max);
    return value < min ? min : (value > max ? max : value);
}

inline double Numeric::clamp_fixed(double value) const
{
    double min = double(m_range.min);
    double max = double(m_range.max);
    return value < min ? min : (value > max ? max : value);
}

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

TO_FIXED(int8_t, s8);
TO_FIXED(int16_t, s16);
TO_FIXED(int32_t, s32);
TO_FIXED(int64_t, s64);
TO_FIXED(uint8_t, u8);
TO_FIXED(uint16_t, u16);
TO_FIXED(uint32_t, u32);
TO_FIXED(uint64_t, u64);

#undef TO_FIXED

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
