// Filename: buffers.h
#pragma once
#include <vector>
#include <string>
#include "../bits/sizetag.h"
namespace bamboo
{


/* Yay for doing all of our heavy-lifting with static-typing! */

inline std::vector<uint8_t> as_buffer(bool r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(char r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int8_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int16_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int32_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int64_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint8_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint16_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint32_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint64_t r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(float r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(double r)
{
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(const std::string& r)
{
    return std::vector<uint8_t>(r.begin(), r.end());
}

inline void pack_value(char r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int8_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int16_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int32_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int64_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint8_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint16_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint32_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint64_t r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(float r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(double r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(const std::string& r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), r.begin(), r.end());
}
inline void pack_value(const std::vector<uint8_t>& r, std::vector<uint8_t>& buf)
{
    buf.insert(buf.end(), r.begin(), r.end());
}

inline bool safe_read_bool(const std::vector<uint8_t>& buf, size_t& offset)
{
    bool r = '\0';
    if(offset + sizeof(r) <= buf.size()) {
        r = bool(buf[offset]);
    }
    offset += sizeof(r);
    return r;
}
inline char safe_read_char(const std::vector<uint8_t>& buf, size_t& offset)
{
    char r = '\0';
    if(offset + sizeof(r) <= buf.size()) {
        r = char(buf[offset]);
    }
    offset += sizeof(r);
    return r;
}
inline int8_t safe_read_int8(const std::vector<uint8_t>& buf, size_t& offset)
{
    int8_t r = '\0';
    if(offset + sizeof(r) <= buf.size()) {
        r = int8_t(buf[offset]);
    }
    offset += sizeof(r);
    return r;
}
inline int16_t safe_read_int16(const std::vector<uint8_t>& buf, size_t& offset)
{
    int16_t r = 0;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(int16_t *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline int32_t safe_read_int32(const std::vector<uint8_t>& buf, size_t& offset)
{
    int32_t r = 0;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(int32_t *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline int64_t safe_read_int64(const std::vector<uint8_t>& buf, size_t& offset)
{
    int64_t r = 0;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(int64_t *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline uint8_t safe_read_uint8(const std::vector<uint8_t>& buf, size_t& offset)
{
    uint8_t r = '\0';
    if(offset + sizeof(r) <= buf.size()) {
        r = buf[offset];
    }
    offset += sizeof(r);
    return r;
}
inline uint16_t safe_read_uint16(const std::vector<uint8_t>& buf, size_t& offset)
{
    uint16_t r = 0;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(uint16_t *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline uint32_t safe_read_uint32(const std::vector<uint8_t>& buf, size_t& offset)
{
    uint32_t r = 0;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(uint32_t *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline uint64_t safe_read_uint64(const std::vector<uint8_t>& buf, size_t& offset)
{
    uint64_t r = 0;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(uint64_t *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline float safe_read_float32(const std::vector<uint8_t>& buf, size_t& offset)
{
    float r = 0.0f;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(float *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline double safe_read_float64(const std::vector<uint8_t>& buf, size_t& offset)
{
    double r = 0.0;
    if(offset + sizeof(r) <= buf.size()) {
        r = *(double *)(&buf[offset]);;
    }
    offset += sizeof(r);
    return r;
}
inline std::string safe_read_string(const std::vector<uint8_t>& buf, size_t n, size_t& offset)
{
    std::string r;
    if(offset + sizeof(r) <= buf.size()) {
        r = std::string((char *)&buf[offset], (char *)&buf[offset] + n);
        offset += n;
    }
    offset += n;
    return r;
}
inline std::vector<uint8_t> safe_read_data(const std::vector<uint8_t>& buf, size_t n, size_t& offset)
{
    std::vector<uint8_t> r;
    if(offset + sizeof(r) <= buf.size()) {
        r = std::vector<uint8_t>(&buf[offset], &buf[offset] + n);
    }
    offset += n;
    return r;
}


} // close namespace bamboo
