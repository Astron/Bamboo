// Filename: Value.h
#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include "../bits/errors.h"
#include "Field.h"
#include "Parameter.h"
namespace bamboo   // open namespace bamboo
{


// Foward declarations
class Type;

// A Value is a variant that can represent the value of any Bamboo::Type.
// Value's constructor throws null_error if the type is null or invalid_type if the type is invalid.
struct Value {

    typedef std::map<const Field *, Value, Field::sort_by_id> fields_t;
    typedef std::map<const Parameter *, Value, Parameter::sort_by_position> arguments_t;

    explicit Value(const Type *);
    Value(const Type *, const std::vector<uint8_t>& packed);
    // Must redefine copy/assign/destruct for non-trivial union
    Value(const Value&);
    Value& operator=(Value rhs);
    ~Value();

    const Type *type;
    union {
        char char_;
        int64_t int_;
        uint64_t uint_;
        float float_;
        double double_;
#ifndef _MSC_VER
        // Use C++11 relaxed union for compliant compilers
        std::string string_;
        std::vector<uint8_t> blob_;
        std::vector<Value> elements_;
        fields_t fields_;
        arguments_t arguments_;
    };
#else
    };

    // Have a bloated Value for MSVC
    std::string string_;
    std::vector<uint8_t> blob_;
    std::vector<Value> elements_;
    fields_t fields_;
    arguments_t arguments_;
#endif

    size_t size() const;
    std::vector<uint8_t> pack() const;
    std::vector<uint8_t> pack32() const;
};

void swap(Value&, Value&);

} // close namespace bamboo

