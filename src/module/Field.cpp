// Filename: Field.cpp
#include "Field.h"
#include "../module/Module.h"
#include "../module/Struct.h"
#include "../module/Value.h"
using namespace std;
namespace bamboo
{

Field::Field(Type *type, bool transfer_ownership) : Field("", type, transfer_ownership) {}
Field::Field(const string& name, Type *type, bool transfer_ownership) :
    m_name(name),
    m_type(type),
    m_type_owned(transfer_ownership)
{
    if(m_type == nullptr) { m_type = Type::None; }
}

Field::~Field()
{
    if(m_type_owned) { delete m_type; }
    if(m_default_value != nullptr) { delete m_default_value; }
}

MolecularField *Field::as_molecular()
{
    return nullptr;
}

const MolecularField *Field::as_molecular() const
{
    return nullptr;
}

bool Field::set_name(const string& name)
{
    // Check to make sure no other fields in our struct have this name
    if(m_struct != nullptr && m_struct->field_by_name(name) != nullptr) {
        return false;
    }

    m_name = name;
    return true;
}

bool Field::set_type(Type *type, bool transfer_ownership)
{
    // Can't change the type of a field that has been added to a struct
    if(m_struct != nullptr) {
        return false;
    }

    // Field can't have null type
    if(type == nullptr) {
        return false;
    }

    if(m_type_owned) { delete m_type; }
    m_type_owned = transfer_ownership;
    m_type = type;

    // Reset our default value, if we had one
    if(m_default_value != nullptr) {
        delete m_default_value;
        m_default_value = nullptr;
    }

    return true;
}

bool Field::set_default_value(const Value& default_value)
{
    return set_default_value(&default_value);
}

bool Field::set_default_value(const Value *default_value)
{
    if(default_value == nullptr) {
        if(m_default_value != nullptr) { delete m_default_value; }
        m_default_value = nullptr;
        return true;
    }

    if(default_value->type() != this->type()) {
        return false;
    }

    if(m_default_value != nullptr) { delete m_default_value; }
    m_default_value = new Value(*default_value);
    return true;
}


} // close namespace bamboo
