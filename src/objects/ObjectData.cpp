// Filename: ObjectData.h
#pragma once
#include "ObjectData.h"

#include <sstream>
#include <stdexcept>
#include "module/DistributedType.h"
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Method.h"
using namespace std;
namespace bamboo {


/* ObjectData base-class definitions */
ObjectDataBase::ObjectDataBase(const DistributedType *type) :
    m_type(type), m_elements_by_name() {
    // Lets catch this case early on so it doesn't cause weird crashes later.
    if(type == nullptr) {
        throw invalid_argument("Tried to construct ObjectData with no type.");
    }

    switch(m_type->get_subtype()) {
        case kTypeArray:
            const ArrayType* arr = type->as_array();
            m_num_elements = arr->get_array_size();
            break;
        case kTypeVararray:
            const ArrayType* arr = type->as_array();
            m_num_elements = 0;
            break;
        case kTypeStruct:
            const Struct* strct = type->as_struct();
            m_num_elements = strct->get_num_fields();
            m_elements_by_name = strct->get_field_names();
            break;
        case kTypeMethod:
            const Method* method = type->as_method();
            m_num_elements = strct->get_num_parameters();
            m_elements_by_name = strct->get_parameter_names();
            break;
        default:
            m_num_elements = 1;
    }
}

const ObjectDataHandle& ObjectDataBase::get_item(sizetag_t index) const {
    return this->operator[](index);
}
const ObjectDataHandle& ObjectDataBase::get_item(const string& element_name) const {
    return this->operator[](element_name);
}
void ObjectDataBase::set_item(sizetag_t index, ObjectData item) {
    this->operator[](index) = item;
}
void ObjectDataBase::set_item(const string& element_name, ObjectData item) {
    this->operator[](element_name) = item;
}


/* ObjectData class definitions */
ObjectData::ObjectData(const DistributedType *type, const vector<uint8_t>& data) :
    ObjectDataBase(type), m_data(data) {}
ObjectData::ObjectData(const Struct *s, unsigned int field_id, const vector<uint8_t>& data) :
    ObjectDataBase(s->get_field_by_id(type)), m_data(data) {}

ObjectData& ObjectData::operator=(const ObjectDataHandle& handle) {
    m_type = handle.m_type;
    m_num_elements = handle.m_num_elements;
    m_elements_by_name = handle.m_elements_by_name;
    m_data = handle.data();
    return *this;
}

ObjectDataHandle& ObjectData::operator[](sizetag_t index) {
    if(index >= m_num_elements) {
        stringstream ss;
        ss << "Tried to reference ObjectData[" << index << "] out of " << m_num_elements
           << " for value with type \"" << m_type->to_string() << "\".";
        throw std::out_of_range(ss.str());
    }

    // FIXME: Actually return the value!
}
const ObjectDataHandle& ObjectData::operator[](sizetag_t index) const {
    if(index >= m_num_elements) {
        stringstream ss;
        ss << "Tried to reference ObjectData[" << index << "] out of " << m_num_elements
           << " for value with type \"" << m_type->to_string() << "\".";
        throw std::out_of_range(ss.str());
    }

    // FIXME: Actually return the value!
}
ObjectDataHandle& ObjectData::operator[](const std::string& element_name) {
    auto found_index = m_elements_by_name.find(element_name);
    if(found_index == m_elements_by_name.end()) {
        stringstream ss;
        ss << "ObjectData[" << element_name << "] is not a valid subvalue"
           " for value with type \"" << m_type->to_string() << "\".";
        throw std::out_of_range(ss.str());
    }

    sizetag_t index = found_index->second;

    // FIXME: Actually return the value!
}
const ObjectDataHandle& ObjectData::operator[](const std::string& element_name) {
    auto found_index = m_elements_by_name.find(element_name);
    if(found_index == m_elements_by_name.end()) {
        throw std::out_of_range("Buffer index out of range.");
    }

    sizetag_t index = found_index->second;

    // FIXME: Actually return the value!
}

/* ObjectDate handle-class definitions */


} // close namespace bamboo
