// Filename: FloatValue.h
#pragma once
#include "ValueInterface.h"
namespace bamboo {


class FloatValue : public ValueInterface {
  private:
    double m_float;

  public:
    FloatValue();
    FloatValue(float);
    FloatValue(double);
    virtual ~FloatValue() {}

    // pack provides the packed data for the value in native endianness.
    //     Throws: bad_cast
    virtual std::vector<uint8_t> pack(const DistributedType*) const;
    virtual void pack(const DistributedType*, std::vector<uint8_t>& buf) const;

    virtual float as_float32() const;
    virtual double as_float64() const;
};


} // close namespace bamboo
