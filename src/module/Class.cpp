// Filename: Class.cpp
#include "Class.h"
#include "../module/Module.h"
#include "../module/Field.h"
using namespace std;
namespace bamboo   // open namespace
{


// constructor
Class::Class(Module *module, const string& name) : Struct(module, name) {}

// as_class returns this Struct as a Class if it is a Class, or nullptr otherwise.
Class *Class::as_class()
{
    return this;
}
const Class *Class::as_class() const
{
    return this;
}

// add_parent adds a new parent to the inheritance hierarchy of the class.
//     Note: This is normally called only during parsing.
bool Class::add_parent(Class *parent)
{
    if(parent == nullptr) { return false; }

    parent->add_child(this);
    m_parents.push_back(parent);

    // We know there will be this many fields, so allocate ahead of time
    const vector<Field *>& parent_fields = parent->m_fields;
    m_fields.reserve(m_fields.size() + parent_fields.size());

    // Add all of the parents fields
    for(auto it = parent_fields.begin(); it != parent_fields.end(); ++it) {
        add_inherited_field(parent, *it);
    }

    return true;
}

// add_child marks a class as a child of this class.
void Class::add_child(Class *child)
{
    m_children.push_back(child);
}

// add_field adds the newly-allocated field to the class.  The class becomes
//     the owner of the pointer and will delete it when it destructs.
//     Returns true if the field is successfully added, or false if the field cannot be added.
bool Class::add_field(unique_ptr<Field> field)
{
    Field *ref = field.get();

    // Field can't be null
    if(field == nullptr) {
        return false;
    }

    // Classes can't share fields.
    if(field->record() != nullptr && field->record() != this) {
        return false;
    }

    // Class fields must have names
    if(field->name().empty()) {
        return false;
    }

    // If the field has the same name as the class, it is a constructor
    if(field->name() == m_name) {
        // Make sure we don't already have a constructor
        if(m_constructor != nullptr) {
            return false;
        }

        // The constructor must be an atomic field.
        if(field->as_molecular()) {
            return false;
        }

        // The constructor has to be the first declared field.
        //     Note: This is the case because the constructor should
        //           always have the earliest field id.
        if(m_owned_fields.size() > 0) {
            return false;
        }

        // Add the field to the field dictionaries
        m_module->add_field(ref);
        m_fields_by_id[field->id()] = ref;
        m_fields_by_name[field->name()] = ref;

        // Transfer ownership of the Field to the Class
        field->set_struct(this);
        m_constructor = move(field);

        return true;
    }

    // Add it to the set of our base field names
    bool inserted = m_base_names.insert(field->name()).second;
    // Fail if there is a name conflict
    if(!inserted) {
        return false;
    }

    // If a parent has a field with the same name, shadow it
    auto prev_field = m_fields_by_name.find(field->name());
    if(prev_field != m_fields_by_name.end()) {
        shadow_field(prev_field->second);
    }

    // Add the field to our full field list
    m_fields.push_back(ref); // Don't have to try to sort; id is always last

    // Add the field to the field dictionaries
    m_module->add_field(ref);
    m_fields_by_id[field->id()] = ref;
    m_fields_by_name[field->name()] = ref;
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
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        (*it)->add_inherited_field(this, ref);
    }

    // Transfer ownership of the Field to the Class
    field->set_struct(this);
    m_owned_fields.push_back(move(field));

    return true;
}

// add_inherited_field updates a classes's fields after a parent adds a new field.
void Class::add_inherited_field(Class *parent, Field *field)
{
    // If the field name matches any base field, it is shadowed.
    if(m_base_names.find(field->name()) != m_base_names.end()) {
        return;
    }

    // If another superclass provides a field with that name, the first parent takes precedence
    auto prev_field = m_fields_by_name.find(field->name());
    if(prev_field != m_fields_by_name.end()) {
        Struct *parentB = prev_field->second->record();
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
    m_fields_by_id[field->id()] = field;
    m_fields_by_name[field->name()] = field;

    // Add the field to the list of fields, sorted by id
    if(m_fields.size() == 0) {
        m_fields.push_back(field);
        m_indices_by_name[field->name()] = 1;
    } else {
        unsigned int index = (unsigned int)m_fields.size() - 1;
        // Note: Iterate in reverse because fields added later are more likely to be at the end
        for(auto it = m_fields.rbegin(); it != m_fields.rend(); ++it) {
            if((*it)->id() < field->id()) {
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
        if(child->field_by_id(field->id()) == field) {
            child->shadow_field(field);
        }
    }
}


} // close namespace bamboo
