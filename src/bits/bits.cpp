// Filename: bits.cpp
#include "../bits/sizetag.h"
#include "../bits/byteorder.h"
namespace bamboo   // open namespace bamboo
{


// Compile sizetag width into binary
#ifdef BAMBOO_32BIT_SIZETAG
const int kSizetagWidth = 32;
#else
const int kSizetagWidth = 16;
#endif
const unsigned long long kSizetagMax = sizetag_t(-1);

// Compile endianness into binary
#ifdef PLATFORM_BIG_ENDIAN
const bool kBigEndian = true;
#else
const bool kBigEndian = false;
#endif


} // close namespace bamboo
