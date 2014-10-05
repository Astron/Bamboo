// Filename: Value.cpp
#include "../bits/buffers.h"
#include "Value.h"
#include "Array.h"
#include "Struct.h"
#include "Method.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{


Value::Value(const Type *type) : type(type)
{
    if(type == nullptr) throw null_error("is not a valid Type");
    switch(type->subtype()) {
    case kTypeInt8:
    case kTypeInt16:
    case kTypeInt32:
    case kTypeInt64:
        int_ = 0;
        break;
    case kTypeUint8:
    case kTypeUint16:
    case kTypeUint32:
    case kTypeUint64:
        uint_ = 0;
        break;
    case kTypeChar:
        char_ = '\0';
        break;
    case kTypeFloat32:
        float_ = 0;
        break;
    case kTypeFloat64:
        double_ = 0;
        break;
    case kTypeString:
    case kTypeVarstring:
        string_ = string(size_t(type->as_array()->range().min.uinteger), '\0');
        break;
    case kTypeBlob:
    case kTypeVarblob:
        blob_ = vector<uint8_t>(size_t(type->as_array()->range().min.uinteger), uint8_t(0));
        break;
    case kTypeArray:
    case kTypeVararray: {
        const Array *array = type->as_array();
        elements_ = vector<Value>(size_t(array->range().min.uinteger), Value(array->element_type()));
        break;
    }
    case kTypeStruct: {
        fields_ = fields_t();

        // Construct the struct from default values for its fields
        const Struct *record = type->as_struct();
        size_t num_fields = record->num_fields();
        for(size_t i = 0; i < num_fields; ++i) {
            const Field *field = record->get_field((unsigned int)i);
            if(field->has_default_value()) {
                // Get default value for field
                fields_.emplace(field, *field->default_value());
            } else {
                // Otherwise, use the implicit default (0, empty, etc...)
                fields_.emplace(field, Value(field->type()));
            }
        }
        break;
    }
    case kTypeMethod: {
        arguments_ = arguments_t();

        // Construct the method-call from default values for its parameters
        const Method *method = type->as_method();
        size_t num_params = method->num_parameters();
        for(unsigned int i = 0; i < num_params; ++i) {
            const Parameter *param = method->get_parameter(i);
            if(param->has_default_value()) {
                // Get default value for field
                arguments_.emplace(param, *param->default_value());
            } else {
                // Otherwise, use the implicit default (0, empty, etc...)
                arguments_.emplace(param, Value(param->type()));
            }
        }
        break;
    }
    case kTypeInvalid:
        throw invalid_type("type has invalid subtype");
        break;
    }
}

Value::Value(const Type *type, const vector<uint8_t>&) : type(type)
{
    // TODO: Implement
    if(type == nullptr) throw null_error("is not a valid Type");
    switch(type->subtype()) {
    case kTypeInt8:
    case kTypeInt16:
    case kTypeInt32:
    case kTypeInt64:
    case kTypeUint8:
    case kTypeUint16:
    case kTypeUint32:
    case kTypeUint64:
    case kTypeChar:
    case kTypeFloat32:
    case kTypeFloat64:
    case kTypeString:
    case kTypeVarstring:
    case kTypeBlob:
    case kTypeVarblob:
    case kTypeArray:
    case kTypeVararray:
    case kTypeStruct:
    case kTypeMethod:
        break;
    case kTypeInvalid:
        throw invalid_type("type has invalid subtype");
        break;
    }
}

Value::Value(const Value& other) : type(other.type)
{
    switch(type->subtype()) {
    case kTypeInt8:
    case kTypeInt16:
    case kTypeInt32:
    case kTypeInt64:
        int_ = other.int_;
        break;
    case kTypeChar:
    case kTypeUint8:
    case kTypeUint16:
    case kTypeUint32:
    case kTypeUint64:
        uint_ = other.uint_;
        break;
    case kTypeFloat32:
        float_ = other.float_;
        break;
    case kTypeFloat64:
        double_ = other.double_;
        break;
    case kTypeString:
    case kTypeVarstring:
        string_ = other.string_;
        break;
    case kTypeBlob:
    case kTypeVarblob:
        blob_ = other.blob_;
        break;
    case kTypeArray:
    case kTypeVararray:
        elements_ = other.elements_;
        break;
    case kTypeStruct:
        fields_ = other.fields_;
        break;
    case kTypeMethod:
        arguments_ = other.arguments_;
        break;
    case kTypeInvalid:
        break;
    }
}

Value& Value::operator=(Value rhs)
{
    swap(*this, rhs);
    return *this;
}

Value::~Value()
{
    switch(type->subtype()) {
    case kTypeString:
    case kTypeVarstring:
        string_.~string();
        break;
    case kTypeBlob:
    case kTypeVarblob:
        blob_.~vector<uint8_t>();
        break;
    case kTypeArray:
    case kTypeVararray:
        elements_.~vector<Value>();
        break;
    case kTypeStruct:
        fields_.~fields_t();
        break;
    case kTypeMethod:
        arguments_.~arguments_t();
        break;
    default:
        break;
    }
}

size_t Value::size() const
{
    switch(type->subtype()) {
    case kTypeString:
    case kTypeVarstring:
        return string_.size();
    case kTypeBlob:
    case kTypeVarblob:
        return blob_.size();
    case kTypeArray:
    case kTypeVararray:
        return elements_.size();
    case kTypeStruct:
        return fields_.size();
    case kTypeMethod:
        return arguments_.size();
    default:
        return 0;
    }
};

