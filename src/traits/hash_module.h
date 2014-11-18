// Filename: hash_module.h
#pragma once
#include <cstdint> // uint32_t
namespace bamboo
{


class Module;

// property_hash will hash nearly everything in the module, and can be used to
//     verify that all significant properties of two files matches.
uint32_t property_hash(const Module *);

// structural_hash generates a hash which is guaranteed not to change if all of
//     the distributed classes and their fields have the same data arrangement.
//     This hash ignores keywords, typedefs, parameter names, and imports.
uint32_t structural_hash(const Module *);

// legacy_hash produces a hash which matches that of the old dcparser.
uint32_t legacy_hash(const Module *);


} // close namespace bamboo
