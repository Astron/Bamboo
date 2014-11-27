// Filename: Numeric.h
#pragma once
#include <cmath> // std::fmod
#include "../module/Type.h"
#include "../module/Value.h"
#include "../module/NumericRange.h"
namespace bamboo   // open namespace bamboo
{

enum NumericType {
    Numeric_Invalid = 0,
    Numeric_Int8,
    Numeric_Int16,
    Numeric_Int32,
    Numeric_Int64,
    Numeric_Uint8,
    Numeric_Uint16,
    Numeric_Uint32,
    Numeric_Uint64,
    Numeric_Float32,
    Numeric_Float64,
};

const char *format_numeric_type(NumericType numeric_type);

// A Numeric can represent any of the basic number types (ie. integers, floats, etc).
//     A Numeric may also have a range and/or modulus to limit its possible values,
//     and/or a divisor representing a fixed-point numeric convention.
//     A divisor scales up any range or modulus to constrain up to (constraint * divisor).
class Numeric : public Type
{
  public:
    explicit Numeric(NumericType packtype);
    Numeric(const Numeric&) = delete;
    Numeric& operator=(const Numeric&) = delete;
    virtual ~Numeric() {};

    Numeric *as_numeric() override;
    const Numeric *as_numeric() const override;

    inline bool is_unsigned() const;
    inline bool is_signed() const;
    inline bool is_floating() const;

    inline bool has_divisor() const;
    inline unsigned int divisor() const;
    inline bool has_modulus() const;
    inline double modulus() const;
    inline bool has_range() const;
    inline NumericRange range() const;

#define TRANSFORMS(from, to) \
    inline to scale(from value) const; \
    inline to descale(from value) const; \
    inline to wrap(from value) const; \
    inline to wrap_scaled(from value) const; \
    inline to clamp(from value) const; \
    inline to clamp_scaled(from value) const;

    TRANSFORMS(int8_t,  int64_t);
    TRANSFORMS(int16_t, int64_t);
    TRANSFORMS(int32_t, int64_t);
    TRANSFORMS(int64_t, int64_t);
    TRANSFORMS(uint8_t,  uint64_t);
    TRANSFORMS(uint16_t, uint64_t);
    TRANSFORMS(uint32_t, uint64_t);
    TRANSFORMS(uint64_t, uint64_t);
    TRANSFORMS(float,  double);
    TRANSFORMS(double, double);
#undef TRANSFORMS

    inline NumericType packtype() const;
    inline std::vector<uint8_t> pack(int64_t value) const;
    inline std::vector<uint8_t> pack(uint64_t value) const;
    inline std::vector<uint8_t> pack(double value) const;
    inline size_t pack(int64_t value, uint8_t *buffer) const;
    inline size_t pack(uint64_t value, uint8_t *buffer) const;
    inline size_t pack(double value, uint8_t *buffer) const;
    inline Number unpack(const uint8_t *buffer) const;

    bool set_divisor(unsigned int divisor); // typically to represent fixed-point
    bool set_modulus(double modulus); // typically for degrees/radians
    bool set_range(const NumericRange& range);

    std::string to_string() const override;

  private:
    NumericType m_packtype;
    unsigned int m_divisor = 1;

    // Original range and modulus values from the file, unscaled by the divisor.
    double m_orig_modulus = 0;
    NumericRange m_orig_range = NumericRange();

    // Range and modulus values after scaling by the divisor.
    Number m_modulus = Number(0.0);
    NumericRange m_range = NumericRange();
};


} // close namespace bamboo
#include "Numeric.ipp"
