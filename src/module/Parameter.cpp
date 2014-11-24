// Filename: Parameter.cpp
#include "Parameter.h"
#include "../module/Struct.h"
#include "../module/Method.h"
#include "../module/Value.h"
using namespace std;
namespace bamboo
{


Parameter::Parameter(Type *type, bool transfer_ownership) : Parameter("", type, transfer_ownership) {}
Parameter::Parameter(const string& name, Type *type, bool transfer_ownership) :
    m_name(name),
    m_type(type),
    m_type_owned(transfer_ownership)
{
    if(m_type == nullptr) { m_type = Type::None; }
}

Parameter::~Parameter()
{
    if(m_type_owned) { delete m_type; }
    if(m_default_value != nullptr) { delete m_default_value; }
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

bool Parameter::set_type(Type *type, bool transfer_ownership)
{
    // Parameters can't have method types for now
    if(type->subtype() == kTypeMethod) {
        return false;
    }

    // Parameters can't have class types for now
    if(type->subtype() == kTypeStruct && type->as_struct()->as_class()) {
        return false;
    }

    // Reset our default value, if we had one
    if(m_default_value != nullptr) {
        delete m_default_value;
        m_default_value = nullptr;
    }

    // Set the type
    if(m_method != nullptr) { m_method->update_param_type(this, type, m_type); }
    if(m_type_owned && m_type != nullptr) { delete m_type; }
    m_type = type;
    m_type_owned = transfer_ownership;

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


} // close namespace bamboo
