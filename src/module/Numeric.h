// Filename: Numeric.h
#pragma once
#include "Type.h"
#include "Value.h"
#include "NumericRange.h"
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

    // as_numeric returns this as a Numeric if it is numeric, or nullptr otherwise.
    Numeric *as_numeric() override;
    const Numeric *as_numeric() const override;

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

    // to_value scales by the divisor and wraps with the modulus to produce a compressed Value.
    //     Typically used to compress reals to fixed-point [/100] or wrap arc degrees [% 360].
    //     Throws range error if the number violates the Numeric's range constraints.
    Value to_value(Number);
    // to_number decompresses the Value (frequently from fixed-point) into the expected number.
    //     Throws range error if the number violates the Numeric's range constraints.
    //     Throws domain error if the value isn't a numeric type.
    double to_floating(const Value&);
    int64_t to_integer(const Value&);
    uint64_t to_uinteger(const Value&);

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
