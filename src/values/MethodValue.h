// Filename: MethodValue.h
#pragma once
#include "ValueInterface.h"
#include <unordered_map>
#include "../values/Value.h"
namespace bamboo {


// Foward declarations
class Method;

class MethodValue : public ValueInterface {
  private:
    const Method *m_method;
    std::unordered_map<sizetag_t, Value> m_params;

  public:
    MethodValue(const Method *);
    virtual ~MethodValue() {}

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
    virtual Value _getitem_(const std::string& item);
    virtual const Value _getitem_(sizetag_t index) const;
    virtual const Value _getitem_(const std::string& item) const;
    virtual void _setitem_(sizetag_t index, const Value);
    virtual void _setitem_(const std::string& item, const Value);
};


} // close namespace bamboo
