// Filename: Parameter.cpp
#include "Parameter.h"
#include "../module/Struct.h"
#include "../module/Method.h"
#include "../module/Value.h"
using namespace std;
namespace bamboo
{


Parameter::Parameter(Type *type, const string& name) : m_type(type), m_name(name)
{
    if(m_type == nullptr) { m_type = Type::None; }
}

bool Parameter::set_name(const string& name)
{
    // Check to make sure no other fields in our struct have this name
    if(m_method != nullptr && m_method->param_by_name(name) != nullptr) {
        return false;
    }

    m_name = name;
    return true;
}

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

bool Parameter::set_default_value(const Value& default_value)
{
    if(default_value.type() != this->type()) { return false; }
    if(m_default_value != nullptr) { delete m_default_value; }
    m_default_value = new Value(default_value);
    return true;
}

bool Parameter::set_default_value(const Value *default_value)
{
    return set_default_value(*default_value);
}

void Parameter::set_position(unsigned int pos)
{
    m_position = pos;
}

void Parameter::set_method(Method *method)
{
    m_method = method;
}


} // close namespace bamboo
