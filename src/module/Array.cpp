// Filename: Array.cpp
#include "Array.h"
using namespace std;
namespace bamboo
{


Array::Array(Type *element_type, bool transfer_ownership, const NumericRange& size) :
    Type(Subtype_Array),
    m_element_owned(transfer_ownership),
    m_element_type(element_type),
    m_array_range(size)
{
    if(m_element_type == nullptr) {
        m_element_type = Type::None;
    }

    // Set array bounds
    if(!size.is_nan() && size.min == size.max) {
        m_array_size = (unsigned int)size.min;
    } else {
        m_array_size = 0;
    }

    // Use special types for single-byte unmodified primitive types
    // These types can be serialized and deserialized faster because they don't have endianness
    // and they don't need to be specially packed unlike types with divisors/ranges/etc
    if(m_element_type == Type::Char) {
        m_subtype = Subtype_String;
    } else if(m_element_type == Type::Byte) {
        m_subtype = Subtype_Blob;
    }

    // Set bytesize
    if(m_array_size > 0 && m_element_type->has_fixed_size()) {
        m_size = m_array_size * m_element_type->fixed_size();
    } else {
        m_size = 0;
    }
}

Array::~Array()
{
    if(m_element_owned) { delete m_element_type; }
}

Array *Array::as_array()
{
    return this;
}

const Array *Array::as_array() const
{
    return this;
}


} // close namespace bamboo
