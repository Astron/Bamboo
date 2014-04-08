// Filename: Value.cpp
#include "Value.h"
#include <stdexcept>
#include "bits/buffers.h"
#include "module/DistributedType.h"
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Method.h"
#include "module/Field.h"
#include "module/Parameter.h"
#include "values/IntValue.h"
#include "values/FloatValue.h"
#include "values/StringValue.h"
#include "values/BlobValue.h"
#include "values/ArrayValue.h"
#include "values/StructValue.h"
#include "values/MethodValue.h"
using namespace std;
namespace bamboo {


Value Value::from_type(const DistributedType* type) {
    if(type == nullptr) { throw invalid_argument("can't create value for invalid type."); }
    switch(type->get_subtype()) {
        case kTypeChar:
        case kTypeInt8:
        case kTypeInt16:
        case kTypeInt32:
        case kTypeInt64:
        case kTypeUint8:
        case kTypeUint16:
        case kTypeUint32:
        case kTypeUint64: {
            return Value(new IntValue());
        }
        case kTypeFloat32:
        case kTypeFloat64: {
            return Value(new FloatValue());
        }
        case kTypeString:
        case kTypeVarstring: {
            const ArrayType *array = type->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            return Value(new StringValue(len));
        }
        case kTypeBlob:
        case kTypeVarblob: {
            const ArrayType *array = type->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            return Value(new BlobValue(len));
        }
        case kTypeArray:
        case kTypeVararray: {
            const ArrayType *array = type->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            ArrayValue *r = new ArrayValue();
            for(unsigned int i = 0; i < len; ++i) {
                r->append(from_type(array->get_element_type()));
            }
            return Value(r);
        }
        case kTypeStruct: {
            return Value(new StructValue(type->as_struct()));
        }
        case kTypeMethod: {
            return Value(new MethodValue(type->as_method()));
        }
        case kTypeInvalid:
        default: {
            throw invalid_argument("can't create value for invalid type.");
        }
    }
}

Value Value::from_type(const DistributedType* type, bool& is_derived) {
    if(type == nullptr) { throw invalid_argument("can't create value for invalid type."); }

    is_derived = false;
    switch(type->get_subtype()) {
        case kTypeChar:
        case kTypeInt8:
        case kTypeInt16:
        case kTypeInt32:
        case kTypeInt64:
        case kTypeUint8:
        case kTypeUint16:
        case kTypeUint32:
        case kTypeUint64: {
            return Value(new IntValue());
        }
        case kTypeFloat32:
        case kTypeFloat64: {
            return Value(new FloatValue());
        }
        case kTypeString:
        case kTypeVarstring: {
            const ArrayType *array = type->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            return Value(new StringValue(len));
        }
        case kTypeBlob:
        case kTypeVarblob: {
            const ArrayType *array = type->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            return Value(new BlobValue(len));
        }
        case kTypeArray:
        case kTypeVararray: {
            const ArrayType *array = type->as_array();
            sizetag_t len = (sizetag_t)array->get_range().min.uinteger;
            ArrayValue *r = new ArrayValue();
            for(unsigned int i = 0; i < len; ++i) {
                bool from_derived;
                r->append(from_type(array->get_element_type(), from_derived));
                if(from_derived) { is_derived = true; }
            }
            return Value(r);
        }
        case kTypeStruct: {
            const Struct *struct_type = type->as_struct();
            size_t num_fields = struct_type->get_num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                if(struct_type->get_field(i)->has_default_value()) {
                    is_derived = true;
                    break;
                }
            }
            return Value(new StructValue(struct_type));
        }
        case kTypeMethod: {
            const Method *method = type->as_method();
            size_t num_params = method->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                if(method->get_parameter(i)->has_default_value()) {
                    is_derived = true;
                    break;
                }
            }
            return Value(new MethodValue(method));
        }
        case kTypeInvalid:
        default: {
            throw invalid_argument("can't create value for invalid type.");
        }
    }
}

Value Value::from_packed(const DistributedType* type, const vector<uint8_t>& buf) {
    sizetag_t offset = 0;
    return from_packed(type, buf, offset);
}

