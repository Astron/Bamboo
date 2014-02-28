// Filename: read.cpp
#include <fstream> // std::ifstream
#include "module/Module.h"
#include "parserDefs.h"

#include "read.h"
using namespace std;
namespace dclass { // open namespace dclass


// append opens the given file or stream and parses it as a .dc file.  The distributed
//     classes defined in the file are added to the list of classes associated with the Module.
//     When appending from a stream, a filename is optional only used to report errors.
bool append(Module *m, istream& in, const string& filename) {
    init_file_parser(in, filename, *m);
    run_parser();
    cleanup_parser();
    return (parser_error_count() == 0);
}
bool append(Module *m, const string& filename) {
    ifstream in;
    in.open(filename.c_str());
    if(!in) {
        cerr << "Cannot open " << filename << " for reading.\n";
        return false;
    }
    return append(m, in, filename);
}

// read opens the given file or stream and parses it as a .dc file.  Classes defined in
//     the file are added to a new Module object, and a pointer to that object is returned.
//     When reading from a stream, a filename is optional only used when reporting errors.
Module *read(istream& in, const string& filename) {
    Module *m = new Module();
    bool ok = append(m, in, filename);
    if(ok) {
        return m;
    }

    return NULL;
}
Module *read(const string& filename) {
    Module *m = new Module();
    bool ok = append(m, filename);
    if(ok) {
        return m;
    }

    return NULL;
}


} // close namespace dclass
