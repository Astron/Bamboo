// Filename: Value.cpp
#include "Value.h"
#include "../bits/buffers.h"
#include "../module/Array.h"
#include "../module/Struct.h"
#include "../module/Method.h"
#include "../module/Numeric.h"
#include "../dcfile/parse.h"
#include "../dcfile/format.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{


Value::Value(const Type *type) : m_type(type)
{
    if(m_type == nullptr) { m_type = Type::None; }
    switch(m_type->subtype()) {
    case kTypeChar:
        m_char = '\0';
        break;
    case kTypeInt8:
        m_int8 = 0;
        break;
    case kTypeInt16:
        m_int16 = 0;
        break;
    case kTypeInt32:
        m_int32 = 0;
        break;
    case kTypeInt64:
        m_int64 = 0;
        break;
    case kTypeUint8:
        m_uint8 = 0;
        break;
    case kTypeUint16:
        m_uint16 = 0;
        break;
    case kTypeUint32:
        m_uint32 = 0;
        break;
    case kTypeUint64:
        m_uint64 = 0;
        break;
    case kTypeFloat32:
        m_float32 = 0;
        break;
    case kTypeFloat64:
    case kTypeFixed:
        m_float64 = 0;
        break;
    case kTypeString:
        new (&m_string) string(size_t(m_type->as_array()->range().min.uinteger), '\0');
        break;
    case kTypeBlob:
        new (&m_blob) vector<uint8_t>(size_t(m_type->as_array()->range().min.uinteger), uint8_t(0));
        break;
    case kTypeArray: {
        const Array *array = m_type->as_array();
        new (&m_array) vector<Value>(size_t(array->range().min.uinteger), Value(array->element_type()));
        break;
    }
    case kTypeStruct: {
        new (&m_struct) struct_t();

        // Construct the struct from default values for its fields
        const Struct *container = m_type->as_struct();
        size_t num_fields = container->num_fields();
        for(size_t i = 0; i < num_fields; ++i) {
            const Field *field = container->nth_field((unsigned int)i);
            if(field->has_default_value()) {
                // Get default value for field
                m_struct.emplace(field, *field->default_value());
            } else {
                // Otherwise, use the implicit default (0, empty, etc...)
                m_struct.emplace(field, Value(field->type()));
            }
        }
        break;
    }
    case kTypeMethod: {
        new (&m_method) method_t();

        // Construct the method-call from default values for its parameters
        const Method *method = m_type->as_method();
        size_t num_params = method->num_params();
        for(unsigned int i = 0; i < num_params; ++i) {
            const Parameter *param = method->nth_param(i);
            if(param->has_default_value()) {
                // Get default value for field
                m_method.emplace(param, *param->default_value());
            } else {
                // Otherwise, use the implicit default (0, empty, etc...)
                m_method.emplace(param, Value(param->type()));
            }
        }
        break;
    }
    case kTypeNone:
        break;
    }
}

