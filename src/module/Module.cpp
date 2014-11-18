// Filename: Module.cpp
#include "Module.h"
#include "../module/Type.h"
#include "../module/Class.h"
#include "../module/Field.h"
using namespace std;
namespace bamboo
{


Class *Module::add_class(const string& name)
{
    return add_class(name.c_str());
}

Class *Module::add_class(const char *name)
{
    // Classes must have a name
    if(name[0] == '\0') { return nullptr; }

    // A Class can't share a name with any other type.
    if(m_types_by_name.find(name) != m_types_by_name.end()) { return nullptr; }

    // Create the class
    Class *class_ = new Class(this, name, m_types_by_id.size());
    m_classes.push_back(class_);
    m_types.insert(class_);
    m_types_by_id.push_back(class_);
    m_types_by_name.emplace(name, class_);
    m_owned_types.push_back(unique_ptr<Type>(class_));
    return class_;
}

Struct *Module::add_struct(const string& name)
{
    return add_struct(name.c_str());
}

bool Module::add_class(Class *class_, bool transfer_ownership)
{
    // TODO: Implement

    // Reject the struct if it has an existing module ptr
    // Otherwise, assign a new id (ignore the existing value)

    // Then also: Set the id of each field declared in the struct
    // Deal with problem of reordering class fields by id
}

Struct *Module::add_struct(const char *name)
{
    // Structs must have a name
    if(name[0] == '\0') { return nullptr; }

    // A Struct can't share a name with any other type.
    if(m_types_by_name.find(name) != m_types_by_name.end()) { return nullptr; }

    // Create the struct
    Struct *struct_ = new Struct(this, name, m_types_by_id.size());
    m_structs.push_back(struct_);
    m_types.insert(struct_);
    m_types_by_id.push_back(struct_);
    m_types_by_name.emplace(name, struct_);
    m_owned_types.push_back(unique_ptr<Type>(struct_));
    return struct_;
}

bool Module::add_struct(Struct *struct_, bool transfer_ownership)
{
    // TODO: Implement

    // Reject the struct if it has an existing module ptr
    // Otherwise, assign a new id (ignore the existing value)

    // Then also: Set the id of each field declared in the struct
}

Import *Module::add_import(const string& pymodule)
{
    return add_import(pymodule.c_str());
}

Import *Module::add_import(const char *pymodule)
{
    // Imports must have a module name
    if(pymodule[0] == '\0') { return nullptr; }

    // Shouldn't create a new Import if an existing import record has the same module
    if(m_imports_by_module.find(pymodule) != m_imports_by_module.end()) { return nullptr; }

    // Create the import
    Import *import = new Import(pymodule);
    m_imports_by_module.emplace(pymodule, import);
    m_imports.push_back(unique_ptr<Import>(import));
    return import;
}

bool Module::add_import(Import *import, bool transfer_ownership)
{
    // TODO: Implement
}

bool Module::add_typedef(const std::string& name, Type *type, bool transfer_ownership)
{
    // Typedefs can't use an empty string as a name
    if(name.empty()) { return false; }

    // A type alias can't share a name with any other type
    bool inserted = m_types_by_name.emplace(name, type).second;
    if(!inserted) { return false; }

    // Add to the set of all of our types
    m_types.insert(type);
    if(transfer_ownership) {
        m_owned_types.push_back(unique_ptr<Type>(type));
    }

    return true;
}

bool Module::add_keyword(const std::string& name)
{
    bool inserted = m_keywords_by_name.insert(name).second;
    if(inserted) { m_keywords.push_back(name); }
    return inserted;
}

void Module::register_field(Field *field)
{
    field->m_id = (int)m_fields_by_id.size();
    m_fields_by_id.push_back(field);
}


} // close namespace bamboo
