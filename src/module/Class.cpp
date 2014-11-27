// Filename: Class.cpp
#include "Class.h"
#include "../module/Module.h"
#include "../module/Field.h"
using namespace std;
namespace bamboo
{


Class *Class::as_class()
{
    return this;
}

const Class *Class::as_class() const
{
    return this;
}

bool Class::add_parent(Class *parent)
{
    if(parent == nullptr) { return false; }

    parent->add_child(this);
    m_parents.push_back(parent);

    // We know there will be this many fields, so allocate ahead of time
    const vector<Field *>& parent_fields = parent->m_fields;
    m_fields.reserve(m_fields.size() + parent_fields.size());

    for(auto it = parent_fields.begin(); it != parent_fields.end(); ++it) {
        add_inherited_field(parent, *it);
    }

    return true;
}

void Class::add_child(Class *child)
{
    m_children.push_back(child);
}

Field *Class::add_field(const string& name, Type *type, bool field_owns_type)
{
    // Class fields must have names
    if(name.empty()) { return nullptr; }

    // Declared fields must have unique names
    if(m_declared_names.find(name) != m_declared_names.end()) { return nullptr; }
    m_declared_names.emplace(name);

    // Actually add Field to Class
    Field *field = new Field(name, type, field_owns_type);
    add_declared_field(field);
    return field;
}

bool Class::add_field(Field *field)
{
    // Classes can't share fields, and can't re-add field to this
    if(field->container() != nullptr) { return false; }

    // Class fields must have names
    if(field->name().empty()) { return false; }

    // Declared fields must have unique names
    bool inserted = m_declared_names.insert(field->name()).second;
    if(!inserted) { return false; }

    // Actually add field to class
    add_declared_field(field);
    return true;
}

// add_declared_field is called by the add_field overloads to actually add the field to the class.
void Class::add_declared_field(Field *field)
{
    // If a parent has a field with the same name, shadow it
    auto prev_field = m_fields_by_name.find(field->name());
    if(prev_field != m_fields_by_name.end()) {
        shadow_field(prev_field->second);
    }

    // Keep m_fields is sorted, a new field will always be last
    m_fields.push_back(field);
    if(m_module != nullptr) {
        m_module->register_field(field);
        m_fields_by_id[field->id()] = field;
    }
    m_fields_by_name[field->name()] = field;
    m_indices_by_name[field->name()] = (unsigned int)m_fields.size() - 1;

    // Update our size
    if(field->as_molecular() == nullptr
       && (has_fixed_size() || m_fields.size() == 1)) {
        if(field->type()->has_fixed_size()) {
            m_size += field->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    // Tell our children about the new field
    for(Class *child : m_children) {
        child->add_inherited_field(this, field);
    }

    // Transfer ownership of the Field to the Class
    field->m_struct = this;
    m_declared_fields.push_back(unique_ptr<Field>(field));
}

// add_inherited_field updates a classes's fields after a parent adds a new field.
void Class::add_inherited_field(Class *parent, Field *field)
{
    // If the field name matches any declared field, it is shadowed.
    if(m_declared_names.find(field->name()) != m_declared_names.end()) {
        return;
    }

    // If another superclass provides a field with that name, the first parent takes precedence
    auto prev_field = m_fields_by_name.find(field->name());
    if(prev_field != m_fields_by_name.end()) {
        Struct *parentB = prev_field->second->container();
        for(auto it = m_parents.begin(); it != m_parents.end(); ++it) {
            if((*it) == parentB) {
                // The early parent's field takes precedence over the new field
                return;
            } else if((*it) == parent) {
                // This parent was added before the later parent, so shadow its field
                shadow_field(prev_field->second);
            }
        }
    }

    // Add the field to our lookup tables
    m_fields_by_name[field->name()] = field;
    if(field->id() >= 0) {
        m_fields_by_id[field->id()] = field;
    }

    // Add the field to the list of fields, sorted by id
    if(m_fields.size() == 0) {
        m_fields.push_back(field);
        m_indices_by_name[field->name()] = 1;
    } else if(field->id() == -1) {

    } else {
        // Note: Iterate in reverse because fields added later are more likely to have higher ids
        unsigned int index = (unsigned int)m_fields.size() - 1;
        for(auto it = m_fields.rbegin(); it != m_fields.rend(); ++it) {

            // Note: Compare ids as unsigned integers, because a field without an id
            //       has the value '-1' and we want these fields to be last because they
            //       are less likely to need sorting later (ids increase monotonically).
            if((unsigned int)(*it)->id() < (unsigned int)field->id()) {
                m_fields.insert(it.base(), field);
                m_indices_by_name[field->name()] = index;
                break;
            }
            index -= 1;
        }
    }

    // Update our size
    if(has_fixed_size() || m_fields.size() == 1) {
        if(field->type()->has_fixed_size()) {
            m_size += field->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    // Tell our children about the new field
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        (*it)->add_inherited_field(this, field);
    }
}

// shadow_field removes the field from all of the Class's field accessors,
//     so that another field with the same name can be inserted.
void Class::shadow_field(Field *field)
{
    if(has_fixed_size()) {
        m_size -= field->type()->fixed_size();
    }

    m_fields_by_id.erase(field->id());
    m_fields_by_name.erase(field->name());
    m_indices_by_name.erase(field->name());
    for(auto it = m_fields.begin(); it != m_fields.end(); ++it) {
        if((*it) == field) {
            m_fields.erase(it);
            break;
        }
    }

    // Tell our children to shadow the field
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        Class *child = (*it);
        if(child->field_by_name(field->name()) == field) {
            child->shadow_field(field);
        }
    }
}


} // close namespace bamboo
