// Filename: MethodValue.cpp
#include "MethodValue.h"
#include <typeinfo> // bad_cast
#include "module/Method.h"
#include "module/Parameter.h"
using namespace std;
namespace bamboo {


MethodValue::MethodValue(const Method *str) : m_method(str), m_params() {}

vector<uint8_t> MethodValue::pack(const DistributedType *type) const {
    vector<uint8_t> ret;
    pack(type, ret);
    return ret;
}
void MethodValue::pack(const DistributedType *type, vector<uint8_t>& buf) const {
    const Method *method = type->as_method();
    if(method != m_method) {
        throw bad_cast();
    }

    size_t num_params = m_method->get_num_parameters();
    for(unsigned int i = 0; i < num_params; ++i) {
        const Parameter *param = m_method->get_parameter(i);
        auto found_item = m_params.find(i);
        if(found_item == m_params.end()) {
            param->get_default_value().pack(param->get_type(), buf);
        } else {
            found_item->second.pack(param->get_type(), buf);
        }
    }
}

sizetag_t MethodValue::size() const {
    return m_method->get_num_parameters();
}

Value MethodValue::get_item(sizetag_t index) {
    if(index >= m_method->get_num_parameters()) {
        throw out_of_range("parameter index out of range for method value.");
    }

    auto found_item = m_params.find(index);
    if(found_item == m_params.end()) {
        return m_method->get_parameter(index)->get_default_value();
    }

    return found_item->second;
}
Value MethodValue::get_item(const string& item) {
    if(m_method->get_parameter_by_name(item) == nullptr) {
        throw out_of_range("parameter name invalid for method value.");
    }

    sizetag_t index = m_method->get_parameter_names().find(item)->second;
    auto found_item = m_params.find(index);
    if(found_item == m_params.end()) {
        return m_method->get_parameter(index)->get_default_value();
    }

    return found_item->second;
}
const Value MethodValue::get_item(sizetag_t index) const {
    if(index >= m_method->get_num_parameters()) {
        throw out_of_range("parameter index out of range for method value.");
    }

    auto found_item = m_params.find(index);
    if(found_item == m_params.end()) {
        return m_method->get_parameter(index)->get_default_value();
    }

    return found_item->second;
}
const Value MethodValue::get_item(const string& item) const {
    if(m_method->get_parameter_by_name(item) == nullptr) {
        throw out_of_range("parameter name invalid for method value.");
    }

    sizetag_t index = m_method->get_parameter_names().find(item)->second;
    auto found_item = m_params.find(index);
    if(found_item == m_params.end()) {
        return m_method->get_parameter(index)->get_default_value();
    }

    return found_item->second;
}
void MethodValue::set_item(sizetag_t index, const Value val) {
    if(index >= m_method->get_num_parameters()) {
        throw out_of_range("parameter assignment index out of range for method value.");
    }

    m_params.insert(pair<sizetag_t, Value>(index, val));
}
void MethodValue::set_item(const string& item, const Value val) {
    if(m_method->get_parameter_by_name(item) == nullptr) {
        throw out_of_range("parameter name invalid for assignment to method value.");
    }
    sizetag_t index = m_method->get_parameter_names().find(item)->second;
    m_params.insert(pair<sizetag_t, Value>(index, val));
}



} // close namespace bamboo
