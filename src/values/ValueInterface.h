// Filename: ValueInterface.h
#pragma once
#include <string>
#include <vector>
#include "../bits/sizetag.h"
namespace bamboo {


// Foward declarations
class Value;
class DistributedType;

class ValueInterface {
  public:
    virtual ~ValueInterface() {}

    /*
    // copy returns a deep copy of the value
    virtual Value copy() const = 0;
    */

    // pack provides the packed data for the value in native endianness.
    //     Throws: invalid_argument
    virtual std::vector<uint8_t> pack(const DistributedType *) const = 0;
    virtual void pack(const DistributedType *, std::vector<uint8_t>& buf) const = 0;

    // implicit conversion operators.
    //     Throws: bad_cast
    virtual char as_char() const;
    virtual int8_t as_int8() const;
    virtual int16_t as_int16() const;
    virtual int32_t as_int32() const;
    virtual int64_t as_int64() const;
    virtual uint8_t as_uint8() const;
    virtual uint16_t as_uint16() const;
    virtual uint32_t as_uint32() const;
    virtual uint64_t as_uint64() const;
    virtual float as_float32() const;
    virtual double as_float64() const;
    virtual std::string as_string() const;
    virtual std::vector<uint8_t> as_blob() const;

    // size returns the size (the number of items) in a value.
    //     Size may be called on an array, string, blob, struct, or function value.
    virtual sizetag_t size() const;

    // array accessor like methods for pointer operations
    //     Throws: out_of_range
    virtual Value get_item(sizetag_t index);
    virtual Value get_item(const std::string& item);
    virtual const Value get_item(sizetag_t index) const;
    virtual const Value get_item(const std::string& item) const;
    virtual void set_item(sizetag_t index, const Value);
    virtual void set_item(const std::string& item, const Value);
};


} // close namespace bamboo
