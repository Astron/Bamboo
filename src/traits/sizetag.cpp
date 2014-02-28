// Filename: sizetag.cc
#include "sizetag.h"
namespace bamboo { // open namespace bamboo


#ifdef DCLASS_32BIT_SIZETAG
int sizetag_width = 32;
#else
int sizetag_width = 16;
#endif


} // close namespace bamboo
