// Filename: default.h
#include "module/TypeData.h"
namespace bamboo { // open namespace bamboo


// Forward declarations
class DistributedType;

// implicit_default returns a sensible default value for the given type (typically 0).
TypeData implicit_default(const DistributedType *dtype);
// implicit_default returns a sensible default value for the given type (typically 0).
//     is_derived will be true if at least one subvalue is explicit
//     (ie. at least one value was user-defined via set_default_value).
TypeData implicit_default(const DistributedType *dtype, bool& is_derived);


} // close namespace bamboo
