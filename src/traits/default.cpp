// Filename: default.cpp
#include <stdint.h> // fixed-width integer types
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Field.h"
#include "module/Method.h"
#include "module/Parameter.h"

#include "default.h"
using namespace std;
namespace bamboo { // open namespace bamboo


// create_default_value returns a sensible default value for the given type (typically 0).
string create_default_value(const DistributedType *dtype) {
    bool discard;
    return create_default_value(dtype, discard);
}

// create_default_value returns a sensible default value for the given type (typically 0).
//     is_implicit will be true if all component values are implicit,
//     or false otherwise (ie. at least one value was user-defined via set_default_value).
string create_default_value(const DistributedType *dtype, bool& is_implicit) {
    if(dtype == NULL) {
        return "";
    }

    // Unless we found a non-implicit value, we are an implicit value
    is_implicit = true;
    switch(dtype->get_subtype()) {
        case kTypeInt8: {
            int8_t v = 0;
            return string((char *)&v, sizeof(int8_t));
        }
        case kTypeInt16: {
            uint16_t v = 0;
            return string((char *)&v, sizeof(int16_t));
        }
        case kTypeInt32: {
            int32_t v = 0;
            return string((char *)&v, sizeof(int32_t));
        }
        case kTypeInt64: {
            int64_t v = 0;
            return string((char *)&v, sizeof(int64_t));
        }
        case kTypeChar:
        case kTypeUint8: {
            uint8_t v = 0;
            return string((char *)&v, sizeof(uint8_t));
        }
        case kTypeUint16: {
            uint16_t v = 0;
            return string((char *)&v, sizeof(uint16_t));
        }
        case kTypeUint32: {
            uint32_t v = 0;
            return string((char *)&v, sizeof(uint32_t));
        }
        case kTypeUint64: {
            uint64_t v = 0;
            return string((char *)&v, sizeof(uint64_t));
        }
        case kTypeFloat32: {
            float v = 0.0f;
            return string((char *)&v, sizeof(float));
        }
        case kTypeFloat64: {
            double v = 0.0;
            return string((char *)&v, sizeof(double));
        }
        case kTypeArray:
        case kTypeBlob:
        case kTypeString: {
            const ArrayType *array = dtype->as_array();
            uint64_t min_array_elements = array->get_range().min.uinteger;
            return string(min_array_elements, '\0');
        }
        case kTypeVararray:
        case kTypeVarblob:
        case kTypeVarstring: {
            const ArrayType *array = dtype->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            return string((char *)&len, sizeof(sizetag_t)) + string(len, '\0');
        }
        case kTypeStruct: {
            const Struct *dstruct = dtype->as_struct();
            size_t num_fields = dstruct->get_num_fields();

            string val;
            for(unsigned int i = 0; i < num_fields; ++i) {
                const Field *field = dstruct->get_field(i);
                if(field->has_default_value()) {
                    is_implicit = false;
                }
                val += field->get_default_value();
            }
            return val;
        }
        case kTypeMethod: {
            const Method *dmethod = dtype->as_method();
            size_t num_params = dmethod->get_num_parameters();

            string val;
            for(unsigned int i = 0; i < num_params; ++i) {
                const Parameter *param = dmethod->get_parameter(i);
                if(param->has_default_value()) {
                    is_implicit = false;
                }
                val += param->get_default_value();
            }
            return val;
        }
        default: {
            return string();
        }
    }
}


} // close namespace bamboo
