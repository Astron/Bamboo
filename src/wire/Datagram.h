#pragma once
#include <string>    // for std::string
#include <vector>    // for std::vector
#include <sstream>   // for std::sstream
#include <stdexcept> // for std::runtime_error
#include <string.h>  // for memcpy
#include "traits/sizetag.h"
#include "traits/byteorder.h"
namespace bamboo {  // close namespace bamboo


// Foward Declarations
class DistributedType;
class Field;

// A DatagramOverflow is an exception which occurs when an add_<value> method is called which would
// increase the size of the datagram past DGSIZE_MAX (preventing integer and buffer overflow).
class DatagramOverflow : public std::runtime_error {
public:
    DatagramOverflow(const std::string& what) : std::runtime_error(what) { }
};

// A BufferEOF is thrown when attempting to read past the end of a provided buffer.
class BufferEOF : public std::runtime_error {
  public:
    BufferEOF(const std::string& what) : std::runtime_error(what) { }
};

// A Datagram is a buffer of binary data in network-endianness.
class Datagram {
  protected:
    uint8_t *buf;
    sizetag_t buf_cap;
    sizetag_t buf_offset;

    void check_add_length(sizetag_t len) {
        if(buf_offset + len > kSizetagMax) {
            std::stringstream error;
            error << "Datagram tried to add data past max datagram size, buf_offset+len("
                  << buf_offset + len << ")" << " max_size(" << kSizetagMax << ")\n";
            throw DatagramOverflow(error.str());
        }

        if(buf_offset + len > buf_cap) {
            uint8_t *tmp_buf = new uint8_t[buf_cap + len + 64];
            memcpy(tmp_buf, buf, buf_cap);
            delete [] buf;
            buf = tmp_buf;
            buf_cap = buf_cap + len + 64;
        }
    }

    void check_read_length(const std::vector<uint8_t>& buffer, sizetag_t offset, sizetag_t len) {
        if(offset + len >= buffer.size()) {
            std::string error("Datagram could not add data from provided buffer;"
                             " reached end of buffer.");
            throw BufferEOF(error);
        }
    }

  public:
    // default-constructor:
    //     creates a new datagram with some pre-allocated space
    Datagram() : buf(new uint8_t[64]), buf_cap(64), buf_offset(0) {}

    // sized-constructor:
    //     allows you to specify the capacity of the datagram ahead of time,
    //     this should be used when the exact size is known ahead of time for performance
    Datagram(sizetag_t capacity) : buf(new uint8_t[capacity]), buf_cap(capacity), buf_offset(0) {}

    // copy-constructor:
    //     creates a new datagram which is a deep-copy of another datagram;
    //     capacity is not perserved and instead is reduced to the size of the source datagram.
    Datagram(const Datagram& dg) :
        buf(new uint8_t[dg.size()]),
        buf_cap(dg.size()),
        buf_offset(dg.size())
    {
        memcpy(buf, dg.buf, dg.size());
    }

    // shallow-constructor:
    //     creates a new datagram that uses an existing buffer as its data
    Datagram(uint8_t *data, sizetag_t length, sizetag_t capacity) :
        buf(data), buf_cap(capacity), buf_offset(length) {}

    // binary-constructor(pointer):
    //     creates a new datagram with a copy of the data contained at the pointer.
    Datagram(const uint8_t *data, sizetag_t length) :
        buf(new uint8_t[length]),
        buf_cap(length),
        buf_offset(length)
    {
        memcpy(buf, data, length);
    }

    // binary-constructor(vector):
    //     creates a new datagram with a copy of the binary data contained in a vector<uint8_t>.
    Datagram(const std::vector<uint8_t>& data) :
        buf(new uint8_t[data.size()]),
        buf_cap(data.size()),
        buf_offset(data.size())
    {
        memcpy(buf, &data[0], data.size());
    }

    // binary-constructor(string):
    //     creates a new datagram with a copy of the data contained in a string, treated as binary.
    Datagram(const std::string& data) :
        buf(new uint8_t[data.length()]),
        buf_cap(data.length()),
        buf_offset(data.length())
    {
        memcpy(buf, data.c_str(), data.length());
    }

    // destructor
    ~Datagram() { delete [] buf; }

    // add_bool adds an 8-bit integer to the datagram that is guaranteed
    // to be one of the values 0x00 (false) or 0x01 (true).
    void add_bool(const bool& v) {
        if(v) { add_uint8(1); }
        else { add_uint8(0); }
    }

    // add_char adds an 8-bit ascii-character value to the datagram.
    void add_char(const char& v) {
        check_add_length(1);
        *(char *)(buf + buf_offset) = v;
        buf_offset += 1;
    }

    // add_int8 adds a signed 8-bit integer value to the datagram.
    void add_int8(const int8_t& v) {
        check_add_length(1);
        *(int8_t *)(buf + buf_offset) = v;
        buf_offset += 1;
    }

