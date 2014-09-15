// Filename: Parameter.cpp
#include "Parameter.h"
#include "../module/Struct.h"
#include "../module/Method.h"
#include "Value.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{


// constructor
Parameter::Parameter(Type *type, const string& name) : m_type(type), m_type_alias(), m_name(name)
{
    if(m_type == nullptr) { m_type = Type::invalid; }
}

// set_name sets the name of this parameter.  Returns false if a parameter with
//     the same name already exists in the containing method.
bool Parameter::set_name(const string& name)
{
    // Check to make sure no other fields in our struct have this name
    if(m_method != nullptr && m_method->parameter_by_name(name) != nullptr) {
        return false;
    }

    m_name = name;
    return true;
}

// set_type sets the distributed type of the parameter and clear's the default value.
bool Parameter::set_type(Type *type)
{
    // Parameters can't have method types for now
    if(type->subtype() == kTypeMethod) {
        return false;
    }

    // Parameters can't have class types for now
    if(type->subtype() == kTypeStruct && type->as_struct()->as_class()) {
        return false;
    }

    // Set the type
    m_type = type;

    // Reset our default value, if we had one
    if(m_default_value != nullptr) {
        delete m_default_value;
        m_default_value = nullptr;
    }

    return true;
}

// set_default_value defines a default value for this parameter.
//     Returns false if the value is invalid for the parameter's type.
bool Parameter::set_default_value(const Value& default_value)
{
    // TODO: Validate default value
    if(m_default_value != nullptr) { delete m_default_value; }
    m_default_value = new Value(default_value);
    return true;
}
bool Parameter::set_default_value(const Value *default_value)
{
    return set_default_value(default_value);
}
bool Parameter::set_default_value(const vector<uint8_t>& default_value)
{
    // TODO: Validate default value
    if(m_default_value != nullptr) { delete m_default_value; }
    m_default_value = new Value(m_type, default_value);
    return true;
}

// set_position allows a method to tell the parameter its order in the function.
void Parameter::set_position(unsigned int pos)
{
    m_position = pos;
}

// set_method sets a pointer to the method containing the parameter.
void Parameter::set_method(Method *method)
{
    m_method = method;
}


} // close namespace bamboo
