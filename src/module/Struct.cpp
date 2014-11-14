// Filename: Struct.cpp
#include "Struct.h"
#include "../module/Module.h"
#include "../module/Field.h"
using namespace std;
namespace bamboo
{


Struct::Struct(Module *module, const string& name) : m_module(module), m_name(name)
{
    m_subtype = kTypeStruct;
}

Struct::Struct(Module *module) : m_module(module)
{
    m_subtype = kTypeStruct;
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

bool Struct::add_field(std::unique_ptr<Field> field)
{
    Field *ref = field.get();

    // Field must not be null
    if(field == nullptr) {
        return false;
    }

    // Structs can't share a field
    if(field->container() != nullptr && field->container() != this) {
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

    if(!field->name().empty()) {
        // Struct fields must have unique names
        bool inserted = m_fields_by_name.emplace(field->name(), ref).second;
        if(!inserted) { return false; }
        m_indices_by_name[field->name()] = (unsigned int)m_fields.size();
    }

    m_fields.push_back(ref);
    m_module->add_field(ref); // Module give us an id
    m_fields_by_id.emplace(field->id(), ref);
    if(has_fixed_size() || m_fields.size() == 1) {
        if(field->type()->has_fixed_size()) {
            m_size += field->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    // Transfer ownership of the Field to the Struct
    field->set_struct(this);
    m_owned_fields.push_back(move(field));

    return true;
}


} // close namespace bamboo