    // add_int16 adds a signed 16-bit integer value to the datagram arranged in little-endian.
    void add_int16(const int16_t& v) {
        check_add_length(2);
        *(int16_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 2;
    }

    // add_int32 adds a signed 32-bit integer value to the datagram arranged in little-endian.
    void add_int32(const int32_t& v) {
        check_add_length(4);
        *(int32_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 4;
    }

    // add_int64 adds a signed 64-bit integer value to the datagram arranged in little-endian.
    void add_int64(const int64_t& v) {
        check_add_length(8);
        *(int64_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 8;
    }

    // add_uint8 adds an unsigned 8-bit integer value to the datagram.
    void add_uint8(const uint8_t& v) {
        check_add_length(1);
        *(uint8_t *)(buf + buf_offset) = v;
        buf_offset += 1;
    }

    // add_uint16 adds a unsigned 16-bit integer value to the datagram arranged in little-endian.
    void add_uint16(const uint16_t& v) {
        check_add_length(2);
        *(uint16_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 2;
    }

    // add_uint32 adds a unsigned 32-bit integer value to the datagram arranged in little-endian.
    void add_uint32(const uint32_t& v) {
        check_add_length(4);
        *(uint32_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 4;
    }

    // add_uint64 adds a unsigned 64-bit integer value to the datagram arranged in little-endian.
    void add_uint64(const uint64_t& v) {
        check_add_length(8);
        *(uint64_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 8;
    }

    // add_float32 adds a float (32-bit IEEE 754 floating point) value to the datagram.
    void add_float32(const float& v) {
        check_add_length(4);
        *(float *)(buf + buf_offset) = swap_le(v);
        buf_offset += 4;
    }

    // add_float64 adds a double (64-bit IEEE 754 floating point) value to the datagram.
    void add_float64(const double& v) {
        check_add_length(8);
        *(double *)(buf + buf_offset) = swap_le(v);
        buf_offset += 8;
    }

    // add_size adds a length-tag to the datagram.
    void add_size(const sizetag_t& v) {
        check_add_length(sizeof(sizetag_t));
        *(sizetag_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += sizeof(sizetag_t);
    }

    // add_data adds raw binary data directly to the end of the datagram.
    void add_data(const std::vector<uint8_t>& data) {
        if(data.size()) {
            check_add_length(data.size());
            memcpy(buf + buf_offset, &data[0], data.size());
            buf_offset += data.size();
        }
    }
    void add_data(const std::string& str) {
        if(str.length()) {
            check_add_length(str.length());
            memcpy(buf + buf_offset, str.c_str(), str.length());
            buf_offset += str.length();
        }
    }
    void add_data(const uint8_t *data, sizetag_t length) {
        if(length) {
            check_add_length(length);
            memcpy(buf + buf_offset, data, length);
            buf_offset += length;
        }
    }
    void add_data(const Datagram& dg) {
        if(dg.buf_offset) {
            check_add_length(dg.buf_offset);
            memcpy(buf + buf_offset, dg.buf, dg.buf_offset);
            buf_offset += dg.buf_offset;
        }
    }

    // add_string adds a bamboo string to the datagram from binary data;
    // a length tag (typically a uint16_t) is prepended to the string before it is added.
    void add_string(const std::string& str) {
        add_size(str.length());
        check_add_length(str.length());
        memcpy(buf + buf_offset, str.c_str(), str.length());
        buf_offset += str.length();
    }
    void add_string(const char *str, sizetag_t length) {
        add_size(length);
        check_add_length(length);
        memcpy(buf + buf_offset, str, length);
        buf_offset += length;
    }

    // add_blob adds a bamboo blob to the datagram from binary data;
    // a length tag (typically a uint16_t) is prepended to the blob before it is added.
    void add_blob(const std::vector<uint8_t>& blob) {
        add_size(blob.size());
        check_add_length(blob.size());
        memcpy(buf + buf_offset, &blob[0], blob.size());
        buf_offset += blob.size();
    }
    void add_blob(const uint8_t *data, sizetag_t length) {
        add_size(length);
        check_add_length(length);
        memcpy(buf + buf_offset, data, length);
        buf_offset += length;
    }
    void add_blob(const Datagram& dg) {
        add_size(dg.buf_offset);
        check_add_length(dg.buf_offset);
        memcpy(buf + buf_offset, dg.buf, dg.buf_offset);
        buf_offset += dg.buf_offset;
    }

    // add_buffer reserves a buffer of size "length" at the end of the datagram
    // and returns a pointer to the buffer so it can be filled manually
    uint8_t *add_buffer(sizetag_t length) {
        check_add_length(length);
        uint8_t *buf_start = buf + buf_offset;
        buf_offset += length;
        return buf_start;
    }

    // add_field adds a packed field value to the datagram, converting byte-order
    // from native-endianess to wire-endianess (if necessary).
    // Returns the number of bytes read from the buffer.
    sizetag_t add_field(const Field* field, const std::vector<uint8_t>& packed);

    // add_dtype adds a packed value with the given type to the datagram, converting
    // byte-order from native-endianess to wire-endianess (if necessary).
    // Returns the number of bytes read from the buffer.
    sizetag_t add_dtype(const DistributedType *dtype,
                        const std::vector<uint8_t>& packed,
                        sizetag_t offset = 0);

    // size returns the amount of data added to the datagram in bytes.
    sizetag_t size() const {
        return buf_offset;
    }

    // cap returns the currently allocated size of the datagram in memory (ie. capacity).
    sizetag_t cap() const {
        return buf_cap;
    }

    // get_data returns a pointer to the start of the Datagram's data buffer.
    const uint8_t *get_data() const {
        return buf;
    }
};


} // close namespace bamboo
