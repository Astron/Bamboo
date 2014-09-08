// Filename: ArrayValue.h
#pragma once
#include "ValueInterface.h"
namespace bamboo {


class ArrayValue : public ValueInterface {
  private:
    std::vector<Value> m_elements;

  public:
    ArrayValue();
    virtual ~ArrayValue() {}

    // pack provides the packed data for the value in native endianness.
    //     Throws: bad_cast
    virtual std::vector<uint8_t> pack(const DistributedType *) const;
    virtual void pack(const DistributedType *, std::vector<uint8_t>& buf) const;

    // size returns the size (the number of items) in a value.
    //     Size may be called on an array, string, blob, struct, or function value.
    virtual sizetag_t size() const;

    // array accessor like methods for pointer operations
    //     Throws: out_of_range
    virtual Value _getitem_(sizetag_t index);
    virtual const Value _getitem_(sizetag_t index) const;
    virtual void _setitem_(sizetag_t index, const Value);

    void append(const Value);
};


} // close namespace bamboo
