#pragma once
#include <cstring>   // memcpy
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>
#include <sstream>
#include "../bits/sizetag.h"
#include "../bits/byteorder.h"
namespace bamboo
{


class Type;
class Value;

// A DatagramOverflow is an exception which occurs when an add_<value> method is called which would
// increase the size of the datagram past DGSIZE_MAX (preventing integer and buffer overflow).
class DatagramOverflow : public std::runtime_error
{
  public:
    DatagramOverflow(const std::string& what) : std::runtime_error(what) { }
};

// A BufferEOF is thrown when attempting to read past the end of a provided buffer.
class BufferEOF : public std::runtime_error
{
  public:
    BufferEOF(const std::string& what) : std::runtime_error(what) { }
};

// A Datagram is a buffer of binary data in network-endianness.
class Datagram
{
  protected:
    uint8_t *buf;
    sizetag_t buf_cap;
    sizetag_t buf_offset;

    void check_add_length(sizetag_t len)
    {
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

    void check_read_length(const std::vector<uint8_t>& buffer, sizetag_t offset, sizetag_t len)
    {
        if(offset + len >= buffer.size()) {
            std::string error("Datagram could not add data from provided buffer;"
                              " reached end of buffer.");
            throw BufferEOF(error);
        }
    }

  public:
    Datagram() : buf(new uint8_t[64]), buf_cap(64), buf_offset(0) {}
    Datagram(const Datagram& dg) : buf(new uint8_t[dg.size()]), buf_cap(dg.size()), buf_offset(dg.size())
    {
        memcpy(buf, dg.buf, dg.size());
    }
    Datagram& operator=(Datagram dg)
    {
        swap(*this, dg);
        return *this;
    }
    friend void swap(Datagram& lhs, Datagram& rhs);
    ~Datagram() { delete [] buf; }

    // specify the capacity of the datagram ahead of time
    explicit Datagram(size_t capacity) : buf(new uint8_t[capacity]), buf_cap(capacity) {}

    // construct datagram pointing to existing buffer; datagram will cleanup the data
    Datagram(uint8_t *data, sizetag_t length, sizetag_t capacity) :
        buf(data), buf_cap(capacity), buf_offset(length) {}

    // construct datagram copying data from a raw pointer
    Datagram(const uint8_t *data, sizetag_t length) :
        buf(new uint8_t[length]), buf_cap(length), buf_offset(length)
    {
        memcpy(buf, data, length);
    }

    // construct a datagram copying data from a vector<uint8_t>
    explicit Datagram(const std::vector<uint8_t>& data) :
        buf(new uint8_t[data.size()]), buf_cap(data.size()), buf_offset(data.size())
    {
        memcpy(buf, &data[0], data.size());
    }

    // construct a datagram copying data from a string
    explicit Datagram(const std::string& data) :
        buf(new uint8_t[data.length()]),
        buf_cap(data.length()),
        buf_offset(data.length())
    {
        memcpy(buf, data.c_str(), data.length());
    }

    // 8-bit value, either 0x00 (false) or 0x01 (true)
    void add_bool(const bool& v)
    {
        if(v) {
            add_uint8(1);
        } else {
            add_uint8(0);
        }
    }

    // 8-bit ascii character
    void add_char(const char& v)
    {
        check_add_length(1);
        *(char *)(buf + buf_offset) = v;
        buf_offset += 1;
    }

    // Numeric adders always convert to little endian.
    void add_int8(const int8_t& v)
    {
        check_add_length(1);
        *(int8_t *)(buf + buf_offset) = v;
        buf_offset += 1;
    }

    void add_int16(const int16_t& v)
    {
        check_add_length(2);
        *(int16_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 2;
    }

    void add_int32(const int32_t& v)
    {
        check_add_length(4);
        *(int32_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 4;
    }

    void add_int64(const int64_t& v)
    {
        check_add_length(8);
        *(int64_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 8;
    }

    void add_uint8(const uint8_t& v)
    {
        check_add_length(1);
        *(uint8_t *)(buf + buf_offset) = v;
        buf_offset += 1;
    }

    void add_uint16(const uint16_t& v)
    {
        check_add_length(2);
        *(uint16_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 2;
    }

    void add_uint32(const uint32_t& v)
    {
        check_add_length(4);
        *(uint32_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 4;
    }

    void add_uint64(const uint64_t& v)
    {
        check_add_length(8);
        *(uint64_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += 8;
    }

    void add_float32(const float& v)
    {
        check_add_length(4);
        *(float *)(buf + buf_offset) = swap_le(v);
        buf_offset += 4;
    }

    void add_float64(const double& v)
    {
        check_add_length(8);
        *(double *)(buf + buf_offset) = swap_le(v);
        buf_offset += 8;
    }

    // adds a sizetag (either 16-bit or 32-bit depending on compile options)
    void add_size(const sizetag_t& v)
    {
        check_add_length(sizeof(sizetag_t));
        *(sizetag_t *)(buf + buf_offset) = swap_le(v);
        buf_offset += sizeof(sizetag_t);
    }

    // add_data adds raw binary data directly to the end of the datagram
    void add_data(const std::vector<uint8_t>& data)
    {
        if(data.size()) {
            check_add_length(data.size());
            memcpy(buf + buf_offset, &data[0], data.size());
            buf_offset += data.size();
        }
    }

    void add_data(const std::string& str)
    {
        if(str.length()) {
            check_add_length(str.length());
            memcpy(buf + buf_offset, str.c_str(), str.length());
            buf_offset += str.length();
        }
    }

    void add_data(const uint8_t *data, sizetag_t length)
    {
        if(length) {
            check_add_length(length);
            memcpy(buf + buf_offset, data, length);
            buf_offset += length;
        }
    }

    void add_data(const char *data, sizetag_t length)
    {
        if(length) {
            check_add_length(length);
            memcpy(buf + buf_offset, data, length);
            buf_offset += length;
        }
    }
    void add_data(const Datagram& dg)
    {
        if(dg.buf_offset) {
            check_add_length(dg.buf_offset);
            memcpy(buf + buf_offset, dg.buf, dg.buf_offset);
            buf_offset += dg.buf_offset;
        }
    }

    // add_string adds an ascii string to the datagram, prepended with a length tag
    void add_string(const std::string& str)
    {
        add_size(str.length());
        check_add_length(str.length());
        memcpy(buf + buf_offset, str.c_str(), str.length());
        buf_offset += str.length();
    }

    void add_string(const char *str, sizetag_t length)
    {
        add_size(length);
        check_add_length(length);
        memcpy(buf + buf_offset, str, length);
        buf_offset += length;
    }

    // add_blob adds a binary data to the datagram, prepended with a length tag
    void add_blob(const std::vector<uint8_t>& blob)
    {
        add_size(blob.size());
        check_add_length(blob.size());
        memcpy(buf + buf_offset, &blob[0], blob.size());
        buf_offset += blob.size();
    }

    void add_blob(const uint8_t *data, sizetag_t length)
    {
        add_size(length);
        check_add_length(length);
        memcpy(buf + buf_offset, data, length);
        buf_offset += length;
    }

    void add_blob(const char *data, sizetag_t length)
    {
        add_size(length);
        check_add_length(length);
        memcpy(buf + buf_offset, data, length);
        buf_offset += length;
    }

    void add_blob(const Datagram& dg)
    {
        add_size(dg.buf_offset);
        check_add_length(dg.buf_offset);
        memcpy(buf + buf_offset, dg.buf, dg.buf_offset);
        buf_offset += dg.buf_offset;
    }

    // add_buffer reserves a buffer of size "length" at the end of the datagram
    //     and returns a pointer to the buffer so it can be filled manually
    uint8_t *add_buffer(sizetag_t length)
    {
        check_add_length(length);
        uint8_t *buf_start = buf + buf_offset;
        buf_offset += length;
        return buf_start;
    }

    // add_value packs data from a Value in little-endian;
    void add_value(const Value&);
    void add_value(const Value *);

    // add_packed returns the number of bytes read from the buffer, converts to little-endian
    sizetag_t add_packed(const Type *, const std::vector<uint8_t>& packed, sizetag_t offset = 0);

    sizetag_t size() const { return buf_offset; }
    sizetag_t cap() const { return buf_cap; }
    const uint8_t *data() const { return buf; }
};


} // close namespace bamboo
