// Filename: parser.h
#pragma once
#include <stdint.h>
#include <string> // std::string
#include <vector> // std::vector
namespace bamboo { // open namespace bamboo


// Forward declarations
class DistributedType;
class Module;

// read_dcfile opens the given file or stream and parses it as a .dc file.  Classes defined in
//     the file are added to a new Module object, and a pointer to that object is returned.
//     When reading from a stream, a filename is optional only used when reporting errors.
Module *read_dcfile(std::istream& in, const std::string& filename);
Module *read_dcfile(const std::string& filename);

// parse_dcfile opens the given file or stream and parses it as a .dc file.  The distributed
//     classes defined in the file are added to the list of classes associated with the Module.
//     When appending from a stream, a filename is optional only used to report errors.
bool parse_dcfile(Module *f, std::istream& in, const std::string& filename);
bool parse_dcfile(Module *f, const std::string& filename);


// parse_value reads a .dc-formatted parameter value and outputs the data in wire form matching
//     the appropriate DistributedType and suitable for a default parameter value.
//     If an error occurs, the error reason is returned instead of the parsed value.
std::vector<uint8_t> parse_dcvalue(const DistributedType *, const std::string& formatted,
                                   bool& err);
std::vector<uint8_t> parse_dcvalue(const DistributedType *, std::istream& in, bool& err);


} // close namespace bamboo
