// Filename: IntValue.h
#pragma once
#include "ValueInterface.h"
namespace bamboo {


class IntValue : public ValueInterface {
  private:
    union {
        int64_t m_signed;
        uint64_t m_unsigned;
    };

  public:
    IntValue();
    IntValue(int8_t);
    IntValue(uint8_t);
    IntValue(int16_t);
    IntValue(uint16_t);
    IntValue(int32_t);
    IntValue(uint32_t);
    IntValue(int64_t);
    IntValue(uint64_t);
    virtual ~IntValue() {}

    // pack provides the packed data for the value in native endianness.
    //     Throws: bad_cast
    virtual std::vector<uint8_t> pack(const DistributedType*) const;
    virtual void pack(const DistributedType*, std::vector<uint8_t>& buf) const;

    virtual char as_char() const;
    virtual int8_t as_int8() const;
    virtual int16_t as_int16() const;
    virtual int32_t as_int32() const;
    virtual int64_t as_int64() const;
    virtual uint8_t as_uint8() const;
    virtual uint16_t as_uint16() const;
    virtual uint32_t as_uint32() const;
    virtual uint64_t as_uint64() const;
};


} // close namespace bamboo
