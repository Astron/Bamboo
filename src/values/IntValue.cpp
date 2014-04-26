// Filename: IntValue.cpp
#include "IntValue.h"
#include <limits>    // numeric_limits
#include <stdexcept> // invalid_argument
#include <typeinfo>  // bad_cast
#include "../bits/buffers.h"
#include "../module/DistributedType.h"
using namespace std;
namespace bamboo {


template<typename T>
static inline bool within_limits(int64_t num) {
    return numeric_limits<T>::min() <= num && num <= numeric_limits<T>::max();
}
template<typename T>
static inline bool within_limits(uint64_t num) {
    return num <= numeric_limits<T>::max();
}

IntValue::IntValue() { m_unsigned = 0; }
IntValue::IntValue(int8_t num) { m_signed = num; }
IntValue::IntValue(uint8_t num) { m_unsigned = num; }
IntValue::IntValue(int16_t num) { m_signed = num; }
IntValue::IntValue(uint16_t num) { m_unsigned = num; }
IntValue::IntValue(int32_t num) { m_signed = num; }
IntValue::IntValue(uint32_t num) { m_unsigned = num; }
IntValue::IntValue(int64_t num) { m_signed = num; }
IntValue::IntValue(uint64_t num) { m_unsigned = num; }

// pack provides the packed data for the value in native endianness.
//     Throws: bad_cast
vector<uint8_t> IntValue::pack(const DistributedType *type) const {
    if(type->get_subtype() > kTypeChar) {
        throw invalid_argument("can't pack integer value as non-integer type.");
    }

    switch(type->get_size()) {
        case sizeof(uint8_t):
            return as_buffer(uint8_t(m_unsigned));
        case sizeof(uint16_t):
            return as_buffer(uint16_t(m_unsigned));
        case sizeof(uint32_t):
            return as_buffer(uint32_t(m_unsigned));
        case sizeof(uint64_t):
            return as_buffer(uint64_t(m_unsigned));
        default:
            throw invalid_argument("integer type has an invalid bytesize.");
    }
}
void IntValue::pack(const DistributedType *type, vector<uint8_t>& buf) const {
    if(type->get_subtype() > kTypeChar) {
        throw invalid_argument("can't pack integer value as non-integer type.");
    }

    switch(type->get_size()) {
        case sizeof(uint8_t):
            pack_value(uint8_t(m_unsigned), buf);
            break;
        case sizeof(uint16_t):
            pack_value(uint16_t(m_unsigned), buf);
            break;
        case sizeof(uint32_t):
            pack_value(uint32_t(m_unsigned), buf);
            break;
        case sizeof(uint64_t):
            pack_value(uint64_t(m_unsigned), buf);
            break;
        default:
            throw invalid_argument("integer type has an invalid bytesize.");
    }
}

char IntValue::as_char() const {
    if(!within_limits<char>(m_signed)) { throw bad_cast(); }
    return m_signed;
}
int8_t IntValue::as_int8() const {
    if(!within_limits<int8_t>(m_signed)) { throw bad_cast(); }
    return m_signed;
}
int16_t IntValue::as_int16() const {
    if(!within_limits<int16_t>(m_signed)) { throw bad_cast(); }
    return m_signed;
}
int32_t IntValue::as_int32() const {
    if(!within_limits<int32_t>(m_signed)) { throw bad_cast(); }
    return m_signed;
}
int64_t IntValue::as_int64() const {
    if(!within_limits<int64_t>(m_signed)) { throw bad_cast(); }
    return m_signed;
}
uint8_t IntValue::as_uint8() const {
    if(!within_limits<uint8_t>(m_unsigned)) { throw bad_cast(); }
    return m_unsigned;
}
uint16_t IntValue::as_uint16() const {
    if(!within_limits<uint16_t>(m_unsigned)) { throw bad_cast(); }
    return m_unsigned;
}
uint32_t IntValue::as_uint32() const {
    if(!within_limits<uint32_t>(m_unsigned)) { throw bad_cast(); }
    return m_unsigned;
}
uint64_t IntValue::as_uint64() const {
    if(!within_limits<uint64_t>(m_unsigned)) { throw bad_cast(); }
    return m_unsigned;
}


} // close namespace bamboo