Value::Value(const Value& other) : m_type(other.m_type)
{
    switch(m_type->subtype()) {
    case kTypeChar:
        m_char = other.m_char;
        break;
    case kTypeInt8:
        m_int8 = other.m_int8;
        break;
    case kTypeInt16:
        m_int16 = other.m_int16;
        break;
    case kTypeInt32:
        m_int32 = other.m_int32;
        break;
    case kTypeInt64:
        m_int64 = other.m_int64;
        break;
    case kTypeUint8:
        m_uint8 = other.m_uint8;
        break;
    case kTypeUint16:
        m_uint16 = other.m_uint16;
        break;
    case kTypeUint32:
        m_uint32 = other.m_uint32;
        break;
    case kTypeUint64:
        m_uint64 = other.m_uint64;
        break;
    case kTypeFloat32:
        m_float32 = other.m_float32;
        break;
    case kTypeFloat64:
    case kTypeFixed:
        m_float64 = other.m_float64;
        break;
    case kTypeString:
        m_string = other.m_string;
        break;
    case kTypeBlob:
        m_blob = other.m_blob;
        break;
    case kTypeArray:
        m_array = other.m_array;
        break;
    case kTypeStruct:
        m_struct = other.m_struct;
        break;
    case kTypeMethod:
        m_method = other.m_method;
        break;
    case kTypeNone:
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
    switch(m_type->subtype()) {
    case kTypeString:
        m_string.~string();
        break;
    case kTypeBlob:
        m_blob.~vector<uint8_t>();
        break;
    case kTypeArray:
        m_array.~vector<Value>();
        break;
    case kTypeStruct:
        m_struct.~struct_t();
        break;
    case kTypeMethod:
        m_method.~method_t();
        break;
    default:
        break;
    }
}

Value Value::parse(const Type *type, const std::string& formatted)
{
    bool err = false;
    Value value = parse_dcvalue(type, formatted, err);
    if(err) { throw InvalidValue("encountered errors parsing formatted value"); }
    return value;
}

Value Value::unpack(const Type *type, const vector<uint8_t>& packed, size_t offset)
{
    Value value;
    value.m_type = type;
    switch(type->subtype()) {
    case kTypeChar:
        value.m_char = safe_read_char(packed, offset);
        break;
    case kTypeInt8:
        value.m_int8 = safe_read_int8(packed, offset);
        break;
    case kTypeInt16:
        value.m_int16 = safe_read_int16(packed, offset);
        break;
    case kTypeInt32:
        value.m_int32 = safe_read_int32(packed, offset);
        break;
    case kTypeInt64:
        value.m_int64 = safe_read_int64(packed, offset);
        break;
    case kTypeUint8:
        value.m_uint8 = safe_read_uint8(packed, offset);
        break;
    case kTypeUint16:
        value.m_uint16 = safe_read_uint16(packed, offset);
        break;
    case kTypeUint32:
        value.m_uint32 = safe_read_uint32(packed, offset);
        break;
    case kTypeUint64:
        value.m_uint64 = safe_read_uint64(packed, offset);
        break;
    case kTypeFloat32:
        value.m_float32 = safe_read_float32(packed, offset);
        break;
    case kTypeFloat64:
        value.m_float64 = safe_read_float64(packed, offset);
        break;
    case kTypeFixed: {
        const Numeric *numeric = type->as_numeric();
        if(numeric->is_signed()) {
            switch(numeric->fixed_size()) {
            case 1:
                value.m_float64 = numeric->to_floating(safe_read_int8(packed, offset));
            case 2:
                value.m_float64 = numeric->to_floating(safe_read_int16(packed, offset));
            case 4:
                value.m_float64 = numeric->to_floating(safe_read_int32(packed, offset));
            case 8:
                value.m_float64 = numeric->to_floating(safe_read_int64(packed, offset));
            default:
                throw InvalidValue("type had unexpected bytesize");
            }
        } else {
            switch(numeric->fixed_size()) {
            case 1:
                value.m_float64 = numeric->to_floating(safe_read_uint8(packed, offset));
            case 2:
                value.m_float64 = numeric->to_floating(safe_read_uint16(packed, offset));
            case 4:
                value.m_float64 = numeric->to_floating(safe_read_uint32(packed, offset));
            case 8:
                value.m_float64 = numeric->to_floating(safe_read_uint64(packed, offset));
            default:
                throw InvalidValue("type had unexpected bytesize");
            }

        }
    }
    case kTypeString: {
        new (&value.m_string) string();
        if(type->has_fixed_size()) {
            value.m_string = safe_read_string(packed, type->fixed_size(), offset);
        } else {
            uint16_t size = safe_read_int16(packed, offset);
            value.m_string = safe_read_string(packed, size, offset);
        }
        break;
    }
    case kTypeBlob: {
        new (&value.m_blob) vector<uint8_t>();
        if(type->has_fixed_size()) {
            value.m_blob = safe_read_data(packed, type->fixed_size(), offset);
        } else {
            uint16_t size = safe_read_int16(packed, offset);
            value.m_blob = safe_read_data(packed, size, offset);
        }
        break;
    }
    case kTypeArray: {
        new (&value.m_array) array_t();

        uint16_t size = 0;
        if(type->has_fixed_size()) {
            size = type->fixed_size();
        } else {
            size = safe_read_int16(packed, offset);
        }

        size_t array_end = offset + size;
        const Array *array = type->as_array();
        while(offset < array_end) {
            value.m_array.push_back(unpack(array->element_type(), packed, offset));
        }
        if(offset > array_end) { throw InvalidValue("array values exceeded array size"); }
    }
    case kTypeStruct: {
        new (&value.m_struct) struct_t();

        const Struct *struct_ = type->as_struct();
        size_t num_fields = struct_->num_fields();
        for(unsigned int i = 0; i < num_fields; ++i) {
            const Field *field = struct_->nth_field(i);
            value.m_struct.emplace(field, unpack(field->type(), packed, offset));
        }
        break;
    }
    case kTypeMethod: {
        new (&value.m_method) method_t();

        const Method *method = type->as_method();
        size_t num_params = method->num_params();
        for(unsigned int i = 0; i < num_params; ++i) {
            const Parameter *param = method->nth_param(i);
            value.m_method.emplace(param, unpack(param->type(), packed, offset));
        }
        break;
    }
    case kTypeNone:
        break;
    }

    if(offset > packed.size()) { throw InvalidValue("found EOF reading packed value"); }
    return value;
}

Value Value::unpack32(const Type *type, const vector<uint8_t>& packed, size_t offset)
{
    Value value;
    value.m_type = type;
    switch(type->subtype()) {
    case kTypeString: {
        new (&value.m_string) string();
        if(type->has_fixed_size()) {
            value.m_string = safe_read_string(packed, type->fixed_size(), offset);
        } else {
            uint32_t size = safe_read_int32(packed, offset);
            value.m_string = safe_read_string(packed, size, offset);
        }
        break;
    }
    case kTypeBlob: {
        new (&value.m_blob) vector<uint8_t>();
        if(type->has_fixed_size()) {
            value.m_blob = safe_read_data(packed, type->fixed_size(), offset);
        } else {
            uint32_t size = safe_read_int32(packed, offset);
            value.m_blob = safe_read_data(packed, size, offset);
        }
        break;
    }
    case kTypeArray: {
        new (&value.m_array) array_t();

        uint32_t size = 0;
        if(type->has_fixed_size()) {
            size = type->fixed_size();
        } else {
            size = safe_read_int32(packed, offset);
        }

        size_t array_end = offset + size;
        const Array *array = type->as_array();
        while(offset < array_end) {
            value.m_array.push_back(unpack32(array->element_type(), packed, offset));
        }
        if(offset > array_end) { throw InvalidValue("array values exceeded array size"); }
    }
    case kTypeStruct: {
        new (&value.m_struct) struct_t();

        const Struct *struct_ = type->as_struct();
        size_t num_fields = struct_->num_fields();
        for(unsigned int i = 0; i < num_fields; ++i) {
            const Field *field = struct_->nth_field(i);
            value.m_struct.emplace(field, unpack32(field->type(), packed, offset));
        }
        break;
    }
    case kTypeMethod: {
        new (&value.m_method) method_t();

        const Method *method = type->as_method();
        size_t num_params = method->num_params();
        for(unsigned int i = 0; i < num_params; ++i) {
            const Parameter *param = method->nth_param(i);
            value.m_method.emplace(param, unpack32(param->type(), packed, offset));
        }
        break;
    }
    default:
        return unpack(type, packed, offset);
    }

    if(offset > packed.size()) { throw InvalidValue("found EOF reading packed value"); }
    return value;
}

std::string Value::format() const
{
    return format_value(this);
}

vector<uint8_t> Value::pack() const
{
    switch(m_type->subtype()) {
    case kTypeChar:
        return as_buffer(m_char);
    case kTypeInt8:
        return as_buffer(m_int8);
    case kTypeInt16:
        return as_buffer(m_int16);
    case kTypeInt32:
        return as_buffer(m_int32);
    case kTypeInt64:
        return as_buffer(m_int64);
    case kTypeUint8:
        return as_buffer(m_uint8);
    case kTypeUint16:
        return as_buffer(m_uint16);
    case kTypeUint32:
        return as_buffer(m_uint32);
    case kTypeUint64:
        return as_buffer(m_uint64);
    case kTypeFloat32:
        return as_buffer(m_float32);
    case kTypeFloat64:
        return as_buffer(m_float64);
    case kTypeFixed:
        {
            const Numeric *numeric = m_type->as_numeric();
            if(numeric->is_signed()) {
                switch(numeric->fixed_size()) {
                case 1:
                    return as_buffer(numeric->to_fixed_s8(m_float64));
                case 2:
                    return as_buffer(numeric->to_fixed_s16(m_float64));
                case 4:
                    return as_buffer(numeric->to_fixed_s32(m_float64));
                case 8:
                    return as_buffer(numeric->to_fixed_s64(m_float64));
                default:
                    return vector<uint8_t>();
                }
            } else {
                switch(numeric->fixed_size()) {
                case 1:
                    return as_buffer(numeric->to_fixed_u8(m_float64));
                case 2:
                    return as_buffer(numeric->to_fixed_u16(m_float64));
                case 4:
                    return as_buffer(numeric->to_fixed_u32(m_float64));
                case 8:
                    return as_buffer(numeric->to_fixed_u64(m_float64));
                default:
                    return vector<uint8_t>();
                }
            }
        }
    case kTypeString:
        {
            if(m_type->has_fixed_size()) { return as_buffer(m_string); }

            vector<uint8_t> packed = as_buffer(uint16_t(m_string.size()));
            pack_value(as_buffer(m_string), packed);
            return packed;
        }
    case kTypeBlob:
        {
            if(m_type->has_fixed_size()) { return m_blob; }

            vector<uint8_t> packed = as_buffer(uint16_t(m_string.size()));
            pack_value(m_blob, packed);
            return packed;
        }
    case kTypeArray:
        {
            if(m_type->has_fixed_size()) {
                vector<uint8_t> packed;
                for(const Value& element : m_array) {
                    pack_value(element.pack(), packed);
                }
                return packed;
            } else {
                // Reserve a 16-bit unsigned integer
                vector<uint8_t> packed = as_buffer(uint16_t(0));
                for(const Value& element : m_array) {
                    pack_value(element.pack(), packed);
                }
                // Set the size using the reserved bytes
                uint16_t *size_ptr = (uint16_t *)&packed[0];
                *size_ptr = uint16_t(packed.size() - sizeof(uint16_t));
                return packed;
            }
        }
    case kTypeStruct:
        {
            const Struct *container = m_type->as_struct();
            size_t num_fields = container->num_fields();
            vector<uint8_t> packed;
            for(unsigned int i = 0; i < num_fields; ++i) {
                pack_value(m_struct.at(container->nth_field(i)).pack(), packed);
            }
            return packed;
        }
    case kTypeMethod:
        {
            const Method *method = m_type->as_method();
            size_t num_params = method->num_params();
            vector<uint8_t> packed;
            for(unsigned int i = 0; i < num_params; ++i) {
                pack_value(m_method.at(method->nth_param(i)).pack(), packed);
            }
            return packed;
        }
    case kTypeNone:
        break;
    }

    return vector<uint8_t>();
};

vector<uint8_t> Value::pack32() const
{
    switch(m_type->subtype()) {
    case kTypeString:
        {
            if(m_type->has_fixed_size()) { return pack(); }

            vector<uint8_t> packed = as_buffer(uint32_t(m_string.size()));
            pack_value(as_buffer(m_string), packed);
            return packed;
        }
    case kTypeBlob:
        {
            if(m_type->has_fixed_size()) { return pack(); }

            vector<uint8_t> packed = as_buffer(uint32_t(m_string.size()));
            pack_value(m_blob, packed);
            return packed;
        }
    case kTypeArray:
        {
            if(m_type->has_fixed_size()) { return pack(); }

            // Reserve a 32-bit unsigned integer
            vector<uint8_t> packed = as_buffer(uint32_t(0));
            for(const Value& element : m_array) {
                pack_value(element.pack32(), packed);
            }
            // Set the size using the reserved bytes
            uint32_t *size_ptr = (uint32_t *)&packed[0];
            *size_ptr = uint32_t(packed.size() - sizeof(uint32_t));
            return packed;
        }
    case kTypeStruct:
        {
            const Struct *container = m_type->as_struct();
            size_t num_fields = container->num_fields();
            vector<uint8_t> packed;
            for(unsigned int i = 0; i < num_fields; ++i) {
                pack_value(m_struct.at(container->nth_field(i)).pack32(), packed);
            }
            return packed;
        }
    case kTypeMethod:
        {
            const Method *method = m_type->as_method();
            size_t num_params = method->num_params();
            vector<uint8_t> packed;
            for(unsigned int i = 0; i < num_params; ++i) {
                pack_value(m_method.at(method->nth_param(i)).pack32(), packed);
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

    swap(lhs.m_type, rhs.m_type);
    switch(lhs.m_type->subtype()) {
    case kTypeChar:
        swap(lhs.m_char, rhs.m_char);
        break;
    case kTypeInt8:
        swap(lhs.m_int8, rhs.m_int8);
        break;
    case kTypeInt16:
        swap(lhs.m_int16, rhs.m_int16);
        break;
    case kTypeInt32:
        swap(lhs.m_int32, rhs.m_int32);
        break;
    case kTypeInt64:
        swap(lhs.m_int64, rhs.m_int64);
        break;
    case kTypeUint8:
        swap(lhs.m_uint8, rhs.m_uint8);
        break;
    case kTypeUint16:
        swap(lhs.m_uint16, rhs.m_uint16);
        break;
    case kTypeUint32:
        swap(lhs.m_uint32, rhs.m_uint32);
        break;
    case kTypeUint64:
        swap(lhs.m_uint64, rhs.m_uint64);
        break;
    case kTypeFloat32:
        swap(lhs.m_float32, rhs.m_float32);
        break;
    case kTypeFloat64:
    case kTypeFixed:
        swap(lhs.m_float64, rhs.m_float64);
        break;
    case kTypeString:
        swap(lhs.m_string, rhs.m_string);
        break;
    case kTypeBlob:
        swap(lhs.m_blob, rhs.m_blob);
        break;
    case kTypeArray:
        swap(lhs.m_array, rhs.m_array);
        break;
    case kTypeStruct:
        swap(lhs.m_struct, rhs.m_struct);
        break;
    case kTypeMethod:
        swap(lhs.m_method, rhs.m_method);
        break;
    case kTypeNone:
        break;
    }
}



} // close namespace bamboo

