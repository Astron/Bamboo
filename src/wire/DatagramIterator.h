#pragma once
#include "Datagram.h"
namespace bamboo {  // close namespace bamboo


// A DatagramIteratorEOF is an exception that is thrown when attempting to read
// past the end of a datagram.
class DatagramIteratorEOF : public std::runtime_error {
  public:
    DatagramIteratorEOF(const std::string& what) : std::runtime_error(what) { }
};

// A DatagramIterator lets you step trough a datagram by reading a single value at a time.
class DatagramIterator {
  protected:
    const Datagram* m_dg;
    sizetag_t m_offset;

    DatagramIterator(const Datagram* dg, sizetag_t offset = 0) : m_dg(dg), m_offset(offset) {
        check_read_length(0);
    }

    void check_read_length(sizetag_t length) {
        if(m_offset + length > m_dg->size()) {
            std::stringstream error;
            error << "Datagram iterator tried to read past dg end, offset+length("
                  << m_offset + length << "), buf_size(" << m_dg->size() << ")\n";
            throw DatagramIteratorEOF(error.str());
        };
    }

  public:
    // constructor
    DatagramIterator(const Datagram& dg, sizetag_t offset = 0) : m_dg(&dg), m_offset(offset) {
        check_read_length(0); //shortcuts, yay
    }

    // read_bool reads the next byte from the datagram and returns either false or true.
    bool read_bool() {
        uint8_t val = read_uint8();
        return val != false; // returns either 1 or 0
    }

    // read_char reads a byte from the datagram,
    // returning an 8-bit ascii character.
    char read_char() {
        check_read_length(1);
        char r = *(char *)(m_dg->get_data() + m_offset);
        m_offset += 1;
        return r;
    }

    // read_int8 reads a byte from the datagram,
    // returning a signed 8-bit integer.
    int8_t read_int8()
    {
        check_read_length(1);
        int8_t r = *(int8_t*)(m_dg->get_data() + m_offset);
        m_offset += 1;
        return r;
    }

    // read_int16 reads 2 bytes from the datagram,
    // returning a signed 16-bit integer in native endianness.
    int16_t read_int16()
    {
        check_read_length(2);
        int16_t r = *(int16_t*)(m_dg->get_data() + m_offset);
        m_offset += 2;
        return swap_le(r);
    }

    // read_int32 reads 4 bytes from the datagram,
    // returning a signed 32-bit integer in native endianness.
    int32_t read_int32()
    {
        check_read_length(4);
        int32_t r = *(int32_t*)(m_dg->get_data() + m_offset);
        m_offset += 4;
        return swap_le(r);
    }

    // read_int64 reads 8 bytes from the datagram,
    // returning a signed 64-bit integer in native endianness.
    int64_t read_int64()
    {
        check_read_length(8);
        int64_t r = *(int64_t*)(m_dg->get_data() + m_offset);
        m_offset += 8;
        return swap_le(r);
    }

    // read_uint8 reads a byte from the datagram,
    // returning an unsigned 8-bit integer.
    uint8_t read_uint8()
    {
        check_read_length(1);
        uint8_t r = *(uint8_t*)(m_dg->get_data() + m_offset);
        m_offset += 1;
        return r;
    }

    // read_uint16 reads 2 bytes from the datagram,
    // returning an unsigned 16-bit integer in native endianness.
    uint16_t read_uint16()
    {
        check_read_length(2);
        uint16_t r = *(uint16_t*)(m_dg->get_data() + m_offset);
        m_offset += 2;
        return swap_le(r);
    }

    // read_uint32 reads 4 bytes from the datagram,
    // returning an unsigned 32-bit integer in native endianness.
    uint32_t read_uint32()
    {
        check_read_length(4);
        uint32_t r = *(uint32_t*)(m_dg->get_data() + m_offset);
        m_offset += 4;
        return swap_le(r);
    }

    // read_uint64 reads 8 bytes from the datagram,
    // returning an unsigned 64-bit integer in native endianness.
    uint64_t read_uint64()
    {
        check_read_length(8);
        uint64_t r = *(uint64_t*)(m_dg->get_data() + m_offset);
        m_offset += 8;
        return swap_le(r);
    }

    // read_size reads a sizetag_t from the datagram.
    sizetag_t read_size()
    {
        check_read_length(sizeof(sizetag_t));
        sizetag_t r = *(sizetag_t*)(m_dg->get_data() + m_offset);
        m_offset += sizeof(sizetag_t);
        return swap_le(r);
    }

    // read_float32 reads 4 bytes from the datagram,
    // returning a 32-bit float in native endianness.
    float read_float32() {
        check_read_length(4);
        float r = *(float *)(m_dg->get_data() + m_offset);
        m_offset += 4;
        return swap_le(r);
    }

    // read_float64 reads 8 bytes from the datagram,
    // returning a 64-bit float (double) in native endianness.
    double read_float64() {
        check_read_length(8);
        double r = *(double *)(m_dg->get_data() + m_offset);
        m_offset += 8;
        return swap_le(r);
    }

    // read_string reads a string from the datagram in the format
    //     {sizetag_t length; char[length] characters} and returns the character data.
    std::string read_string() {
        sizetag_t length = read_size();
        check_read_length(length);
        std::string str((char *)(m_dg->get_data() + m_offset), length);
        m_offset += length;
        return str;
    }

    // read_blob reads a blob from the datagram in the format
    //     {sizetag_t length; uint8[length] binary} and returns the binary part.
    std::vector<uint8_t> read_blob() {
        sizetag_t length = read_size();
        return read_data(length);
    }

    // read_datagram reads a blob from the datagram and returns it as another datagram.
    Datagram read_datagram() {
        sizetag_t length = read_size();
        return Datagram(m_dg->get_data() + m_offset, length);
    }

    // read_data returns the next <length> bytes in the datagram.
    std::vector<uint8_t> read_data(sizetag_t length) {
        check_read_length(length);
        std::vector<uint8_t> data(m_dg->get_data() + m_offset, m_dg->get_data() + m_offset + length);
        m_offset += length;
        return data;
    }

    // read_remainder returns a vector containing the rest of the bytes in the datagram.
    std::vector<uint8_t> read_remainder() {
        return read_data(m_dg->size() - m_offset);
    }

    // read_dtype interprets the data as a value for the DistributedType in native endianness.
    TypeData read_dtype(const DistributedType *dtype);
    // Can also provide an external buffer, and get a handle back instead.
    TypeDataHandle read_dtype(const DistributedType *dtype, std::vector<uint8_t>& ref);

    // skip_dtype can be used to seek past the packed data for a DistributedType.
    //     Throws DatagramIteratorEOF if it skips past the end of the datagram.
    void skip_dtype(const DistributedType *dtype);

    // tell returns the current message offset in std::vector<uint8_t>
    sizetag_t tell() const {
        return m_offset;
    }

    // get_remaining returns the number of unread bytes left
    sizetag_t get_remaining() const {
        return m_dg->size() - m_offset;
    }

    // seek sets the current message offset in std::vector<uint8_t>
    void seek(sizetag_t to) {
        m_offset = to;
    }

    // skip increments the current message offset by a length.
    //     Throws DatagramIteratorEOF if it skips past the end of the datagram.
    void skip(sizetag_t length) {
        check_read_length(length);
        m_offset += length;
    }
};


} // close namespace bamboo
