// Filename: Field.cpp
#include "Field.h"
#include "../module/Module.h"
#include "../module/Struct.h"
#include "../module/Value.h"
using namespace std;
namespace bamboo
{


Field::Field(Type *type, const string& name) : m_type(type), m_name(name)
{
    if(m_type == nullptr) { m_type = Type::None; }
}

Field::~Field()
{
    delete m_type;
    delete m_default_value;
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

void Field::set_type(Type *type)
{
    m_type = type;

    // Reset our default value, if we had one
    if(m_default_value != nullptr) {
        delete m_default_value;
        m_default_value = nullptr;
    }
}

void Field::set_id(unsigned int id)
{
    m_id = id;
}

void Field::set_struct(Struct *strct)
{
    m_struct = strct;
}

bool Field::set_default_value(const Value& default_value)
{
    if(default_value.type() != this->type()) { return false; }
    if(m_default_value != nullptr) { delete m_default_value; }
    m_default_value = new Value(default_value);
    return true;
}

bool Field::set_default_value(const Value *default_value)
{
    return set_default_value(*default_value);
}


} // close namespace bamboo
