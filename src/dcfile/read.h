// Filename: read.h
#pragma once
#include <iostream> // std::istream
#include <string>   // std::string
namespace dclass { // open namespace dclass


// Foward declarations
class Module;

// append opens the given file or stream and parses it as a .dc file.  The distributed
//     classes defined in the file are added to the list of classes associated with the Module.
//     When appending from a stream, a filename is optional only used to report errors.
bool append(Module *f, std::istream& in, const std::string& filename);
bool append(Module *f, const std::string& filename);

// read opens the given file or stream and parses it as a .dc file.  Classes defined in
//     the file are added to a new Module object, and a pointer to that object is returned.
//     When reading from a stream, a filename is optional only used when reporting errors.
Module *read(std::istream& in, const std::string& filename);
Module *read(const std::string& filename);


} // close namespace dclass
