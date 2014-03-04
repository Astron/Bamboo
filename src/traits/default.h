// Filename: default.h
#include <stdint.h>
#include <vector>
namespace bamboo { // open namespace bamboo


// Forward declarations
class DistributedType;

// create_default_value returns a sensible default value for the given type (typically 0).
std::vector<uint8_t> create_default_value(const DistributedType *dtype);
// create_default_value returns a sensible default value for the given type (typically 0).
//     is_implicit will be true if all component values are implicit,
//     or false otherwise (ie. at least one value was user-defined via set_default_value).
std::vector<uint8_t> create_default_value(const DistributedType *dtype, bool& is_implicit);


} // close namespace bamboo
