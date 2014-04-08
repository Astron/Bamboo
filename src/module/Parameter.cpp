// Filename: Parameter.cpp
#include "Parameter.h"
#include "module/Struct.h"
#include "module/Method.h"
#include "values/Value.h"
using namespace std;
namespace bamboo { // open namespace bamboo


// constructor
Parameter::Parameter(DistributedType *type, const string& name) :
    m_name(name), m_type(type), m_method(nullptr),
    m_default_value(Value::from_type(type, m_has_default_value)) {
    if(m_type == nullptr) {
        m_type = DistributedType::invalid;
    }
}

// set_name sets the name of this parameter.  Returns false if a parameter with
//     the same name already exists in the containing method.
bool Parameter::set_name(const string& name) {
    // Check to make sure no other fields in our struct have this name
    if(m_method != nullptr && m_method->get_parameter_by_name(name) != nullptr) {
        return false;
    }

    m_name = name;
    return true;
}

// set_type sets the distributed type of the parameter and clear's the default value.
bool Parameter::set_type(DistributedType *type) {
    if(type == nullptr) {
        return false;
    }

    // Parameters can't have method types for now
    if(type->get_subtype() == kTypeMethod) {
        return false;
    }

    // Parameters can't have class types for now
    if(type->get_subtype() == kTypeStruct && type->as_struct()->as_class()) {
        return false;
    }

    m_type = type;
    m_default_value = Value::from_type(type, m_has_default_value);

    return true;
}

// set_default_value defines a default value for this parameter.
//     Returns false if the value is invalid for the parameter's type.
bool Parameter::set_default_value(const Value default_value) {
    // TODO: Validate default value
    m_has_default_value = true;
    m_default_value = default_value;
    return true;
}
bool Parameter::set_default_value(const vector<uint8_t>& default_value) {
    // TODO: Validate default value
    m_has_default_value = true;
    m_default_value = Value::from_packed(m_type, default_value);
    return true;
}

// set_method sets a pointer to the method containing the parameter.
void Parameter::set_method(Method *method) {
    m_method = method;
}


} // close namespace bamboo
