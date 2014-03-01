// Filename: traits.cpp
#include "sizetag.h"
#include "byteorder.h"
namespace bamboo { // open namespace bamboo


// Compile sizetag width into binary
#ifdef DCLASS_32BIkTypeSIZETAG
const int kSizetagWidth = 32;
#else
const int kSizetagWidth = 16;
#endif
// Compile endianness into binary
#ifdef PLATFORM_BIG_ENDIAN
const bool kBigEndian = true;
#else
const bool kBigEndian = false;
#endif


} // close namespace bamboo