vector<uint8_t> Value::pack() const
{
    switch(type->subtype()) {
    case kTypeInt8:
        return as_buffer(int8_t(int_));
    case kTypeInt16:
        return as_buffer(int16_t(int_));
    case kTypeInt32:
        return as_buffer(int32_t(int_));
    case kTypeInt64:
        return as_buffer(int64_t(int_));
    case kTypeUint8:
        return as_buffer(uint8_t(uint_));
    case kTypeUint16:
        return as_buffer(uint16_t(uint_));
    case kTypeUint32:
        return as_buffer(uint32_t(uint_));
    case kTypeUint64:
        return as_buffer(uint64_t(uint_));
    case kTypeChar:
        return as_buffer(char_);
    case kTypeFloat32:
        return as_buffer(float_);
    case kTypeFloat64:
        return as_buffer(double_);
    case kTypeString:
        return as_buffer(string_);
    case kTypeVarstring: {
        vector<uint8_t> packed = as_buffer(uint16_t(string_.size()));
        pack_value(as_buffer(string_), packed);
        return packed;
    }
    case kTypeBlob:
        return blob_;
    case kTypeVarblob: {
        vector<uint8_t> packed = as_buffer(uint16_t(string_.size()));
        pack_value(blob_, packed);
        return packed;
    }
    case kTypeArray: {
        vector<uint8_t> packed;
        for(const Value& element : elements_) {
            pack_value(element.pack(), packed);
        }
        return packed;
    }
    case kTypeVararray: {
        // Reserve a 16-bit unsigned integer
        vector<uint8_t> packed = as_buffer(uint16_t(0));
        for(const Value& element : elements_) {
            pack_value(element.pack(), packed);
        }
        // Set the size using the reserved bytes
        uint16_t *size_ptr = (uint16_t *)&packed[0];
        *size_ptr = uint16_t(packed.size() - sizeof(uint16_t));
        return packed;
    }
    case kTypeStruct: {
        const Struct *record = type->as_struct();
        size_t num_fields = record->num_fields();
        vector<uint8_t> packed;
        for(unsigned int i = 0; i < num_fields; ++i) {
            pack_value(fields_.at(record->get_field(i)).pack(), packed);
        }
        return packed;
    }
    case kTypeMethod: {
        const Method *method = type->as_method();
        size_t num_params = method->num_parameters();
        vector<uint8_t> packed;
        for(unsigned int i = 0; i < num_params; ++i) {
            pack_value(arguments_.at(method->get_parameter(i)).pack(), packed);
        }
        return packed;
    }
    case kTypeInvalid:
        break;
    }

    return vector<uint8_t>();
};

vector<uint8_t> Value::pack32() const
{
    switch(type->subtype()) {
    case kTypeVarstring: {
        vector<uint8_t> packed = as_buffer(uint32_t(string_.size()));
        pack_value(as_buffer(string_), packed);
        return packed;
    }
    case kTypeVarblob: {
        vector<uint8_t> packed = as_buffer(uint32_t(string_.size()));
        pack_value(blob_, packed);
        return packed;
    }
    case kTypeArray: {
        vector<uint8_t> packed;
        for(const Value& element : elements_) {
            pack_value(element.pack32(), packed);
        }
        return packed;
    }
    case kTypeVararray: {
        // Reserve a 32-bit unsigned integer
        vector<uint8_t> packed = as_buffer(uint32_t(0));
        for(const Value& element : elements_) {
            pack_value(element.pack32(), packed);
        }
        // Set the size using the reserved bytes
        uint32_t *size_ptr = (uint32_t *)&packed[0];
        *size_ptr = uint32_t(packed.size() - sizeof(uint32_t));
        return packed;
    }
    case kTypeStruct: {
        const Struct *record = type->as_struct();
        size_t num_fields = record->num_fields();
        vector<uint8_t> packed;
        for(unsigned int i = 0; i < num_fields; ++i) {
            pack_value(fields_.at(record->get_field(i)).pack32(), packed);
        }
        return packed;
    }
    case kTypeMethod: {
        const Method *method = type->as_method();
        size_t num_params = method->num_parameters();
        vector<uint8_t> packed;
        for(unsigned int i = 0; i < num_params; ++i) {
            pack_value(arguments_.at(method->get_parameter(i)).pack32(), packed);
        }
        return packed;
    }
    default:
        return pack();
    }
};

void swap(Value& lhs, Value& rhs)
{
    using std::swap;

    swap(lhs.type, rhs.type);
    switch(lhs.type->subtype()) {
    case kTypeInt8:
    case kTypeInt16:
    case kTypeInt32:
    case kTypeInt64:
        swap(lhs.int_, rhs.int_);
        break;
    case kTypeChar:
    case kTypeUint8:
    case kTypeUint16:
    case kTypeUint32:
    case kTypeUint64:
        swap(lhs.uint_, rhs.uint_);
        break;
    case kTypeFloat32:
        swap(lhs.float_, rhs.float_);
        break;
    case kTypeFloat64:
        swap(lhs.double_, rhs.double_);
        break;
    case kTypeString:
    case kTypeVarstring:
        swap(lhs.string_, rhs.string_);
        break;
    case kTypeBlob:
    case kTypeVarblob:
        swap(lhs.blob_, rhs.blob_);
        break;
    case kTypeArray:
    case kTypeVararray:
        swap(lhs.elements_, rhs.elements_);
        break;
    case kTypeStruct:
        swap(lhs.fields_, rhs.fields_);
        break;
    case kTypeMethod:
        swap(lhs.arguments_, rhs.arguments_);
        break;
    case kTypeInvalid:
        break;
    }
}



} // close namespace bamboo

