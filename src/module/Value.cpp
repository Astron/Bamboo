// Filename: Value.cpp
#include "Value.h"
#include <cctype>  // isprint
#include <sstream> // ostringstream
#include "../bits/buffers.h"
#include "../module/Array.h"
#include "../module/Struct.h"
#include "../module/Method.h"
#include "../module/Numeric.h"
#include "../dcfile/lexer.h"
#include "../dcfile/parser.h"
using namespace std;

#if defined(_MSC_VER) && _MSC_VER <= 1800
#define snprintf sprintf_s
#endif

namespace bamboo   // open namespace bamboo
{

static string format_value(const Value& value);
static void format_value(const Value& value, ostream& out);
static void format_hex(uint8_t byte, ostream& out);
static void format_hex(const vector<uint8_t>& bytes, ostream& out);
static void format_quoted(char chr, ostream& out);
static void format_quoted(const string& str, ostream& out);

Value::Value(const Type *type) : m_type(type)
{
    if(m_type == nullptr) { m_type = Type::None; }
    switch(m_type->subtype()) {
    case Subtype_String:
        new (&m_string) string();
        break;
    case Subtype_Blob:
        new (&m_blob) vector<uint8_t>();
        break;
    case Subtype_Array:
        new (&m_array) vector<Value>();
        break;
    case Subtype_Struct:
        new (&m_struct) struct_t();
        break;
    case Subtype_Method:
        new (&m_method) method_t();
        break;
    case Subtype_None:
    case Subtype_Numeric:
        break;
    }
}

Value::Value(const Value& other) : m_type(other.m_type)
{
    switch(m_type->subtype()) {
    case Subtype_Numeric:
        m_numeric = other.m_numeric;
        break;
    case Subtype_String:
        m_string = other.m_string;
        break;
    case Subtype_Blob:
        m_blob = other.m_blob;
        break;
    case Subtype_Array:
        m_array = other.m_array;
        break;
    case Subtype_Struct:
        m_struct = other.m_struct;
        break;
    case Subtype_Method:
        m_method = other.m_method;
        break;
    case Subtype_None:
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
    case Subtype_String:
        m_string.~string();
        break;
    case Subtype_Blob:
        m_blob.~vector<uint8_t>();
        break;
    case Subtype_Array:
        m_array.~vector<Value>();
        break;
    case Subtype_Struct:
        m_struct.~struct_t();
        break;
    case Subtype_Method:
        m_method.~method_t();
        break;
    case Subtype_Numeric:
    case Subtype_None:
        break;
    }
}

Value Value::Default(const Type *type)
{
    Value value(type);
    switch(type->subtype()) {
    case Subtype_Numeric:
        {
            const Numeric *numeric = type->as_numeric();
            if(numeric->is_unsigned()) { value.m_numeric = (uint64_t)0; }
            else if(numeric->is_signed()) { value.m_numeric = (int64_t)0; }
            else if(numeric->is_floating()) { value.m_numeric = (double)0; }
            else { value.m_numeric = Number(); }   
        }
        break;
    case Subtype_String:
        value.m_string.assign((size_t)type->as_array()->range().min.uinteger, '\0');
        break;
    case Subtype_Blob:
        value.m_blob.assign((size_t)type->as_array()->range().min.uinteger, (uint8_t)0);
        break;
    case Subtype_Array:
        {
            const Array *array = type->as_array();
            value.m_array.assign((size_t)array->range().min.uinteger,
                                 Default(array->element_type()));
        }
        break;
    case Subtype_Struct:
        {
            // Construct the struct from default values for its fields
            const Struct *struct_ = type->as_struct();
            size_t num_fields = struct_->num_fields();
            for(size_t i = 0; i < num_fields; ++i) {
                const Field *field = struct_->nth_field((unsigned int)i);
                if(field->has_default_value()) {
                    // Get default value for field
                    value.m_struct.emplace(field, *field->default_value());
                } else {
                    // Otherwise, use the implicit default (0, empty, etc...)
                    value.m_struct.emplace(field, Default(field->type()));
                }
            }
        }
        break;
    case Subtype_Method:
        {
            // Construct the method-call from default values for its parameters
            const Method *method = type->as_method();
            size_t num_params = method->num_params();
            for(unsigned int i = 0; i < num_params; ++i) {
                const Parameter *param = method->nth_param(i);
                if(param->has_default_value()) {
                    // Get default value for field
                    value.m_method.emplace(param, *param->default_value());
                } else {
                    // Otherwise, use the implicit default (0, empty, etc...)
                    value.m_method.emplace(param, Default(param->type()));
                }
            }
        }
        break;
    case Subtype_None:
        break;
    }

    return value;
}

Value Value::parse(const Type *type, const string& formatted)
{
    auto lexer = Lexer(formatted.c_str(), formatted.length());
    auto parser = Parser(&lexer);
    parser.start();

    Value *value = parser.parse_value_expr(type);
    if(value == nullptr) { throw InvalidValue("encountered errors parsing formatted value"); }
    return *value;
}

Value Value::unpack(const Type *type, const vector<uint8_t>& packed, size_t offset)
{
    Value value;
    value.m_type = type;
    switch(type->subtype()) {
    case Subtype_Numeric:
        {
            const Numeric *numeric = type->as_numeric();
            value.m_numeric = numeric->unpack(&packed[offset]);
            offset += numeric->fixed_size();
        }
        break;
    case Subtype_String:
        new (&value.m_string) string();
        if(type->has_fixed_size()) {
            value.m_string = safe_read_string(packed, type->fixed_size(), offset);
        } else {
            uint16_t size = safe_read_int16(packed, offset);
            value.m_string = safe_read_string(packed, size, offset);
        }
        break;
    case Subtype_Blob:
        new (&value.m_blob) vector<uint8_t>();
        if(type->has_fixed_size()) {
            value.m_blob = safe_read_data(packed, type->fixed_size(), offset);
        } else {
            uint16_t size = safe_read_int16(packed, offset);
            value.m_blob = safe_read_data(packed, size, offset);
        }
        break;
    case Subtype_Array:
        {
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
        break;
    case Subtype_Struct:
        {
            new (&value.m_struct) struct_t();

            const Struct *struct_ = type->as_struct();
            size_t num_fields = struct_->num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                const Field *field = struct_->nth_field(i);
                value.m_struct.emplace(field, unpack(field->type(), packed, offset));
            }
        }
        break;
    case Subtype_Method:
        {
            new (&value.m_method) method_t();

            const Method *method = type->as_method();
            size_t num_params = method->num_params();
            for(unsigned int i = 0; i < num_params; ++i) {
                const Parameter *param = method->nth_param(i);
                value.m_method.emplace(param, unpack(param->type(), packed, offset));
            }
        }
        break;
    case Subtype_None:
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
    case Subtype_String:
        new (&value.m_string) string();
        if(type->has_fixed_size()) {
            value.m_string = safe_read_string(packed, type->fixed_size(), offset);
        } else {
            uint32_t size = safe_read_int32(packed, offset);
            value.m_string = safe_read_string(packed, size, offset);
        }
        break;
    case Subtype_Blob:
        new (&value.m_blob) vector<uint8_t>();
        if(type->has_fixed_size()) {
            value.m_blob = safe_read_data(packed, type->fixed_size(), offset);
        } else {
            uint32_t size = safe_read_int32(packed, offset);
            value.m_blob = safe_read_data(packed, size, offset);
        }
        break;
    case Subtype_Array:
        {
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
        break;
    case Subtype_Struct:
        {
            new (&value.m_struct) struct_t();

            const Struct *struct_ = type->as_struct();
            size_t num_fields = struct_->num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                const Field *field = struct_->nth_field(i);
                value.m_struct.emplace(field, unpack32(field->type(), packed, offset));
            }
        }
        break;
    case Subtype_Method:
        {
            new (&value.m_method) method_t();

            const Method *method = type->as_method();
            size_t num_params = method->num_params();
            for(unsigned int i = 0; i < num_params; ++i) {
                const Parameter *param = method->nth_param(i);
                value.m_method.emplace(param, unpack32(param->type(), packed, offset));
            }
        }
        break;
    default:
        return unpack(type, packed, offset);
    }

    if(offset > packed.size()) { throw InvalidValue("found EOF reading packed value"); }
    return value;
}

string Value::format() const
{
    return format_value(*this);
}

vector<uint8_t> Value::pack() const
{
    switch(m_type->subtype()) {
    case Subtype_Numeric:
        {
            const Numeric *numeric = m_type->as_numeric();
            switch(m_numeric.type) {
            case Number_Signed:
                return numeric->pack(m_numeric.sinteger);
            case Number_Unsigned:
                return numeric->pack(m_numeric.uinteger);
            case Number_Floating:
                return numeric->pack(m_numeric.floating);
            case Number_NotANumber:
                return vector<uint8_t>();
            }
        }
        break;
    case Subtype_String:
        {
            if(m_type->has_fixed_size()) { return as_buffer(m_string); }

            vector<uint8_t> packed = as_buffer(uint16_t(m_string.size()));
            pack_value(as_buffer(m_string), packed);
            return packed;
        }
        break;
    case Subtype_Blob:
        {
            if(m_type->has_fixed_size()) { return m_blob; }

            vector<uint8_t> packed = as_buffer(uint16_t(m_string.size()));
            pack_value(m_blob, packed);
            return packed;
        }
        break;
    case Subtype_Array:
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
        break;
    case Subtype_Struct:
        {
            const Struct *container = m_type->as_struct();
            size_t num_fields = container->num_fields();
            vector<uint8_t> packed;
            for(unsigned int i = 0; i < num_fields; ++i) {
                pack_value(m_struct.at(container->nth_field(i)).pack(), packed);
            }
            return packed;
        }
        break;
    case Subtype_Method:
        {
            const Method *method = m_type->as_method();
            size_t num_params = method->num_params();
            vector<uint8_t> packed;
            for(unsigned int i = 0; i < num_params; ++i) {
                pack_value(m_method.at(method->nth_param(i)).pack(), packed);
            }
            return packed;
        }
        break;
    case Subtype_None:
        break;
    }

    return vector<uint8_t>();
};

vector<uint8_t> Value::pack32() const
{
    switch(m_type->subtype()) {
    case Subtype_String:
        {
            if(m_type->has_fixed_size()) { return pack(); }

            vector<uint8_t> packed = as_buffer(uint32_t(m_string.size()));
            pack_value(as_buffer(m_string), packed);
            return packed;
        }
    case Subtype_Blob:
        {
            if(m_type->has_fixed_size()) { return pack(); }

            vector<uint8_t> packed = as_buffer(uint32_t(m_string.size()));
            pack_value(m_blob, packed);
            return packed;
        }
    case Subtype_Array:
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
    case Subtype_Struct:
        {
            const Struct *container = m_type->as_struct();
            size_t num_fields = container->num_fields();
            vector<uint8_t> packed;
            for(unsigned int i = 0; i < num_fields; ++i) {
                pack_value(m_struct.at(container->nth_field(i)).pack32(), packed);
            }
            return packed;
        }
    case Subtype_Method:
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
    case Subtype_Numeric:
        swap(lhs.m_numeric, rhs.m_numeric);
        break;
    case Subtype_String:
        swap(lhs.m_string, rhs.m_string);
        break;
    case Subtype_Blob:
        swap(lhs.m_blob, rhs.m_blob);
        break;
    case Subtype_Array:
        swap(lhs.m_array, rhs.m_array);
        break;
    case Subtype_Struct:
        swap(lhs.m_struct, rhs.m_struct);
        break;
    case Subtype_Method:
        swap(lhs.m_method, rhs.m_method);
        break;
    case Subtype_None:
        break;
    }
}

static string format_value(const Value& value)
{
    ostringstream ss;
    format_value(value, ss);
    return ss.str();
}

static void format_value(const Value& value, ostream& out)
{
    const Type *type = value.type();
    Subtype subtype = type->subtype();
    switch(subtype) {
    case Subtype_Numeric:
        if(type == Type::Char) { format_quoted((char)value.m_numeric, out); }
        else if(type == Type::Byte) { format_hex((uint8_t)value.m_numeric, out); }
        out << value.m_numeric;
        break;
    case Subtype_String:
        // Only arrays using the built-in char-type (Type::Char) have Subtype_String
        format_quoted(value.m_string, out);
        break;
    case Subtype_Blob:
        // Format blob as a hex constant then output
        format_hex(value.m_blob, out);
        break;
    case Subtype_Array:
        {
            out << '[';
            if(value.m_array.size() > 0) {
                format_value(value.m_array[0], out);
            }
            for(size_t i = 1; i < value.m_array.size(); ++i) {
                out << ", ";
                format_value(value.m_array[i], out);
            }
            out << ']';
        }
        break;
    case Subtype_Struct:
        {
            out << '{';
            const Struct *struct_ = type->as_struct();
            size_t num_fields = struct_->num_fields();
            if(num_fields > 0) {
                const Field *field = struct_->nth_field(0);
                if(field->as_molecular() == nullptr) {
                    format_value(value.m_struct.at(field), out);
                }
            }
            for(unsigned int i = 1; i < num_fields; ++i) {
                out << ", ";
                const Field *field = struct_->nth_field(i);
                if(field->as_molecular() == nullptr) {
                    format_value(value.m_struct.at(field), out);
                }
            }
            out << '}';
        }
        break;
    case Subtype_Method:
        {
            out << '(';
            const Method *method_ = type->as_method();
            size_t num_params = method_->num_params();
            if(num_params > 0) {
                const Parameter *param = method_->nth_param(0);
                format_value(value.m_method.at(param), out);
            }
            for(unsigned int i = 1; i < num_params; ++i) {
                const Parameter *param = method_->nth_param(i);
                format_value(value.m_method.at(param), out);
            }
            out << ')';
        }
        break;
    case Subtype_None:
        out << "None";
        break;
    }
}

static void format_hex(uint8_t byte, ostream& out)
{
    char buffer[10];
    snprintf(buffer, 10, "%02x", byte);

    out << '<' << buffer << '>';
}

static void format_hex(const vector<uint8_t>& bytes, ostream& out)
{
    out << '<';
    for(auto it = bytes.begin(); it != bytes.end(); ++it) {
        uint8_t byte = *it;
        char buffer[10];
        snprintf(buffer, 10, "%02x", (uint8_t)(byte));
        out << buffer;
    }
    out << '>';
}

static void format_quoted(char chr, ostream& out)
{
    out << '\'';
    if(chr == '"' || chr == '\\') {
        // Escape the character
        out << '\\' << chr;
    } else if(!isprint(chr)) { // Character is not a printable ascii character
        // Print the character as an escaped hexidecimal character constant
        char buffer[10];
        snprintf(buffer, 10, "%02x", (unsigned char)chr);
        out << "\\x" << buffer;
    } else {
        out << chr;
    }
    out << '\'';
}

static void format_quoted(const string& str, ostream& out)
{
    out << '"';
    for(auto it = str.begin(); it != str.end(); ++it) {
        char chr = *it;
        if(chr == '"' || chr == '\\') {
            // Escape the character
            out << '\\' << chr;
        } else if(!isprint(chr)) { // Character is not a printable ascii character
            // Print the character as an escaped hexidecimal character constant
            char buffer[10];
            snprintf(buffer, 10, "%02x", (unsigned char)chr);
            out << "\\x" << buffer;
        } else {
            out << chr;
        }
    }
    out << '"';
}


} // close namespace bamboo

