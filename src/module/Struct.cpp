// Filename: Struct.cpp
#include "Struct.h"
#include "../module/Module.h"
#include "../module/Field.h"
using namespace std;
namespace bamboo   // open namespace bamboo
{

// public constructor
Struct::Struct(Module *module, const string& name) : m_module(module), m_id(0), m_name(name)
{
    m_subtype = kTypeStruct;
}

// protected constructor
Struct::Struct(Module *module) : m_module(module), m_id(0)
{
    m_subtype = kTypeStruct;
}

// as_struct returns this as a Struct if it is a Struct, or nullptr otherwise.
Struct *Struct::as_struct()
{
    return this;
}
const Struct *Struct::as_struct() const
{
    return this;
}

// as_class returns this Struct as a Class if it is a Class, or nullptr otherwise.
Class *Struct::as_class()
{
    return nullptr;
}
const Class *Struct::as_class() const
{
    return (const Class *)nullptr;
}

// add_field adds a new Field to the struct.
bool Struct::add_field(std::unique_ptr<Field> field)
{
    Field *ref = field.get();

    // Field must not be null
    if(field == nullptr) {
        return false;
    }

    // Structs can't share a field
    if(field->record() != nullptr && field->record() != this) {
        return false;
    }

    // Structs can't have moleculars
    if(field->as_molecular()) {
        return false;
    }
    // Structs can't have methods
    if(field->type()->as_method()) {
        return false;
    }

    // Struct fields are accessible by name
    if(!field->name().empty()) {
        // Structs can't have Constructors
        if(field->name() == m_name) {
            return false;
        }

        // Try to add the field
        bool inserted = m_fields_by_name.insert(
                            unordered_map<string, Field *>::value_type(field->name(), ref)).second;
        if(!inserted) {
            // But the field had a name conflict
            return false;
        }
        // The index of this named field is the next available index
        m_indices_by_name[field->name()] = (unsigned int)m_fields.size();
    }

    // Struct fields are accessible by id.
    m_module->add_field(ref);
    m_fields_by_id.insert(unordered_map<int, Field *>::value_type(field->id(), ref));

    // Add it to the accessible list of fields
    m_fields.push_back(ref);
    if(has_fixed_size() || m_fields.size() == 1) {
        if(field->type()->has_fixed_size()) {
            m_size += field->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    // Transfer ownership of the Field to the Struct
    m_owned_fields.push_back(move(field));

    return true;
}


} // close namespace bamboo
