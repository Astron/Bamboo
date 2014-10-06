// Filename: Array.cpp
#include "Array.h"
using namespace std;
namespace bamboo   // open namespace
{


// type constructor
Array::Array(Type *element_type, const NumericRange& size) : m_element_type(element_type)
{
    // Determine array range/size
    if(size.is_nan()) {
        m_array_size = 0;
        m_array_range = NumericRange(uint64_t(0), numeric_limits<uint64_t>::max());
    } else if(size.min == size.max) {
        m_array_size = (unsigned int)size.min;
    } else {
        m_array_size = 0;
        m_array_range = NumericRange(uint64_t(m_array_range.min), uint64_t(m_array_range.max));
    }

    // Set subtype
    if(m_element_type->subtype() == kTypeChar) {
        m_subtype = kTypeString;
    } else if(m_element_type->subtype() == kTypeUint8) {
        m_subtype = kTypeBlob;
    } else {
        m_subtype = kTypeArray;
    }

    // Set byte size
    if(m_array_size > 0 && m_element_type->has_fixed_size()) {
        m_size = m_array_size * m_element_type->fixed_size();
    } else {
        m_size = 0;
    }
}

// as_array returns this as an Array if it is an array, or nullptr otherwise.
Array *Array::as_array()
{
    return this;
}
const Array *Array::as_array() const
{
    return this;
}


} // close namespace bamboo
