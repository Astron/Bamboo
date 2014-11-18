// Filename: MolecularField.cpp
#include "MolecularField.h"
#include "../bits/buffers.h"
#include "../module/Class.h"
#include "../module/Value.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{


MolecularField::MolecularField(const string& name) :
    Field(name, Type::None, false), Struct("molecular_" + name)
{
    Field::m_type = this;
}

MolecularField *MolecularField::as_molecular()
{
    return this;
}

const MolecularField *MolecularField::as_molecular() const
{
    return this;
}

bool MolecularField::add_atomic(Field *field)
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


} // close namespace bamboo
