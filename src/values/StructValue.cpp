// Filename: StructValue.cpp
#include "StructValue.h"
#include <typeinfo> // bad_cast
#include "module/Struct.h"
#include "module/Field.h"
using namespace std;
namespace bamboo {


StructValue::StructValue(const Struct *str) : m_struct(str), m_fields() {}

vector<uint8_t> StructValue::pack(const DistributedType *type) const {
    vector<uint8_t> ret;
    pack(type, ret);
    return ret;
}
void StructValue::pack(const DistributedType *type, vector<uint8_t>& buf) const {
    const Struct *str = type->as_struct();
    if(str != m_struct) {
        throw bad_cast();
    }

    size_t num_fields = m_struct->get_num_fields();
    for(unsigned int i = 0; i < num_fields; ++i) {
        const Field *field = m_struct->get_field(i);
        auto found_item = m_fields.find(i);
        if(found_item == m_fields.end()) {
            field->get_default_value().pack(field->get_type(), buf);
        } else {
            found_item->second.pack(field->get_type(), buf);
        }
    }
}

sizetag_t StructValue::size() const {
    return m_struct->get_num_fields();
}

Value StructValue::get_item(sizetag_t index) {
    if(index >= m_struct->get_num_fields()) {
        throw out_of_range("field index out of range for struct value.");
    }

    auto found_item = m_fields.find(index);
    if(found_item == m_fields.end()) {
        return m_struct->get_field(index)->get_default_value();
    }

    return found_item->second;
}
Value StructValue::get_item(const string& item) {
    if(m_struct->get_field_by_name(item) == nullptr) {
        throw out_of_range("field name invalid for struct value.");
    }

    sizetag_t index = m_struct->get_field_names().find(item)->second;
    auto found_item = m_fields.find(index);
    if(found_item == m_fields.end()) {
        return m_struct->get_field(index)->get_default_value();
    }

    return found_item->second;
}
const Value StructValue::get_item(sizetag_t index) const {
    if(index >= m_struct->get_num_fields()) {
        throw out_of_range("field index out of range for struct value.");
    }

    auto found_item = m_fields.find(index);
    if(found_item == m_fields.end()) {
        return m_struct->get_field(index)->get_default_value();
    }

    return found_item->second;
}
const Value StructValue::get_item(const string& item) const {
    if(m_struct->get_field_by_name(item) == nullptr) {
        throw out_of_range("field name invalid for struct value.");
    }

    sizetag_t index = m_struct->get_field_names().find(item)->second;
    auto found_item = m_fields.find(index);
    if(found_item == m_fields.end()) {
        return m_struct->get_field(index)->get_default_value();
    }

    return found_item->second;
}
void StructValue::set_item(sizetag_t index, const Value val) {
    if(index >= m_struct->get_num_fields()) {
        throw out_of_range("field assignment index out of range for struct value.");
    }

    m_fields.insert(pair<sizetag_t, Value>(index, val));
}
void StructValue::set_item(const string& item, const Value val) {
    if(m_struct->get_field_by_name(item) == nullptr) {
        throw out_of_range("field name invalid for assignment to struct value.");
    }
    sizetag_t index = m_struct->get_field_names().find(item)->second;
    m_fields.insert(pair<sizetag_t, Value>(index, val));
}



} // close namespace bamboo
