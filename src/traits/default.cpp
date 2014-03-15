// Filename: default.cpp
#include "default.h"
#include "bits/buffers.h"
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Field.h"
#include "module/Method.h"
#include "module/Parameter.h"
using namespace std;
namespace bamboo { // open namespace bamboo


static vector<uint8_t> generate_default(const DistributedType *type, bool& is_derived);

// implicit_default returns a sensible default value for the given type (typically 0).
TypeData implicit_default(const DistributedType *type) {
    bool discard;
    return implicit_default(type, discard);
}

// implicit_default returns a sensible default value for the given type (typically 0).
//     is_derived will be true if at least one subvalue is explicit
//     (ie. at least one value was user-defined via set_default_value).
TypeData implicit_default(const DistributedType *type, bool& is_derived) {
    return TypeData(type, generate_default(type, is_derived));
}

static vector<uint8_t> generate_default(const DistributedType *type, bool& is_derived) {
    if(type == nullptr) { return vector<uint8_t>(); }

    // Unless we found a non-implicit value, we are an implicit value
    is_derived = false;
    switch(type->get_subtype()) {
        case kTypeInt8:
            return as_buffer(int8_t(0));
        case kTypeInt16:
            return as_buffer(int16_t(0));
        case kTypeInt32:
            return as_buffer(int32_t(0));
        case kTypeInt64:
            return as_buffer(int64_t(0));
        case kTypeChar:
            return as_buffer('\0');
        case kTypeUint8:
            return as_buffer(uint8_t(0));
        case kTypeUint16:
            return as_buffer(uint16_t(0));
        case kTypeUint32:
            return as_buffer(uint32_t(0));
        case kTypeUint64:
            return as_buffer(uint64_t(0));
        case kTypeFloat32:
            return as_buffer(float(0.0));
        case kTypeFloat64:
            return as_buffer(double(0.0));
        case kTypeArray:
        case kTypeBlob:
        case kTypeString: {
            const ArrayType *array = type->as_array();
            uint64_t len = array->get_range().min.uinteger;
            return vector<uint8_t>(len, 0);
        }
        case kTypeVararray:
        case kTypeVarblob:
        case kTypeVarstring: {
            const ArrayType *array = type->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            vector<uint8_t> buf = as_buffer(len);
            buf.insert(buf.end(), len, 0);
            return buf;
        }
        case kTypeStruct: {
            const Struct *dstruct = type->as_struct();
            size_t num_fields = dstruct->get_num_fields();

            vector<uint8_t> buf;
            for(unsigned int i = 0; i < num_fields; ++i) {
                const Field *field = dstruct->get_field(i);
                if(field->has_default_value()) {
                    is_derived = true;
                }
                pack_value(field->get_default_value().data(), buf);
            }
            return buf;
        }
        case kTypeMethod: {
            const Method *dmethod = type->as_method();
            size_t num_params = dmethod->get_num_parameters();

            vector<uint8_t> buf;
            for(unsigned int i = 0; i < num_params; ++i) {
                const Parameter *param = dmethod->get_parameter(i);
                if(param->has_default_value()) {
                    is_derived = true;
                }
                pack_value(param->get_default_value().data(), buf);
            }
            return buf;
        }
        default: {
            return std::vector<uint8_t>();
        }
    }
}


} // close namespace bamboo
