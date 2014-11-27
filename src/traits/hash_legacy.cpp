// Filename: hash_legacy.cpp
#include "hash_module.h"

#include <cmath>    // std::floor
#include <iostream> // std::cerr
#include <set>
#include "../traits/HashGenerator.h"
#include "../module/module"

using namespace std;
namespace bamboo
{


static void hash_module(HashGenerator& hashgen, const Module *file);
static void hash_class(HashGenerator& hashgen, const Class *cls);
static void hash_struct(HashGenerator& hashgen, const Struct *strct);
static void hash_field(HashGenerator& hashgen, const Field *field);
static void hash_parameter(HashGenerator& hashgen, const Parameter *param);
static void hash_keywords(HashGenerator& hashgen, const KeywordList *list);
static void hash_legacy_type(HashGenerator& hashgen, const Type *type);
static void hash_int_type(HashGenerator& hashgen, const Numeric *type);
static void hash_num_type(HashGenerator& hashgen, const Numeric *type);

// legacy_hash produces a hash which matches that of the old dcparser.
uint32_t legacy_hash(const Module *file)
{
    HashGenerator hashgen;
    hash_module(hashgen, file);
    return hashgen.hash();
}

enum LegacyType {
    L_INT8,
    L_INT16,
    L_INT32,
    L_INT64,

    L_UINT8,
    L_UINT16,
    L_UINT32,
    L_UINT64,

    L_FLOAT64,

    L_STRING,
    L_BLOB,

    L_CHAR = 19,

