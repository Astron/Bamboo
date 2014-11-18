// Filename: Numeric.h
#pragma once
#include <cmath> // std::fmod
#include "../module/Type.h"
#include "../module/Value.h"
#include "../module/NumericRange.h"
namespace bamboo   // open namespace bamboo
{


// A Numeric can represent any of the basic number types (ie. integers, floats, etc).
//     A Numeric may also have a range and/or modulus to limit its possible values,
//     and/or a divisor representing a fixed-point numeric convention.
//     A divisor scales up any range or modulus to constrain up to (constraint * divisor).
class Numeric : public Type
{
  public:
    explicit Numeric(Subtype type);
    Numeric(Subtype type, const std::string& alias);
    Numeric(const Numeric&) = delete;
    Numeric& operator=(const Numeric&) = delete;
    virtual ~Numeric() {};
    Numeric *as_numeric() override;
    const Numeric *as_numeric() const override;

    inline bool is_signed() const;
    inline unsigned int divisor() const;
    inline bool has_modulus() const;
    inline double modulus() const;
    inline bool has_range() const;
    inline NumericRange range() const;

    inline double clamp(double value) const;
    inline double clamp_fixed(double value) const; // clamp using range scaled by divisor

    inline int8_t to_fixed_s8(double floating) const;
    inline int16_t to_fixed_s16(double floating) const;
    inline int32_t to_fixed_s32(double floating) const;
    inline int64_t to_fixed_s64(double floating) const;
    inline uint8_t to_fixed_u8(double floating) const;
    inline uint16_t to_fixed_u16(double floating) const;
    inline uint32_t to_fixed_u32(double floating) const;
    inline uint64_t to_fixed_u64(double floating) const;

    inline double to_floating(int8_t fixed) const;
    inline double to_floating(int16_t fixed) const;
    inline double to_floating(int32_t fixed) const;
    inline double to_floating(int64_t fixed) const;
    inline double to_floating(uint8_t fixed) const;
    inline double to_floating(uint16_t fixed) const;
    inline double to_floating(uint32_t fixed) const;
    inline double to_floating(uint64_t fixed) const;

    bool set_divisor(unsigned int divisor); // typically to represent fixed-point
    bool set_modulus(double modulus); // typically for degrees/radians
    bool set_range(const NumericRange& range);

  private:
    bool m_signed = true;
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