Value Value::from_packed(const DistributedType* type, const vector<uint8_t>& buf, sizetag_t& offset) {
    if(type == nullptr) { throw invalid_argument("can't create value for invalid type."); }
    switch(type->get_subtype()) {
        case kTypeChar:
        case kTypeInt8:
        case kTypeUint8: {
            if(buf.size() - offset < 1) {
                throw invalid_argument("buffer must contain at least 1 byte for (u)int8/char value.");
            }
            return Value(new IntValue(read_int8(buf, offset)));
        }
        case kTypeInt16:
        case kTypeUint16: {
            if(buf.size() - offset < 2) {
                throw invalid_argument("buffer must contain at least 2 bytes for (u)int16 value.");
            }
            return Value(new IntValue(read_int16(buf, offset)));
        }
        case kTypeInt32:
        case kTypeUint32: {
            if(buf.size() - offset < 4) {
                throw invalid_argument("buffer must contain at least 4 bytes for (u)int32 value.");
            }
            return Value(new IntValue(read_int32(buf, offset)));
        }
        case kTypeInt64:
        case kTypeUint64: {
            if(buf.size() - offset < 8) {
                throw invalid_argument("buffer must contain at least 8 bytes for (u)int64 value.");
            }
            return Value(new IntValue(read_int64(buf, offset)));
        }
        case kTypeFloat32: {
            if(buf.size() - offset < 4) {
                throw invalid_argument("buffer must contain at least 4 bytes for float32 value.");
            }
            return Value(new FloatValue(read_float32(buf, offset)));
        }
        case kTypeFloat64: {
            if(buf.size() - offset < 8) {
                throw invalid_argument("buffer must contain at least 8 bytes for float64 value.");
            }
            return Value(new FloatValue(read_float64(buf, offset)));
        }
        case kTypeString: {
            if(buf.size() - offset < type->get_size()) {
                throw invalid_argument("buffer doesn't match size of fixed-length string.");
            }
            return Value(new StringValue(read_string(buf, type->get_size(), offset)));
        }
        case kTypeVarstring: {
            if(buf.size() - offset < sizeof(sizetag_t)) {
                throw invalid_argument("buffer doesn't have enough bytes to read a string length.");
            }
            sizetag_t len = read_size(buf, offset);

            if(buf.size() - offset < len) {
                throw invalid_argument("buffer doesn't have enough bytes to read remainder of string.");
            }
            return Value(new StringValue(read_string(buf, len, offset)));
        }
        case kTypeBlob: {
            if(buf.size() - offset < type->get_size()) {
                throw invalid_argument("buffer doesn't match size of fixed-length blob.");
            }
            return Value(new BlobValue(read_blob(buf, type->get_size(), offset)));
        }
        case kTypeVarblob: {
            if(buf.size() - offset < sizeof(sizetag_t)) {
                throw invalid_argument("buffer doesn't have enough bytes to read a blob length.");
            }
            sizetag_t len = read_size(buf, offset);

            if(buf.size() - offset < len) {
                throw invalid_argument("buffer doesn't have enough bytes to read remainder of blob.");
            }
            return Value(new BlobValue(read_blob(buf, len, offset)));
        }
        case kTypeArray: {
            if(buf.size() - offset < type->get_size()) {
                throw invalid_argument("buffer doesn't match size of fixed-size array.");
            }

            const ArrayType *array = type->as_array();
            sizetag_t array_end = offset + type->get_size();

            ArrayValue *r = new ArrayValue();
            while(offset < array_end) {
                r->append(from_packed(array->get_element_type(), buf, offset));
            }
            if(offset > array_end) {
                throw invalid_argument("buffer contained array exceeding expected length.");
            }

            return Value(r);
        }
        case kTypeVararray: {
            if(buf.size() - offset < sizeof(sizetag_t)) {
                throw invalid_argument("buffer doesn't have enough bytes to read a string length.");
            }
            sizetag_t len = read_size(buf, offset);

            if(buf.size() - offset < len) {
                throw invalid_argument("buffer doesn't have enough bytes to read remainder of string.");
            }

            const ArrayType *array = type->as_array();
            sizetag_t array_end = offset + len;

            ArrayValue *r = new ArrayValue();
            while(offset < array_end) {
                r->append(from_packed(array->get_element_type(), buf, offset));
            }
            if(offset > array_end) {
                throw invalid_argument("buffer contained array exceeding expected length.");
            }

            return Value(r);
        }
        case kTypeStruct: {
            const Struct *str = type->as_struct();
            StructValue *r = new StructValue(str);
            size_t num_fields = str->get_num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                r->set_item(i, from_packed(str->get_field(i)->get_type(), buf, offset));
            }
            return Value(r);
        }
        case kTypeMethod: {
            const Method* method = type->as_method();
            MethodValue *r = new MethodValue(method);
            size_t num_params = method->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                r->set_item(i, from_packed(method->get_parameter(i)->get_type(), buf, offset));
            }
            return Value(r);
        }
        case kTypeInvalid:
        default: {
            throw invalid_argument("can't create value for invalid type.");
        }
    }
}


} // close namespace bamboo
