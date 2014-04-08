// Filename: StringValue.h
#pragma once
#include "ValueInterface.h"
namespace bamboo {


class StringValue : public ValueInterface {
  private:
    std::string m_string;

  public:
    StringValue(sizetag_t length = 0);
    StringValue(const std::string&);
    virtual ~StringValue() {}

    // pack provides the packed data for the value in native endianness.
    //     Throws: bad_cast
    virtual std::vector<uint8_t> pack(const DistributedType *) const;
    virtual void pack(const DistributedType *, std::vector<uint8_t>& buf) const;

    virtual std::string as_string() const;

    // size returns the size (the number of items) in a value.
    //     Size may be called on an array, string, blob, struct, or function value.
    virtual sizetag_t size() const;

    // array accessor like methods for pointer operations
    //     Throws: out_of_range
    virtual Value get_item(sizetag_t index);
    virtual const Value get_item(sizetag_t index) const;
    virtual void set_item(sizetag_t index, const Value);
};


} // close namespace bamboo