// Filename: Buffer.h
#pragma once
#include "buffers.h"
#include <stdexcept>
namespace bamboo {


// A Buffer is a convenience object for building and reading data.
// It is primarily an interface for language bindings, but can also be used by end programs.
// It should not be used by Bamboo developers within the C++ core, see buffers.h instead.
struct Buffer {
    std::vector<uint8_t> data;
    sizetag_t offset;

    Buffer() : data(), offset(0) {}
    Buffer(std::vector<uint8_t> buf) : data(buf), offset(0) {}
    Buffer(const Buffer& buf) : data(buf.data), offset(buf.offset) {}
    Buffer& operator=(std::vector<uint8_t> rhs) {
        data = rhs;
        offset = 0;
        return *this;
    }
    operator std::vector<uint8_t>&() { return data; }

    Buffer copy() { return Buffer(*this); }

    uint8_t& operator[](sizetag_t index) { return data[index]; }
    const uint8_t& operator[](sizetag_t index) const { return data[index]; }

    // These functions are primarily for use with bindings.
    // In C++ code, use array subscription instead.
    uint8_t get_byte(sizetag_t index) const {
        if(index >= data.size()) { throw std::range_error("Buffer index out of range."); }
        return data[index];
    }
    void set_byte(sizetag_t index, uint8_t byte){
        if(index >= data.size()) { throw std::range_error("Buffer index out of range."); }
        data[index] = byte;
    }

    inline bool read_bool() { return bamboo::read_bool(data, offset); }
    inline char read_char() { return bamboo::read_char(data, offset); }
    inline int8_t read_int8() { return bamboo::read_int8(data, offset); }
    inline int16_t read_int16() { return bamboo::read_int16(data, offset); }
    inline int32_t read_int32() { return bamboo::read_int32(data, offset); }
    inline int64_t read_int64() { return bamboo::read_int64(data, offset); }
    inline uint8_t read_uint8() { return bamboo::read_uint8(data, offset); }
    inline uint16_t read_uint16() { return bamboo::read_uint16(data, offset); }
    inline uint32_t read_uint32() { return bamboo::read_uint32(data, offset); }
    inline uint64_t read_uint64() { return bamboo::read_uint64(data, offset); }
    inline float read_float32() { return bamboo::read_float32(data, offset); }
    inline double read_float64() { return bamboo::read_float64(data, offset); }

    inline void pack_bool(bool r) { return pack_value(r, data); }
    inline void pack_char(char r) { return pack_value(r, data); }
    inline void pack_int8(int8_t r) { return pack_value(r, data); }
    inline void pack_int16(int16_t r) { return pack_value(r, data); }
    inline void pack_int32(int32_t r) { return pack_value(r, data); }
    inline void pack_int64(int64_t r) { return pack_value(r, data); }
    inline void pack_uint8(uint8_t r) { return pack_value(r, data); }
    inline void pack_uint16(uint16_t r) { return pack_value(r, data); }
    inline void pack_uint32(uint32_t r) { return pack_value(r, data); }
    inline void pack_uint64(uint64_t r) { return pack_value(r, data); }
    inline void pack_float32(float r) { return pack_value(r, data); }
    inline void pack_float64(double r) { return pack_value(r, data); }
    inline void pack_string(const std::string& r) { return pack_value(r, data); }
    inline void pack_buffer(const std::vector<uint8_t>& r) { return pack_value(r, data); }

    static Buffer from_bool(bool r) { return as_buffer(r); }
    static Buffer from_char(char r) { return as_buffer(r); }
    static Buffer from_int8(int8_t r) { return as_buffer(r); }
    static Buffer from_int16(int16_t r) { return as_buffer(r); }
    static Buffer from_int32(int32_t r) { return as_buffer(r); }
    static Buffer from_int64(int64_t r) { return as_buffer(r); }
    static Buffer from_uint8(uint8_t r) { return as_buffer(r); }
    static Buffer from_uint16(uint16_t r) { return as_buffer(r); }
    static Buffer from_uint32(uint32_t r) { return as_buffer(r); }
    static Buffer from_uint64(uint64_t r) { return as_buffer(r); }
    static Buffer from_float32(float r) { return as_buffer(r); }
    static Buffer from_float64(double r) { return as_buffer(r); }
    static Buffer from_string(const std::string& r) { return as_buffer(r); }
};

/*
struct ReadOnlyBuffer {
    const std::vector<uint8_t>& data;
    sizetag_t offset;

    ReadOnlyBuffer(const ReadOnlyBuffer& buf) : data(buf.data), offset(buf.offset) {}
    ReadOnlyBuffer& operator=(const std::vector<uint8_t>& rhs) {
        data = rhs;
        offset = 0;
        return *this;
    }
    operator const std::vector<uint8_t>&() const { return data; }

    inline const uint8_t& operator[](size_t index) const { return data[index]; }

    inline bool read_bool() { return bamboo::read_bool(data, offset); }
    inline char read_char() { return bamboo::read_char(data, offset); }
    inline int8_t read_int8() { return bamboo::read_int8(data, offset); }
    inline int16_t read_int16() { return bamboo::read_int16(data, offset); }
    inline int32_t read_int32() { return bamboo::read_int32(data, offset); }
    inline int64_t read_int64() { return bamboo::read_int64(data, offset); }
    inline uint8_t read_uint8() { return bamboo::read_uint8(data, offset); }
    inline uint16_t read_uint16() { return bamboo::read_uint16(data, offset); }
    inline uint32_t read_uint32() { return bamboo::read_uint32(data, offset); }
    inline uint64_t read_uint64() { return bamboo::read_uint64(data, offset); }
    inline float read_float32() { return bamboo::read_float32(data, offset); }
    inline double read_float64() { return bamboo::read_float64(data, offset); }
};
*/


} // close namespace bamboo
