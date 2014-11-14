#pragma once
#include "Datagram.h"
namespace bamboo
{


// A DatagramIteratorEOF is an exception that is thrown when attempting to read
// past the end of a datagram.
class DatagramIteratorEOF : public std::runtime_error
{
  public:
    DatagramIteratorEOF(const std::string& what) : std::runtime_error(what) { }
};

// A DatagramIterator lets you step trough a datagram by reading a single value at a time.
class DatagramIterator
{
  protected:
    const Datagram *m_dg;
    sizetag_t m_offset;

    DatagramIterator(const Datagram *dg, sizetag_t offset = 0) : m_dg(dg), m_offset(offset)
    {
        check_read_length(0);
    }

    void check_read_length(sizetag_t length)
    {
        if(m_offset + length > m_dg->size()) {
            std::stringstream error;
            error << "Datagram iterator tried to read past dg end, offset+length("
                  << m_offset + length << "), buf_size(" << m_dg->size() << ")\n";
            throw DatagramIteratorEOF(error.str());
        };
    }

  public:
    DatagramIterator(const Datagram& dg, sizetag_t offset = 0) : m_dg(&dg), m_offset(offset)
    {
        check_read_length(0); //shortcuts, yay
    }

    bool read_bool()
    {
        uint8_t val = read_uint8();
        return val != false; // returns either 1 or 0
    }

    char read_char()
    {
        check_read_length(1);
        char r = *(char *)(m_dg->data() + m_offset);
        m_offset += 1;
        return r;
    }


    // Numeric readers always convert to native-endiannes
    int8_t read_int8()
    {
        check_read_length(1);
        int8_t r = *(int8_t *)(m_dg->data() + m_offset);
        m_offset += 1;
        return r;
    }

    int16_t read_int16()
    {
        check_read_length(2);
        int16_t r = *(int16_t *)(m_dg->data() + m_offset);
        m_offset += 2;
        return swap_le(r);
    }

    int32_t read_int32()
    {
        check_read_length(4);
        int32_t r = *(int32_t *)(m_dg->data() + m_offset);
        m_offset += 4;
        return swap_le(r);
    }

    int64_t read_int64()
    {
        check_read_length(8);
        int64_t r = *(int64_t *)(m_dg->data() + m_offset);
        m_offset += 8;
        return swap_le(r);
    }

    uint8_t read_uint8()
    {
        check_read_length(1);
        uint8_t r = *(uint8_t *)(m_dg->data() + m_offset);
        m_offset += 1;
        return r;
    }

    uint16_t read_uint16()
    {
        check_read_length(2);
        uint16_t r = *(uint16_t *)(m_dg->data() + m_offset);
        m_offset += 2;
        return swap_le(r);
    }

    uint32_t read_uint32()
    {
        check_read_length(4);
        uint32_t r = *(uint32_t *)(m_dg->data() + m_offset);
        m_offset += 4;
        return swap_le(r);
    }

    uint64_t read_uint64()
    {
        check_read_length(8);
        uint64_t r = *(uint64_t *)(m_dg->data() + m_offset);
        m_offset += 8;
        return swap_le(r);
    }

    float read_float32()
    {
        check_read_length(4);
        float r = *(float *)(m_dg->data() + m_offset);
        m_offset += 4;
        return swap_le(r);
    }

    double read_float64()
    {
        check_read_length(8);
        double r = *(double *)(m_dg->data() + m_offset);
        m_offset += 8;
        return swap_le(r);
    }

    // reads a sizetag (either 16-bit or 32-bit depending on compile options)
    sizetag_t read_size()
    {
        check_read_length(sizeof(sizetag_t));
        sizetag_t r = *(sizetag_t *)(m_dg->data() + m_offset);
        m_offset += sizeof(sizetag_t);
        return swap_le(r);
    }

    // read_string reads a string from the datagram in the format
    //     {sizetag_t length; char[length] characters} and returns the character data.
    // When given a length, returns the next <length> bytes as a string.
    std::string read_string()
    {
        sizetag_t length = read_size();
        check_read_length(length);
        std::string str((char *)(m_dg->data() + m_offset), length);
        m_offset += length;
        return str;
    }

    std::string read_string(sizetag_t length)
    {
        check_read_length(length);
        std::string str((char *)(m_dg->data() + m_offset), length);
        m_offset += length;
        return str;
    }

    // read_blob reads a blob from the datagram in the format
    //     {sizetag_t length; uint8[length] binary} and returns the binary part.
    // When given a length, returns the next <length> bytes as a blob.
    std::vector<uint8_t> read_blob()
    {
        sizetag_t length = read_size();
        return read_data(length);
    }

    std::vector<uint8_t> read_blob(sizetag_t length)
    {
        return read_data(length);
    }

    // read_datagram reads a var-sized blob from the datagram and returns it as another datagram.
    Datagram read_datagram()
    {
        sizetag_t length = read_size();
        return Datagram(m_dg->data() + m_offset, length);
    }

    std::vector<uint8_t> read_data(sizetag_t length)
    {
        check_read_length(length);
        std::vector<uint8_t> data(m_dg->data() + m_offset, m_dg->data() + m_offset + length);
        m_offset += length;
        return data;
    }

    std::vector<uint8_t> read_remainder()
    {
        return read_data(m_dg->size() - m_offset);
    }

    Value read_value(const Type *);

    // read_packed returns a vector containing the native-endian data corresponding to the type.
    //     Can also endian-swap packed data into a pre-existing buffer.
    std::vector<uint8_t> read_packed(const Type *);
    void read_packed(const Type *, std::vector<uint8_t>&);

    // skip increments the current message offset by a length.
    //     Throws DatagramIteratorEOF if it skips past the end of the datagram.
    void skip(sizetag_t length)
    {
        check_read_length(length);
        m_offset += length;
    }

    // skip_type can be used to seek past the packed data for a Type.
    //     Throws DatagramIteratorEOF if it skips past the end of the datagram.
    void skip_type(const Type *);

    sizetag_t remaining() const
    {
        return m_dg->size() - m_offset;
    }

    sizetag_t tell() const
    {
        return m_offset;
    }

    void seek(sizetag_t to)
    {
        m_offset = to;
    }
};


} // close namespace bamboo
