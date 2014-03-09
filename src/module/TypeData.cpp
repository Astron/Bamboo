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


TypeData::TypeData(const DistributedType *type, const std::vector<uint8_t>& data) :
    m_type(type), m_data(data), m_start(0), m_end(0), m_elements_by_name(),
    m_element_offsets(), m_furthest_index(0) {
    // Lets catch this case early on so it doesn't cause weird crashes later.
    if(type == nullptr) {
        throw invalid_argument("Tried to construct TypeData with no type.");
    }

    switch(m_type->get_subtype()) {
        case kTypeArray: {
            const ArrayType* arr = type->as_array();
            m_num_elements = arr->get_array_size();
            m_element_offsets[0] = 0;
            break;
        }
        case kTypeVararray: {
            sizetag_t offset = 0;
            m_num_elements = read_size(data, offset);
            m_element_offsets[0] = sizeof(sizetag_t);
            break;
        }
        case kTypeStruct: {
            const Struct* strct = type->as_struct();
            m_num_elements = strct->get_num_fields();
            m_elements_by_name = strct->get_field_names();
            m_element_offsets[0] = 0;
            break;
        }
        case kTypeMethod: {
            const Method* method = type->as_method();
            m_num_elements = method->get_num_parameters();
            m_elements_by_name = method->get_parameter_names();
            m_element_offsets[0] = 0;
            break;
        }
        default: {
            m_num_elements = 0;
        }
    }
}

TypeData TypeData::get_item(sizetag_t index) {
    return this->operator[](index);
}
TypeData TypeData::get_item(const string& element_name) {
    return this->operator[](element_name);
}


TypeData TypeData::operator[](sizetag_t index) {
    if(index >= m_num_elements) {
        stringstream ss;
        ss << "Tried to reference TypeData[" << index << "] out of " << m_num_elements
           << " for value with type \"" << m_type->to_string() << "\".";
        throw std::out_of_range(ss.str());
    }

    auto found_offset = m_element_offsets.find(index);
    switch(m_type->get_subtype()) {
        case kTypeArray: {
            const ArrayType* arr = m_type->as_array();
            const DistributedType* etype = arr->get_element_type();
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                return read_type(etype, found_offset->second);
            }
            
            if(etype->has_fixed_size()) {
                // Yay! We can just do random access.
                return read_type(etype, index * etype->get_size());
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       Previous indices must be generated for var-sized types, because
            //       random-access is prevented due to lack of size foreknowledge.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                TypeData& handle = read_type(etype, curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            return read_type(etype, m_element_offsets[index]);
        }

        case kTypeVararray: {
            // VarArrays will be just like arrays above, except we have to skip the length tag
            const ArrayType* arr = m_type->as_array();
            const DistributedType* etype = arr->get_element_type();
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                return read_type(etype, found_offset->second);
            }

            if(etype->has_fixed_size()) {
                // Yay! We can just do random access.
                return read_type(etype, index * etype->get_size() + sizeof(sizetag_t));
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       Previous indices must be generated for var-sized types, because
            //       random-access is prevented due to lack of size foreknowledge.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                TypeData& handle = read_type(etype, curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            return read_type(etype, m_element_offsets[index]);
        }
        case kTypeStruct: {
            const Struct* strct = m_type->as_struct();
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                return read_type(strct->get_field(index)->get_type(), found_offset->second);
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       We always generate early fields in a struct while reaching later fields.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                TypeData& handle = read_type(strct->get_field(curr_index)->get_type(), curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            return read_type(strct->get_field(index)->get_type(), m_element_offsets[index]);
        }
        case kTypeMethod: {
            const Method* method = m_type->as_method();
            const Parameter* param = nullptr;
            if(found_offset != m_element_offsets.end()) {
                // In the lucky case, we already found the offset
                param = method->get_parameter(index);
                return read_type(param->get_type(), found_offset->second);
            }

            // N. B. In this case, "index > m_furthest_index" will always hold.
            //       We always generate early fields in a struct while reaching later fields.
            sizetag_t curr_index = m_furthest_index;
            sizetag_t curr_offset = m_element_offsets[curr_index];
            while(curr_index != index) {
                param = method->get_parameter(curr_index);
                TypeData& handle = read_type(param->get_type(), curr_offset);
                // Increment our loop variables
                curr_offset = handle.m_end;
                curr_index += 1;
                // Set the start of our next element
                m_element_offsets[curr_index] = curr_offset;
            }
            m_furthest_index = index;
            param = method->get_parameter(index);
            return read_type(param->get_type(), m_element_offsets[index]);
        }
        default: {
            sizetag_t offset = 0;
            return read_type(m_type, offset);
        }
    }
}
TypeData TypeData::operator[](const std::string& element_name) {
    auto found_index = m_elements_by_name.find(element_name);
    if(found_index == m_elements_by_name.end()) {
        stringstream ss;
        ss << "TypeData[" << element_name << "] is not a valid subvalue"
           " for value with type \"" << m_type->to_string() << "\".";
        throw std::out_of_range(ss.str());
    }

    sizetag_t index = found_index->second;
    return this->operator[](index);
}


} // close namespace bamboo
