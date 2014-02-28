// Filename: traits.h
#pragma once
#include <stdint.h>
namespace bamboo { // open namespace bamboo


#ifdef BAMBOO_32BIT_SIZETAG
	typedef uint32_t sizetag_t;
#else
	typedef uint16_t sizetag_t;
#endif

// kSizetagWidth is declared extern and compiled into the library for runtime width-checking.
extern const int kSizetagWidth;
unsigned long long kSizetagMax = sizetag_t(-1);


} // close namespace bamboo
