// Filename: Numeric.ipp
namespace bamboo
{

inline bool Numeric::is_unsigned() const
{
    return Numeric_Uint8 <= m_packtype && m_packtype <= Numeric_Uint64;
}

inline bool Numeric::is_signed() const
{
    return Numeric_Int8 <= m_packtype && m_packtype <= Numeric_Int64;
}

inline bool Numeric::is_floating() const
{
    return (m_packtype == Numeric_Float32) || (m_packtype == Numeric_Float64);
}

inline bool Numeric::has_divisor() const
{
    return m_divisor != 1;
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

inline NumericType Numeric::packtype() const
{
    return m_packtype;
}

inline std::vector<uint8_t> Numeric::pack(int64_t value) const
{
    uint8_t buf[8];
    pack(value, buf);
    return std::vector<uint8_t>(buf, &buf[m_size]);
}

inline std::vector<uint8_t> Numeric::pack(uint64_t value) const
{
    uint8_t buf[8];
    pack(value, buf);
    return std::vector<uint8_t>(buf, &buf[m_size]);
}


inline std::vector<uint8_t> Numeric::pack(double value) const
{
    uint8_t buf[8];
    pack(value, buf);
    return std::vector<uint8_t>(buf, &buf[m_size]);
}


inline size_t Numeric::pack(int64_t value, uint8_t *buffer) const
{
    switch(m_packtype) {
    case Numeric_Uint8:
        *(uint8_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint16:
        *(uint16_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint32:
        *(uint32_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint64:
        *(uint64_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int8:
        *(int8_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int16:
        *(int16_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int32:
        *(int32_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int64:
        *(int64_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Float32:
        *(float*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Float64:
        *(double*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Invalid:
        break;
    }

    return m_size;
}

inline size_t Numeric::pack(uint64_t value, uint8_t *buffer) const
{
    switch(m_packtype) {
    case Numeric_Uint8:
        *(uint8_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint16:
        *(uint16_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint32:
        *(uint32_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint64:
        *(uint64_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int8:
        *(int8_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int16:
        *(int16_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int32:
        *(int32_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int64:
        *(int64_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Float32:
        *(float*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Float64:
        *(double*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Invalid:
        break;
    }

    return m_size;
}

inline size_t Numeric::pack(double value, uint8_t *buffer) const
{
    switch(m_packtype) {
    case Numeric_Uint8:
        *(uint8_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint16:
        *(uint16_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint32:
        *(uint32_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Uint64:
        *(uint64_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int8:
        *(int8_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int16:
        *(int16_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int32:
        *(int32_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Int64:
        *(int64_t*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Float32:
        *(float*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Float64:
        *(double*)buffer = clamp_scaled(wrap_scaled(scale(value)));
        break;
    case Numeric_Invalid:
        break;
    }

    return m_size;
}

inline Number Numeric::unpack(const uint8_t *buffer) const
{
    switch(m_packtype) {
    case Numeric_Uint8:
        return clamp(descale((*(uint8_t *)buffer)));
    case Numeric_Uint16:
        return clamp(descale((*(uint16_t *)buffer)));
    case Numeric_Uint32:
        return clamp(descale((*(uint32_t *)buffer)));
    case Numeric_Uint64:
        return clamp(descale((*(uint64_t *)buffer)));
    case Numeric_Int8:
        return clamp(descale((*(int8_t *)buffer)));
    case Numeric_Int16:
        return clamp(descale((*(int16_t *)buffer)));
    case Numeric_Int32:
        return clamp(descale((*(int32_t *)buffer)));
    case Numeric_Int64:
        return clamp(descale((*(int64_t *)buffer)));
    case Numeric_Float32:
        return clamp(descale((*(float *)buffer)));
    case Numeric_Float64:
        return clamp(descale((*(double *)buffer)));
    case Numeric_Invalid:
        break;
    }

    return Number(); // NaN
}

// Define transform methods
#define SCALE(from, to) \
inline to Numeric::scale(from value) const { \
    if(has_divisor()) { return (to)(value * m_divisor); } \
    else { return (to)value; } \
}

#define DESCALE(from, to) \
inline to Numeric::descale(from value) const { \
    if(has_divisor()) { return (to)(value / m_divisor); } \
    else { return (to)value; } \
}

#define WRAP(from, to) \
inline to Numeric::wrap(from value) const { \
    if(has_modulus()) { return (to)(value % (uint64_t)(m_orig_modulus + 0.5)); } \
    else { return (to)value; } \
}

#define WRAP_FLOAT(from, to) \
inline to Numeric::wrap(from value) const { \
    to wrapped = (to)value; \
    if(has_modulus()) { \
        if(value < 0.0) { \
            wrapped = m_orig_modulus - std::fmod(-value, m_orig_modulus); \
            if(wrapped == m_orig_modulus) { wrapped = 0.0; } \
        } else { \
            wrapped = std::fmod(value, m_orig_modulus); \
        } \
    } \
    return wrapped; \
}

#define WRAP_SCALED(from, to) \
inline to Numeric::wrap_scaled(from value) const { \
    if(has_modulus()) { return (to)(value % m_modulus.uinteger); } \
    else { return (to)value; } \
}

#define WRAP_SCALED_FLOAT(from, to) \
inline to Numeric::wrap_scaled(from value) const { \
    to wrapped = (to)value; \
    if(has_modulus()) { \
        double fmodulus = m_modulus.floating; \
        if(value < 0.0) { \
            wrapped = fmodulus - std::fmod(-value, fmodulus); \
            if(wrapped == fmodulus) { wrapped = 0.0; } \
        } else { \
            wrapped = std::fmod(value, fmodulus); \
        } \
    } \
    return wrapped; \
}

#define CLAMP(from, to) \
inline to Numeric::clamp(from value) const { \
    if(has_range()) { \
        to min = (to) m_orig_range.min; \
        to max = (to) m_orig_range.max; \
        return value < min ? min : (value > max ? max : (to)value); \
    } else { \
        return (to)value; \
    } \
}

#define CLAMP_SCALED_INT(from, to) \
inline to Numeric::clamp_scaled(from value) const { \
    if(has_range()) { \
        to min = m_range.min.sinteger; \
        to max = m_range.max.sinteger; \
        return value < min ? min : (value > max ? max : (to)value); \
    } else { \
        return (to)value; \
    } \
}

#define CLAMP_SCALED_UINT(from, to) \
inline to Numeric::clamp_scaled(from value) const { \
    if(has_range()) { \
        to min = m_range.min.uinteger; \
        to max = m_range.max.uinteger; \
        return value < min ? min : (value > max ? max : (to)value); \
    } else { \
        return (to)value; \
    } \
}

#define CLAMP_SCALED_FLOAT(from, to) \
inline to Numeric::clamp_scaled(from value) const { \
    if(has_range()) { \
        to min = m_range.min.floating; \
        to max = m_range.max.floating; \
        return value < min ? min : (value > max ? max : (to)value); \
    } else { \
        return (to)value; \
    } \
}

// Signed integer transforms
#define TRANSFORMS_INT(from, to) \
        SCALE(from, to) \
        DESCALE(from, to) \
        WRAP(from, to) \
        WRAP_SCALED(from, to) \
        CLAMP(from, to) \
        CLAMP_SCALED_INT(from, to)

    TRANSFORMS_INT(int8_t,  int64_t)
    TRANSFORMS_INT(int16_t, int64_t)
    TRANSFORMS_INT(int32_t, int64_t)
    TRANSFORMS_INT(int64_t, int64_t)

#undef TRANSFORMS_INT

// Unsigned integer transforms
#define TRANSFORMS_UINT(from, to) \
        SCALE(from, to) \
        DESCALE(from, to) \
        WRAP(from, to) \
        WRAP_SCALED(from, to) \
        CLAMP(from, to) \
        CLAMP_SCALED_UINT(from, to)

    TRANSFORMS_UINT(uint8_t,  uint64_t)
    TRANSFORMS_UINT(uint16_t, uint64_t)
    TRANSFORMS_UINT(uint32_t, uint64_t)
    TRANSFORMS_UINT(uint64_t, uint64_t)

#undef TRANSFORMS_UINT

// Floating transforms
#define TRANSFORMS_FLOAT(from, to) \
        SCALE(from, to) \
        DESCALE(from, to) \
        WRAP_FLOAT(from, to) \
        WRAP_SCALED_FLOAT(from, to) \
        CLAMP(from, to) \
        CLAMP_SCALED_FLOAT(from, to)

    TRANSFORMS_FLOAT(float,  double)
    TRANSFORMS_FLOAT(double, double)

#undef TRANSFORMS_FLOAT

// Undefine transforms methods
#undef CLAMP_SCALED_FLOAT
#undef CLAMP_SCALED_UINT
#undef CLAMP_SCALED_INT
#undef CLAMP
#undef WRAP_SCALED_FLOAT
#undef WRAP_SCALED
#undef WRAP_FLOAT
#undef WRAP
#undef SCALE


} // close namespace bamboo
