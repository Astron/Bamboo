// Filename: Array.h
#pragma once
#include <stddef.h> // for size_t
#include "../module/Type.h"
#include "../module/NumericRange.h"
namespace bamboo   // open namespace
{


// An Array represents an array of some other kind of object, meaning
//     this parameter type accepts an arbitrary (or possibly fixed) number of
//     nested fields, all of which are of the same type.
//     Strings and blobs are arrays with char and uint8 elements respectively.
// Array's constructor throws null_error if the type is null.
class Array : public Type
{
  public:
    Array(Type *element_type, const NumericRange& size = NumericRange()); // TODO: throw null_error
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
    virtual ~Array() {};

    // as_array returns this as a Array if it is an array/string/blob, or nullptr otherwise.
    Array *as_array() override;
    const Array *as_array() const override;

    // element_type returns the type of the individual elements of this array.
    inline Type *element_type();
    inline const Type *element_type() const;

    // array_size returns the fixed number of elements in this array,
    //     or 0 if the array may contain a variable number of elements.
    inline size_t array_size() const;

    // has_range returns true if there is a constraint on the range of valid array sizes.
    //     This is always true for fixed-size arrays.
    inline bool has_range() const;
    // range returns the range of sizes that the array may have.
    inline NumericRange range() const;

  private:
    Type *m_element_type; // type of the elements contained in the array
    unsigned int m_array_size; // number of elements in the array if it is a constant (or 0)
    NumericRange m_array_range; // the range of possible elements in the array
};


} // close namespace bamboo
#include "Array.ipp"
