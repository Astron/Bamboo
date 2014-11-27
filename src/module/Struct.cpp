// Filename: Struct.cpp
#include "Struct.h"
#include "../module/Module.h"
#include "../module/Field.h"
using namespace std;
namespace bamboo
{


Struct::Struct(const std::string& name) :
    Type(kTypeStruct),
    m_name(name)
{
}

Struct::Struct(Module *module, const std::string& name, int id) :
    Type(kTypeStruct),
    m_id(id),
    m_module(module),
    m_name(name)
{
}

Struct *Struct::as_struct()
{
    return this;
}

const Struct *Struct::as_struct() const
{
    return this;
}

Class *Struct::as_class()
{
    return nullptr;
}

const Class *Struct::as_class() const
{
    return nullptr;
}

Field *Struct::add_field(const string& name, Type *type, bool field_owns_type)
{
    // Structs can't have methods
    if(type->subtype() == kTypeMethod) { return nullptr; }

    // Struct fields must have unique names
    if(m_fields_by_name.find(name) != m_fields_by_name.end()) { return nullptr; }

    // Actually add field to struct
    Field *field = new Field(name, type, field_owns_type);
    if(!name.empty()) {
        m_fields_by_name.emplace(name, field);
        m_indices_by_name.emplace(name, (unsigned int)m_fields.size());
    }

    return field;
}

bool Struct::add_field(Field *field)
{
    // Structs can't share a field, and can't re-add field to this
    if(field->container() != nullptr) {
        return false;
    }

    // Structs can't have moleculars
    if(field->as_molecular()) {
        return false;
    }

    // Structs can't have methods
    if(field->type()->subtype() == kTypeMethod) {
        return false;
    }

    // Struct fields must have unique names
    if(m_fields_by_name.find(field->name()) != m_fields_by_name.end()) { return false; }

    // Actually add field to struct
    if(!field->name().empty()) {
        m_fields_by_name.emplace(field->name(), field);
        m_indices_by_name.emplace(field->name(), (unsigned int)m_fields.size());
    }


    return true;
}

void Struct::add_declared_field(Field *field)
{
    m_fields.push_back(field);
    if(m_module != nullptr) {
        // Get a module-wide id
        m_module->register_field(field);
        m_fields_by_id.emplace(field->id(), field);
    }

    // Update struct size
    if(has_fixed_size() || m_fields.size() == 1) {
        if(field->type()->has_fixed_size()) {
            m_size += field->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    // Transfer ownership of the Field to the Struct
    field->m_struct = this;
    m_declared_fields.push_back(unique_ptr<Field>(field));
}

void Struct::update_field_id(Field *field, int id)
{
    // TODO: Implement
}

void Struct::update_field_type(Field *field, Type *new_type, Type *old_type)
{
    // TODO: Implement
}


} // close namespace bamboo
