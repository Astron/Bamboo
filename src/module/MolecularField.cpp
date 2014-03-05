// Filename: MolecularField.cpp
#include "MolecularField.h"
#include "bits/buffers.h"
#include "module/Class.h"
namespace bamboo { // open namespace bamboo


// constructor
MolecularField::MolecularField(Class *cls, const std::string& name) :
    Field(cls, name), Struct(cls->get_module()) {
    Field::m_type = this;
}

// destructor
MolecularField::~MolecularField() {
}

// as_molecular returns this as a MolecularField if it is molecular, or nullptr otherwise.
MolecularField *MolecularField::as_molecular() {
    return this;
}
const MolecularField *MolecularField::as_molecular() const {
    return this;
}

// add_field adds a new Field as part of the Molecular.
//     Returns false if the field could not be added.
bool MolecularField::add_field(Field *field) {
    // Moleculars cannot be nested
    if(field->as_molecular()) {
        return false;
    }

    // Copy our keywords from the first add field
    if(m_fields.size() == 0) {
        copy_keywords(*field);
    }

    // Each field has to have the same set of keywords
    else if(!has_matching_keywords(*field)) {
        return false;
    }

    // Add the field
    m_fields.push_back(field);

    // See if we still have a fixed byte size.
    if(has_fixed_size() || m_fields.size() == 1) {
        if(field->get_type()->has_fixed_size()) {
            m_size += field->get_type()->get_size();
        } else {
            m_size = 0;
        }
    }

    if(has_default_value() || m_fields.size() == 1) {
        // If any atomic field of the molecular has a default value,
        // the molecular is also considerd to have a default value.
        if(field->has_default_value()) {
            m_has_default_value = true;
        }

        pack_value(field->get_default_value(), m_default_value);
    }

    return true;
}

bool MolecularField::set_default_value(const std::string&) {
    // MolecularField default values are implict from their
    // atomic components and cannot be defined manually.
    return false;
}


} // close namespace bamboo
