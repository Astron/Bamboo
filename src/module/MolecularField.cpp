// Filename: MolecularField.cpp
#include "MolecularField.h"
#include "../bits/buffers.h"
#include "module/Class.h"
#include "module/Value.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{


MolecularField::MolecularField(Class *cls, const string& name) :
    Field(cls, name), Struct(cls->module())
{
    Field::m_type = this;
}

// as_molecular returns this as a MolecularField if it is molecular, or nullptr otherwise.
MolecularField *MolecularField::as_molecular()
{
    return this;
}
const MolecularField *MolecularField::as_molecular() const
{
    return this;
}

// add_field adds a new Field as part of the Molecular.
//     Returns false if the field could not be added.
bool MolecularField::add_field(Field *field)
{
    // Moleculars cannot be nested
    if(field->as_molecular()) { return false; }

    // Copy our keywords from the first added field
    // Each field has to have the same set of keywords
    if(m_fields.size() == 0) { copy_keywords(*field); }
    else if(!has_matching_keywords(*field)) { return false; }

    // Add the field
    m_fields.push_back(field);

    // See if we still have a fixed byte size.
    if(has_fixed_size() || m_fields.size() == 1) {
        if(field->type()->has_fixed_size()) {
            m_size += field->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    return true;
}

// add_field with a unique_ptr always returns false; molecular fields don't own fields.
bool MolecularField::add_field(unique_ptr<Field>)
{
    return false;
}

// set_default_value always returns false; molecular fields don't have default values.
bool MolecularField::set_default_value(const Value&)
{
    return false;
}
bool MolecularField::set_default_value(const Value *)
{
    return false;
}


} // close namespace bamboo
