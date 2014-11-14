// Filename: Array.h
#pragma once
#include <cstddef> // size_t
#include "../module/Type.h"
#include "../module/NumericRange.h"
namespace bamboo
{


// An Array represents an array of some other kind of object, meaning
//     this type describes an arbitrary (or possibly fixed) number of
//     continguous values, all of which are of the same type.
//     Strings and Blobs are arrays with char and uint8 elements respectively.
class Array : public Type
{
  public:
    Array(Type *element_type, const NumericRange& size = NumericRange());
    Array(Type *element_type, const std::string& alias);
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
    virtual ~Array() {};
    Array *as_array() override;
    const Array *as_array() const override;

    inline Type *element_type() { return m_element_type; }
    inline const Type *element_type() const { return m_element_type; }

    // array_size returns the fixed number of elements in this array,
    //    or 0 if the array may contain a variable number of elements.
    inline size_t array_size() const { return m_array_size;}
    inline bool has_range() const { return !m_array_range.is_nan(); }
    inline NumericRange range() const { return m_array_range; }

  private:
    Type *m_element_type;
    unsigned int m_array_size = 0; // number of elements in a fixed-size array (or 0)
    NumericRange m_array_range;
};


} // close namespace bamboo
