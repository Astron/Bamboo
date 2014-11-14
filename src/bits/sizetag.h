// Filename: sizetag.h
#pragma once
#include <cstdint>
namespace bamboo
{


#ifdef BAMBOO_32BIT_SIZETAG
typedef uint32_t sizetag_t;
#else
typedef uint16_t sizetag_t;
#endif

extern const int kSizetagWidth;
extern const unsigned long long kSizetagMax;


} // close namespace bamboo
