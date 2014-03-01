// Filename: buffers.h
#pragma once
#include <vector>
#include <string>
#include "sizetag.h"

// static inline uint16_t read_u16(const std::vector<uint8_t>& buf, size_t& offset) {}
// static inline uint32_t read_u32(const std::vector<uint8_t>& buf, size_t& offset) {}


/* Yay for doing all of our heavy-lifting with static-typing! */

static inline std::vector<uint8_t> as_buffer(char r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(int8_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(int16_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(int32_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(int64_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(uint8_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(uint16_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(uint32_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(uint64_t r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(float r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(double r) {
    return std::vector<uint8_t>((uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline std::vector<uint8_t> as_buffer(const std::string& r) {
	return std::vector<uint8_t>(r.begin(), r.end());
}


static inline void pack_value(char r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(int8_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(int16_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(int32_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(int64_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(uint8_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(uint16_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(uint32_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(uint64_t r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(float r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(double r, std::vector<uint8_t>& buf) {
    buf.insert(buf.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
}
static inline void pack_value(const std::string& r, std::vector<uint8_t>& buf) {
	buf.insert(buf.end(), r.begin(), r.end());
}
static inline void pack_value(const std::vector<uint8_t>& r, std::vector<uint8_t>& buf) {
	buf.insert(buf.end(), r.begin(), r.end());
}
