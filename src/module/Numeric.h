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
    // Type constructor
    explicit Numeric(Subtype type);
    Numeric(const Numeric&) = delete;
    Numeric& operator=(const Numeric&) = delete;
    virtual ~Numeric() {};

    // as_numeric returns this as a Numeric if it is numeric, or nullptr otherwise.
    Numeric *as_numeric() override;
    const Numeric *as_numeric() const override;

    // is_signed returns whether the numeric is a signed type or not.
    inline bool is_signed() const;

    // divisor returns the divisor of the numeric, with a default value of one.
    inline unsigned int divisor() const;

    // has_modulus returns true if the numeric is constrained by a modulus.
    inline bool has_modulus() const;
    // modulus returns a double representation of the modulus value.
    inline double modulus() const;

    // has_range returns true if the numeric is constrained by a range.
    inline bool has_range() const;
    // range returns the NumericRange that constrains the type's values.
    inline NumericRange range() const;
    // clamp returns the value clamped by the Numeric's range.
    inline double clamp(double value) const;
    // clamp_fixed returns the value clamped by Numeric's range scaled by the divisor.
    inline double clamp_fixed(double value) const;

    // to_fixed packs a floating point value into a fixed-point representation.
    // The value is clamped by the numeric's range.
    inline int8_t to_fixed_i8(double floating) const;
    inline int16_t to_fixed_i16(double floating) const;
    inline int32_t to_fixed_i32(double floating) const;
    inline int64_t to_fixed_i64(double floating) const;
    inline uint8_t to_fixed_u8(double floating) const;
    inline uint16_t to_fixed_u16(double floating) const;
    inline uint32_t to_fixed_u32(double floating) const;
    inline uint64_t to_fixed_u64(double floating) const;

    // to_floating unpacks a fixed point value into a floating-point representation.
    // The value is clamped by the numeric's range.
    inline double to_floating(int8_t fixed) const;
    inline double to_floating(int16_t fixed) const;
    inline double to_floating(int32_t fixed) const;
    inline double to_floating(int64_t fixed) const;
    inline double to_floating(uint8_t fixed) const;
    inline double to_floating(uint16_t fixed) const;
    inline double to_floating(uint32_t fixed) const;
    inline double to_floating(uint64_t fixed) const;

    // set_divisor sets a divisor for the numeric type, typically to represent fixed-point.
    //     Returns false if the divisor is not valid for this type.
    bool set_divisor(unsigned int divisor);
    // set_modulus sets a modulus value of the numeric type.
    //     Returns false if the modulus is not valid for this type.
    bool set_modulus(double modulus);
    // set_range sets a valid range of the numeric type.
    //     Returns false if the range is not valid for this type.
    bool set_range(const NumericRange& range);

  private:
    bool m_signed = true;
    unsigned int m_divisor = 1;

    // These are the original range and modulus values from the file, unscaled by the divisor.
    double m_orig_modulus = 0;
    NumericRange m_orig_range = NumericRange();

    // These are the range and modulus values after scaling by the divisor.
    Number m_modulus = Number(0.0);
    NumericRange m_range = NumericRange();
};


} // close namespace bamboo
#include "Numeric.ipp"
