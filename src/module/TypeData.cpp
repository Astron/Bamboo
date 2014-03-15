// Filename: TypeData.cpp
#include "TypeData.h"
#include <assert.h>
#include <sstream>
#include <stdexcept>
#include "bits/buffers.h"
#include "module/DistributedType.h"
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Method.h"
#include "module/Field.h"
#include "module/Parameter.h"
using namespace std;
namespace bamboo {

TypeData::TypeData(const DistributedType *type) : TypeData(type, nullptr, nullptr) {}

TypeData::TypeData(const DistributedType *type, const vector<uint8_t>& data) :
    TypeData(type, data, 0, 0) {}

TypeData::TypeData(const DistributedType *type, const vector<uint8_t>& data,
                   sizetag_t start, sizetag_t end) : TypeData(type, &data[start], &data[end]) {}

TypeData::TypeData(const DistributedType *type, const uint8_t *start, const uint8_t *end) :
    m_type(type), m_data(start, end), m_num_elements(), m_elements_by_name() {
    // Lets catch this case early on so it doesn't cause weird crashes later.
    if(type == nullptr) {
        throw invalid_argument("Tried to construct TypeData with no type.");
    }

    switch(m_type->get_subtype()) {
        case kTypeArray: {
            const ArrayType* arr = type->as_array();
            m_num_elements = arr->get_array_size();
            break;
        }
        case kTypeVararray: {
            const ArrayType* arr = type->as_array();
            const DistributedType* etype = arr->get_element_type();
            if(etype->has_fixed_size()) {
                sizetag_t offset = 0;
                m_num_elements = read_size(m_data, offset) / etype->get_size();
            } else {
                sizetag_t offset = 0;
                sizetag_t end = read_size(m_data, offset);
                while(offset < end) {
                    offset = read_type(etype, offset).m_end;
                    m_num_elements += 1;
                }
            }
            break;
        }
        case kTypeStruct: {
            const Struct* strct = type->as_struct();
            m_num_elements = strct->get_num_fields();
            m_elements_by_name = strct->get_field_names();
            break;
        }
        case kTypeMethod: {
            const Method* method = type->as_method();
            m_num_elements = method->get_num_parameters();
            m_elements_by_name = method->get_parameter_names();
            break;
        }
        default: {
            m_num_elements = 1;
        }
    }
}

TypeDataHandle TypeData::handle() const { return TypeDataHandle(this, 0, m_data.size()); }

TypeDataHandle TypeData::read_type(const DistributedType* type, sizetag_t offset) const {
    switch(m_type->get_subtype()) {
        case kTypeChar: {
            sizetag_t start = offset;
            read_char(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeInt8: {
            sizetag_t start = offset;
            read_int8(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeInt16: {
            sizetag_t start = offset;
            read_int16(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeInt32: {
            sizetag_t start = offset;
            read_int32(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeInt64: {
            sizetag_t start = offset;
            read_int64(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeUint8: {
            sizetag_t start = offset;
            read_uint8(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeUint16: {
            sizetag_t start = offset;
            read_uint16(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeUint32: {
            sizetag_t start = offset;
            read_uint32(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeUint64: {
            sizetag_t start = offset;
            read_uint64(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeFloat32: {
            sizetag_t start = offset;
            read_float32(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeFloat64: {
            sizetag_t start = offset;
            read_float64(m_data, offset);
            return TypeDataHandle(this, start, offset);
        }
        case kTypeString:
        case kTypeBlob: {
            return TypeDataHandle(this, offset, offset + type->get_size());
        }
        case kTypeArray: {
            if(type->has_fixed_size()) {
                return TypeDataHandle(this, offset, offset + type->get_size());
            }

            sizetag_t start = offset;
            sizetag_t next = offset;
            const ArrayType* arr = m_type->as_array();
            for(unsigned int i = 0; i < arr->get_array_size(); ++i) {
                // We're not going to validate the data-length here, any TypeData must be valid.
                next = read_type(arr->get_element_type(), next).m_end;
            }
            return TypeDataHandle(this, start, next);
        }
        case kTypeVarstring:
        case kTypeVararray:
        case kTypeVarblob: {
            sizetag_t start = offset;
            sizetag_t size = read_size(m_data, offset);
            return TypeDataHandle(this, start, offset + size);
        }
        case kTypeStruct: {
            if(type->has_fixed_size()) {
                return TypeDataHandle(this, offset, offset + type->get_size());
            }

            sizetag_t start = offset;
            sizetag_t next = offset;
            const Struct* strct = m_type->as_struct();
            for(unsigned int i = 0; i < strct->get_num_fields(); ++i) {
                // We're not going to validate the data-length here, any TypeData must be valid.
                next = read_type(strct->get_field(i)->get_type(), next).m_end;
            }
            return TypeDataHandle(this, start, next);
        }
        case kTypeMethod: {
            if(type->has_fixed_size()) {
                return TypeDataHandle(this, offset, offset + type->get_size());
            }

            sizetag_t start = offset;
            sizetag_t next = offset;
            const Method* method = m_type->as_method();
            for(unsigned int i = 0; i < method->get_num_parameters(); ++i) {
                // We're not going to validate the data-length here, any TypeData must be valid.
                next = read_type(method->get_parameter(i)->get_type(), next).m_end;
            }
            return TypeDataHandle(this, start, next);
        }
        case kTypeInvalid:
        default: {
            return TypeDataHandle(this, offset, offset);
        }
    }
}

TypeDataHandle::TypeDataHandle(const TypeData* data, sizetag_t start, sizetag_t end) :
    m_data(data), m_start(start), m_end(end), m_furthest_index(0), m_element_offsets()
{
    if(m_data->type()->get_subtype() == kTypeVararray) {
        m_element_offsets[0] = sizeof(sizetag_t);
    } else {
        m_element_offsets[0] = 0;        
    }
}

TypeDataHandle::TypeDataHandle(const TypeData& data) :
    TypeDataHandle(&data, 0, data.m_data.size()) {}

TypeDataHandle TypeDataHandle::get_item(sizetag_t index) {
    return this->operator[](index);
}
TypeDataHandle TypeDataHandle::get_item(const string& element_name) {
    return this->operator[](element_name);
}


TypeDataHandle TypeDataHandle::operator[](sizetag_t index) {
    if(index >= m_data->size()) {
        stringstream ss;
        ss << "Tried to reference TypeData[" << index << "] out of " << m_data->size()
           << " for value with type \"" << m_data->type()->to_string() << "\".";
        throw out_of_range(ss.str());
    }

    auto found_offset = m_element_offsets.find(index);
    switch(m_data->type()->get_subtype()) {
        case kTypeArray: {
            const ArrayType* arr = m_data->type()->as_array();
            const DistributedType* etype = arr->get_element_type();
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                return m_data->read_type(etype, found_offset->second);
            }
            
            if(etype->has_fixed_size()) {
                // Yay! We can just do random access.
                return m_data->read_type(etype, index * etype->get_size());
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       Previous indices must be generated for var-sized types, because
            //       random-access is prevented due to lack of size foreknowledge.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                TypeDataHandle handle = m_data->read_type(etype, curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            return m_data->read_type(etype, m_element_offsets[index]);
        }

        case kTypeVararray: {
            // VarArrays will be just like arrays above, except we have to skip the length tag
            const ArrayType* arr = m_data->type()->as_array();
            const DistributedType* etype = arr->get_element_type();
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                return m_data->read_type(etype, found_offset->second);
            }

            if(etype->has_fixed_size()) {
                // Yay! We can just do random access.
                return m_data->read_type(etype, index * etype->get_size() + sizeof(sizetag_t));
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       Previous indices must be generated for var-sized types, because
            //       random-access is prevented due to lack of size foreknowledge.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                TypeDataHandle handle = m_data->read_type(etype, curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            return m_data->read_type(etype, m_element_offsets[index]);
        }
        case kTypeStruct: {
            const Struct* strct = m_data->type()->as_struct();
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                return m_data->read_type(strct->get_field(index)->get_type(), found_offset->second);
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       We always generate early fields in a struct while reaching later fields.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                TypeDataHandle handle = m_data->read_type(strct->get_field(curr_index)->get_type(), curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            return m_data->read_type(strct->get_field(index)->get_type(), m_element_offsets[index]);
        }
        case kTypeMethod: {
            const Method* method = m_data->type()->as_method();
            const Parameter* param = nullptr;
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                param = method->get_parameter(index);
                return m_data->read_type(param->get_type(), found_offset->second);
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       We always generate early fields in a struct while reaching later fields.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                param = method->get_parameter(curr_index);
                TypeDataHandle handle = m_data->read_type(param->get_type(), curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            param = method->get_parameter(index);
            return m_data->read_type(param->get_type(), m_element_offsets[index]);
        }
        default: {
            sizetag_t offset = 0;
            return m_data->read_type(m_data->type(), offset);
        }
    }
}
TypeDataHandle TypeDataHandle::operator[](const string& element_name) {
    auto found_index = m_data->m_elements_by_name.find(element_name);
    if(found_index == m_data->m_elements_by_name.end()) {
        stringstream ss;
        ss << "TypeData[" << element_name << "] is not a valid subvalue"
           " for value with type \"" << m_data->type()->to_string() << "\".";
        throw out_of_range(ss.str());
    }

    sizetag_t index = found_index->second;
    return this->operator[](index);
}


} // close namespace bamboo
