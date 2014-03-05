// Filename: buffers.h
#pragma once
#include <vector>
#include <string>
#include "bits/sizetag.h"
namespace bamboo {

/* Yay for doing all of our heavy-lifting with static-typing! */

inline std::vector<uint8_t> as_buffer(bool r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(char r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int8_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int16_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int32_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(int64_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint8_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint16_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint32_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(uint64_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(float r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(double r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline std::vector<uint8_t> as_buffer(const std::string& r) {
	return std::vector<uint8_t>(r.begin(), r.end());
}


inline void pack_value(char r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int8_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int16_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int32_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(int64_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint8_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint16_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint32_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(uint64_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(float r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(double r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
inline void pack_value(const std::string& r, std::vector<uint8_t>& buf) {
	buf.insert(buf.end(), r.begin(), r.end());
}
inline void pack_value(const std::vector<uint8_t>& r, std::vector<uint8_t>& buf) {
	buf.insert(buf.end(), r.begin(), r.end());
}


inline bool read_bool(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    return bool(buf[offset++]) != false;
}
inline char read_char(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    return char(buf[offset++]);
}
inline int8_t read_int8(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    return int8_t(buf[offset++]);
}
inline int16_t read_int16(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    int16_t r = *(int16_t *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}
inline int32_t read_int32(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    int32_t r = *(int32_t *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}
inline int64_t read_int64(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    int64_t r = *(int64_t *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}
inline uint8_t read_uint8(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    return buf[offset++];
}
inline uint16_t read_uint16(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    uint16_t r = *(uint16_t *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}
inline uint32_t read_uint32(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    uint32_t r = *(uint32_t *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}
inline uint64_t read_uint64(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    uint64_t r = *(uint64_t *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}
inline float read_float32(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    float r = *(float *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}
inline double read_float64(const std::vector<uint8_t>& buf, sizetag_t& offset) {
    double r = *(double *)(&buf[offset]);
    offset += sizeof(r);
    return r;
}


} // close namespace bamboo
