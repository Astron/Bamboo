// Filename: Value.h
#pragma once
#include "ValueInterface.h"
#include <memory> // shared_ptr
namespace bamboo {


// A Value is a local representation of data.
//     We use the Value class to do some magic for us that allows us to
//     manipulate the data as if we were a loosely typed language like python.
//     Value wraps many methods of ValueInterface to provide this abstraction
//     efficiently with a clean API for a library-user.
class Value {
  private:
    static Value from_packed(const DistributedType *, const std::vector<uint8_t>& buf,
                             sizetag_t& offset);

    std::shared_ptr<ValueInterface> m_value;

  public:
    Value() : m_value(nullptr) {}
    Value(ValueInterface *value) : m_value(value) {}

    /* Named constructors */
    // from_type returns the default Value for the provided type.
    //     May throw std::invalid_argument.
    static Value from_type(const DistributedType *);
    static Value from_type(const DistributedType *, bool& is_derived);
    // from_packed unpacks the value for the provided type from a buffer.
    //     May throw std::invalid_argument.
    static Value from_packed(const DistributedType *, const std::vector<uint8_t>& buf);

    /*
    // copy returns a deep copy of the value
    inline Value copy() const { return Value(m_value->copy()); }
    */

    // pack provides the packed data for the value in native endianness.
    //     May throw std::invalid_argument.
    inline std::vector<uint8_t> pack(const DistributedType *type) const {
        return m_value->pack(type);
    }
    inline void pack(const DistributedType *type, std::vector<uint8_t>& buf) const {
        m_value->pack(type, buf);
    }

    // implicit conversion operators
    //     May throw std::bad_cast.
    inline operator char() const { return m_value->as_char(); }
    inline operator int8_t() const { return m_value->as_int8(); }
    inline operator int16_t() const { return m_value->as_int16(); }
    inline operator int32_t() const { return m_value->as_int32(); }
    inline operator int64_t() const { return m_value->as_int64(); }
    inline operator uint8_t() const { return m_value->as_uint8(); }
    inline operator uint16_t() const { return m_value->as_uint16(); }
    inline operator uint32_t() const { return m_value->as_uint32(); }
    inline operator uint64_t() const { return m_value->as_uint64(); }
    inline operator float() const { return m_value->as_float32(); }
    inline operator double() const { return m_value->as_float64(); }
    inline operator std::string() const { return m_value->as_string(); }
    inline operator std::vector<uint8_t>() const { return m_value->as_blob(); }

    // size returns the size (the number of items) in a value.
    //     Size may be called on an array, string, blob, struct, or function value.
    inline sizetag_t size() const { return m_value->size(); }

    // [] -- array subscription may be used to access the items in the value
    // TODO: Some voodoo magic with these to provide
    //     May throw std::out_of_range.
    inline Value operator[](sizetag_t index) { return get_item(index); }
    inline const Value operator[](sizetag_t index) const { return get_item(index); }
    inline Value operator[](const std::string& item) { return get_item(item); }
    inline const Value operator[](const std::string& item) const { return get_item(item); }

    // These functions are primarily for use with bindings.
    // In C++ code, use array subscription instead.
    inline Value get_item(sizetag_t index) { return m_value->get_item(index); }
    inline Value get_item(const std::string& item) { return m_value->get_item(item); }
    inline const Value get_item(sizetag_t index) const { return m_value->get_item(index); }
    inline const Value get_item(const std::string& item) const { return m_value->get_item(item); }
    inline void set_item(sizetag_t index, const Value value) { return m_value->set_item(index, value); }
    inline void set_item(const std::string& item, const Value value) { return m_value->set_item(item, value); }

    // These functions are primarily for use with bindings.
    // In C++ code, use implicit conversions instead.
    inline char as_char() const { return m_value->as_char(); }
    inline int8_t as_int8() const { return m_value->as_int8(); }
    inline int16_t as_int16() const { return m_value->as_int16(); }
    inline int32_t as_int32() const { return m_value->as_int32(); }
    inline int64_t as_int64() const { return m_value->as_int64(); }
    inline uint8_t as_uint8() const { return m_value->as_uint8(); }
    inline uint16_t as_uint16() const { return m_value->as_uint16(); }
    inline uint32_t as_uint32() const { return m_value->as_uint32(); }
    inline uint64_t as_uint64() const { return m_value->as_uint64(); }
    inline float as_float32() const { return m_value->as_float32(); }
    inline double as_float64() const { return m_value->as_float64(); }
    inline std::string as_string() const { return m_value->as_string(); }
    inline std::vector<uint8_t> as_blob() const { return m_value->as_blob(); }
};


} // close namespace bamboo
