// Filename: Field.cpp
#include "Field.h"
#include "../module/Module.h"
#include "../module/Struct.h"
#include "Value.h"
using namespace std;
namespace bamboo   // open namespace
{

// constructor
Field::Field(Type *type, const string& name) : m_type(type), m_name(name)
{
    if(m_type == nullptr) { m_type = Type::invalid; }
}

// destructor
Field::~Field()
{
    delete m_type;
    delete m_default_value;
}

// as_molecular returns this as a MolecularField if it is molecular, or nullptr otherwise.
MolecularField *Field::as_molecular()
{
    return nullptr;
}
const MolecularField *Field::as_molecular() const
{
    return nullptr;
}

// set_name sets the name of this field.  Returns false if a field with
//     the same name already exists in the containing struct.
bool Field::set_name(const string& name)
{
    // Check to make sure no other fields in our struct have this name
    if(m_struct != nullptr && m_struct->field_by_name(name) != nullptr) {
        return false;
    }

    m_name = name;
    return true;
}

// set_type sets the distributed type of the field and clear's the default value.
void Field::set_type(Type *type)
{
    m_type = type;

    // Reset our default value, if we had one
    if(m_default_value != nullptr) {
        delete m_default_value;
        m_default_value = nullptr;
    }
}

// set_default_value establishes a default value for this field.
//     Returns false if the value is invalid for the field.
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

// set_id sets the unique index number associated with the field.
void Field::set_id(unsigned int id)
{
    m_id = id;
}

// set_struct sets a pointer to the struct containing the field.
void Field::set_struct(Struct *strct)
{
    m_struct = strct;
}


} // close namespace bamboo