    L_INVALID = 20
};

void hash_module(HashGenerator& hashgen, const Module *file)
{
    hashgen.add_int(1); // (dc_virtual_inheritance && dc_sort_inheritance_by_file)
    hashgen.add_int(int(file->num_structs() + file->num_classes()));

    size_t num_types = file->num_ids();
    for(unsigned int i = 0; i < num_types; ++i) {
        const Type *type = file->type_by_id(i);
        if(!type->as_struct()) {
            cerr << "Cannot generate legacy hash for this file.\n";
            return;
        }

        const Struct *strct = type->as_struct();
        if(strct->as_class()) {
            hash_class(hashgen, strct->as_class());
        } else {
            hash_struct(hashgen, strct);
        }
    }
}

void hash_class(HashGenerator& hashgen, const Class *cls)
{
    hashgen.add_string(cls->name());

    size_t num_parents = cls->num_parents();
    hashgen.add_int(int(num_parents));
    for(unsigned int i = 0; i < num_parents; ++i) {
        hashgen.add_int(cls->nth_parent(i)->id());
    }

    size_t num_fields = cls->num_declared_fields();
    hashgen.add_int(int(num_fields));
    for(unsigned int i = 0; i < num_fields; ++i) {
        hash_field(hashgen, cls->nth_declared_field(i));
    }
}

void hash_struct(HashGenerator& hashgen, const Struct *strct)
{
    hashgen.add_string(strct->name());
    hashgen.add_int(1); // is_struct()
    hashgen.add_int(0); // num_parents()

    size_t num_fields = strct->num_fields();
    hashgen.add_int(int(num_fields));
    for(unsigned int i = 0; i < num_fields; ++i) {
        hash_field(hashgen, strct->nth_field(i));
    }
}

void hash_field(HashGenerator& hashgen, const Field *field)
{
    /* Handle DCMolecularField */
    if(field->as_molecular()) {
        // DCField::generate_hash()
        hashgen.add_string(field->name());
        hashgen.add_int(field->id());

        const MolecularField *mol = field->as_molecular();
        size_t num_fields = mol->num_atomics();

        hashgen.add_int(int(num_fields)); // _fields.size();
        for(unsigned int i = 0; i < num_fields; ++i) {
            hash_field(hashgen, mol->nth_atomic(i));
        }
        return;
    }


    /* Handle DCAtomicField */
    if(field->type()->subtype() == Subtype_Method) {
        // DCField::generate_hash()
        hashgen.add_string(field->name());
        hashgen.add_int(field->id());

        const Method *method = field->type()->as_method();
        size_t num_params = method->num_params();

        hashgen.add_int(int(num_params)); // _elements.size();
        for(unsigned int i = 0; i < num_params; ++i) {
            hash_parameter(hashgen, method->nth_param(i));
        }

        // DCKeywordList::generate_hash()
        hash_keywords(hashgen, field);
        return;
    }


    /* Handle DCSimpleParameter, DCClassParameter, DCArrayParameter */
    // DCParameter::generate_hash()
    if(field->num_keywords() != 0) {
        // DCKeywordList::generate_hash()
        hash_keywords(hashgen, field);
    }
    hash_legacy_type(hashgen, field->type());
}

void hash_parameter(HashGenerator& hashgen, const Parameter *param)
{
    hash_legacy_type(hashgen, param->type());
}

void hash_keywords(HashGenerator& hashgen, const KeywordList *list)
{
    struct LegacyKeyword {
        const char *keyword;
        int flag;
    };
    static LegacyKeyword legacy_keywords[] = {
        { "required", 0x0001 },
        { "broadcast", 0x0002 },
        { "ownrecv", 0x0004 },
        { "ram", 0x0008 },
        { "db", 0x0010 },
        { "clsend", 0x0020 },
        { "clrecv", 0x0040 },
        { "ownsend", 0x0080 },
        { "airecv", 0x0100 },
        { nullptr, 0 }
    };

    size_t num_keywords = list->num_keywords();

    int flags = 0;
    for(unsigned int i = 0; i < num_keywords; ++i) {
        bool set_flag = false;
        string keyword = list->nth_keyword(i);
        for(unsigned int j = 0; j < sizeof(legacy_keywords); ++j) {
            if(keyword == legacy_keywords[j].keyword) {
                flags |= legacy_keywords[j].flag;
                set_flag = true;
                break;
            }
        }

        if(!set_flag) {
            flags = ~0;
            break;
        }
    }

    if(flags != ~0) {
        hashgen.add_int(flags);
    } else {
        hashgen.add_int(int(num_keywords)); // _keywords_by_name.size()

        set<string> keywords_by_name;
        for(unsigned int i = 0; i < num_keywords; ++i) {
            keywords_by_name.insert(list->nth_keyword(i));
        }

        for(auto it = keywords_by_name.begin(); it != keywords_by_name.end(); ++it) {
            // keyword->generate_hash();
            hashgen.add_string(*it);
        }
    }
}

void hash_legacy_type(HashGenerator& hashgen, const Type *type)
{
    const TypeAlias *aliased = type->as_aliased();
    if(aliased != nullptr) {
        if(aliased->alias() == "char") {
            hashgen.add_int(L_CHAR);
            hash_int_type(hashgen, type->as_numeric());
            return;
        } else if(aliased->alias() == "byte") {
            cerr << "Warning: byte ignored in legacy_hash.\n";
            return;
        }
    }

    switch(type->subtype()) {
    case Subtype_Struct:
        {
            // get_class()->generate_hash()
            const Struct *strct = type->as_struct();
            if(strct->as_class()) {
                hash_class(hashgen, strct->as_class());
            } else {
                hash_struct(hashgen, strct);
            }
        }
        break;
    case Subtype_Array:
        {
            const Array *arr = type->as_array();

            // _element_type->generate_hash()
            hash_legacy_type(hashgen, arr->element_type());

            // _array_size_range.generate_hash()
            if(arr->has_range()) {
                NumericRange rng = arr->range();
                hashgen.add_int(1); // _range._ranges.size();
                hashgen.add_int((int)rng.min.uinteger);
                hashgen.add_int((int)rng.max.uinteger);
            }
        }
        break;
    case Subtype_Blob:
        {
            hashgen.add_int(L_BLOB); // _type
            hashgen.add_int(1); // _divisor

            const Array *blob = type->as_array();
            if(blob->has_range()) {
                // _uint_range.generate_hash();
                NumericRange rng = blob->range();
                hashgen.add_int(1); // _range._ranges.size();
                hashgen.add_int((int)rng.min.uinteger);
                hashgen.add_int((int)rng.max.uinteger);
            }
        }
        break;
    case Subtype_String:
        {
            hashgen.add_int(L_STRING); // _type
            hashgen.add_int(1); // _divisor

            const Array *str = type->as_array();
            if(str->has_range()) {
                // _uint_range.generate_hash());
                NumericRange rng = str->range();
                hashgen.add_int(1u); // _range._ranges.size();
                hashgen.add_int((int)rng.min.uinteger);
                hashgen.add_int((int)rng.max.uinteger);
            }
        }
        break;
    case Subtype_Numeric:
        hash_num_type(hashgen, type->as_numeric());
        break;
    case Subtype_None:
        cerr << "Warning: none type ignored in legacy_hash.\n";
        break;
    case Subtype_Method:
        cerr << "Warning: unbound method ignored in legacy_hash.\n";
        break;
    }
}

void hash_num_type(HashGenerator& hashgen, const Numeric *numeric)
{
    switch(numeric->packtype()) {
    case Numeric_Int8:
        hashgen.add_int(L_INT8);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Int16:
        hashgen.add_int(L_INT16);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Int32:
        hashgen.add_int(L_INT32);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Int64:
        hashgen.add_int(L_INT64);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Uint8:
        hashgen.add_int(L_UINT8);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Uint16:
        hashgen.add_int(L_UINT16);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Uint32:
        hashgen.add_int(L_UINT32);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Uint64:
        hashgen.add_int(L_UINT64);
        hash_int_type(hashgen, numeric);
        break;
    case Numeric_Float64:
        {
            hashgen.add_int(L_FLOAT64); // _type
            hashgen.add_int(numeric->divisor()); // _divisor
            if(numeric->has_modulus()) {
                hashgen.add_int(int(numeric->modulus() * numeric->divisor())); // _modulus
            }
            if(numeric->has_range()) {
                // _double_range.generate_hash());
                NumericRange rng = numeric->range();
                hashgen.add_int(1u); // _range._ranges.size();
                hashgen.add_int(int(rng.min.floating * numeric->divisor()));
                hashgen.add_int(int(rng.max.floating * numeric->divisor()));
            }
        }
        break;
    case Numeric_Float32:
        cerr << "Warning: float32 ignored in legacy_hash.\n";
        break;
    case Numeric_Invalid:
        cerr << "Warning: invalid numeric ignored in legacy_hash.\n";
        break;
    }
}

void hash_int_type(HashGenerator& hashgen, const Numeric *numeric)
{
    hashgen.add_int(numeric->divisor());
    if(numeric->has_modulus()) {
        unsigned int modulus = (unsigned int)floor(numeric->modulus() * numeric->divisor() + 0.5);
        hashgen.add_int(int(modulus));
    }
    if(numeric->has_range()) {
        // _uint_range.generate_hash());
        NumericRange rng = numeric->range();
        hashgen.add_int(1u); // _range._ranges.size();
        hashgen.add_int(int(floor(rng.min.floating * numeric->divisor() + 0.5)));
        hashgen.add_int(int(floor(rng.max.floating * numeric->divisor() + 0.5)));
    }
}


} // close namespace bamboo
