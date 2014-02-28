// Filename: Class.cpp
#include "Class.h"
#include "module/Module.h"
#include "module/Field.h"
using namespace std;
namespace bamboo { // open namespace


// constructor
Class::Class(Module *module, const string& name) : Struct(module, name), m_constructor(NULL) {}

// destructor
Class::~Class() {
    if(m_constructor != (Field *)NULL) {
        delete m_constructor;
    }
}

// as_class returns this Struct as a Class if it is a Class, or NULL otherwise.
Class *Class::as_class() {
    return this;
}
const Class *Class::as_class() const {
    return this;
}

// add_parent adds a new parent to the inheritance hierarchy of the class.
//     Note: This is normally called only during parsing.
void Class::add_parent(Class *parent) {
    parent->add_child(this);
    m_parents.push_back(parent);

    // We know there will be this many fields, so allocate ahead of time
    const vector<Field *>& parent_fields = parent->m_fields;
    m_fields.reserve(m_fields.size() + parent_fields.size());

    // Add all of the parents fields
    for(auto it = parent_fields.begin(); it != parent_fields.end(); ++it) {
        add_inherited_field(parent, *it);
    }
}

// add_child marks a class as a child of this class.
void Class::add_child(Class *child) {
    m_children.push_back(child);
}

// add_field adds the newly-allocated field to the class.  The class becomes
//     the owner of the pointer and will delete it when it destructs.
//     Returns true if the field is successfully added, or false if the field cannot be added.
bool Class::add_field(Field *field) {
    // Field can't be null
    if(field == (Field *)NULL) {
        return false;
    }

    // Classes can't share fields.
    if(field->get_struct() != NULL && field->get_struct() != this) {
        return false;
    }

    // Class fields must have names
    if(field->get_name().empty()) {
        return false;
    }

    // If the field has the same name as the class, it is a constructor
    if(field->get_name() == m_name) {
        // Make sure we don't already have a constructor
        if(m_constructor != (Field *)NULL) {
            return false;
        }

        // The constructor must be an atomic field.
        if(field->as_molecular()) {
            return false;
        }

        // The constructor has to be the first declared field.
        //     Note: This is the case because the constructor should
        //           always have the earliest field id.
        if(m_base_fields.size() > 0) {
            return false;
        }

        field->set_struct(this);
        m_constructor = field;

        m_module->add_field(field);
        m_fields_by_id[field->get_id()] = field;
        m_fields_by_name[field->get_name()] = field;
        return true;
    }

    // Try to add the field
    bool inserted = m_base_fields_by_name.insert(
                        unordered_map<string, Field *>::value_type(field->get_name(), field)).second;
    // Fail if there is a name conflict
    if(!inserted) {
        return false;
    }
    m_base_fields.push_back(field);

    // If a parent has a field with the same name, shadow it
    auto prev_field = m_fields_by_name.find(field->get_name());
    if(prev_field != m_fields_by_name.end()) {
        shadow_field(prev_field->second);
    }

    // Add the field to our full field list
    field->set_struct(this);
    m_fields.push_back(field); // Don't have to try to sort; id is always last

    // Add the field to the lookups
    m_module->add_field(field);
    m_fields_by_id[field->get_id()] = field;
    m_fields_by_name[field->get_name()] = field;

    // Update our size
    if(field->as_molecular() == (MolecularField *)NULL
            && (has_fixed_size() || m_fields.size() == 1)) {
        if(field->get_type()->has_fixed_size()) {
            m_size += field->get_type()->get_size();
        } else {
            m_size = 0;
        }
    }

    // Tell our children about the new field
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        (*it)->add_inherited_field(this, field);
    }

    return true;
}

// add_inherited_field updates a classes's fields after a parent adds a new field.
void Class::add_inherited_field(Class *parent, Field *field) {
    // If the field name matches any base field, it is shadowed.
    if(m_base_fields_by_name.find(field->get_name()) != m_base_fields_by_name.end()) {
        return;
    }

    // If another superclass provides a field with that name, the first parent takes precedence
    auto prev_field = m_fields_by_name.find(field->get_name());
    if(prev_field != m_fields_by_name.end()) {
        Struct *parentB = prev_field->second->get_struct();
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
    m_fields_by_id[field->get_id()] = field;
    m_fields_by_name[field->get_name()] = field;

    // Add the field to the list of fields, sorted by id
    if(m_fields.size() == 0) {
        m_fields.push_back(field);
    } else {
        // Note: Iterate in reverse because fields added later are more likely to be at the end
        for(auto it = m_fields.rbegin(); it != m_fields.rend(); ++it) {
            if((*it)->get_id() < field->get_id()) {
                m_fields.insert(it.base(), field);
                break;
            }
        }
    }

    // Update our size
    if(has_fixed_size() || m_fields.size() == 1) {
        if(field->get_type()->has_fixed_size()) {
            m_size += field->get_type()->get_size();
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
void Class::shadow_field(Field *field) {
    if(has_fixed_size()) {
        m_size -= field->get_type()->get_size();
    }

    m_fields_by_id.erase(field->get_id());
    m_fields_by_name.erase(field->get_name());
    for(auto it = m_fields.begin(); it != m_fields.end(); ++it) {
        if((*it) == field) {
            m_fields.erase(it);
            break;
        }
    }

    // Tell our children to shadow the field
    for(auto it = m_children.begin(); it != m_children.end(); ++it) {
        Class *child = (*it);
        if(child->get_field_by_id(field->get_id()) == field) {
            child->shadow_field(field);
        }
    }
}


} // close namespace bamboo
