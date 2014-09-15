// Filename: token.h
#pragma once
#include <string> // std::string
#include <vector> // std::vector
#include "../module/Type.h"
#include "../module/NumericRange.h"
namespace bamboo   // open namespace bamboo
{


// Foward declarations
class Class;
class Field;
class MolecularField;
class Parameter;

class DCToken
{
  public:
    union {
        int8_t int8;
        int16_t int16;
        int32_t int32;
        int64_t int64;
        uint8_t uint8;
        uint16_t uint16;
        uint32_t uint32;
        uint64_t uint64;
        double real;
        bool flag;
        Subtype subtype;

        Class *dclass;
        Struct *dstruct;
        Field *dfield;
        MolecularField *dmolecule;
        Method *dmethod;
        Parameter *dparam;
        Type *dtype;
        Numeric *dnumeric;
    };

    std::string str;
    std::vector<uint8_t> buffer;
    std::vector<std::string> strings;
    NumericRange range;
    struct NameType {
        std::string name;
        Type *type;
    } nametype;
};


} // close namespace

// The bison-generated code expects to use the symbol 'YYSTYPE' to refer to the above class.
#ifndef DCSTYPE
#define DCSTYPE bamboo::DCToken
#endif
