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
    add_declared_class(class_);
    return class_;
}

Struct *Module::add_struct(const string& name)
{
    return add_struct(name.c_str());
}

bool Module::add_class(Class *class_)
{
    // Modules can't share classes, and can't re-add class to this
    if(class_->module() != nullptr) { return false; }

    // Classes must have a name
    if(class_->name().empty()) { return false; }

    // A Class can't share a name with any other type.
    if(m_types_by_name.find(class_->name()) != m_types_by_name.end()) { return nullptr; }

    // Set ids for the class and fields
    class_->m_id = m_types_by_id.size();
    // @FIXME(Kevin): If we were to add fields to a class, then add parents that
    // weren't in a module, then add both this and its parents to the module,
    // the fields would be out of order and need to be reordered.
    // In our current use case (parsing from a dcfile) this shouldn't happen.
    for(Field *field : class_->m_fields) {
        this->register_field(field);
        class_->update_field_id(field, field->id());
    }

    add_declared_class(class_);
    return true;
}

Struct *Module::add_struct(const char *name)
{
    // Structs must have a name
    if(name[0] == '\0') { return nullptr; }

    // A Struct can't share a name with any other type.
    if(m_types_by_name.find(name) != m_types_by_name.end()) { return nullptr; }

    // Create the struct
    Struct *struct_ = new Struct(this, name, m_types_by_id.size());
    add_declared_struct(struct_);
    return struct_;
}

bool Module::add_struct(Struct *struct_)
{
    // Modules can't share classes, and can't re-add class to this
    if(struct_->module() != nullptr) { return false; }

    // Classes must have a name
    if(struct_->name().empty()) { return false; }

    // A Class can't share a name with any other type.
    if(m_types_by_name.find(struct_->name()) != m_types_by_name.end()) { return false; }

    // Set ids for the struct and fields
    struct_->m_id = m_types_by_id.size();
    for(Field *field : struct_->m_fields) {
        this->register_field(field);
        struct_->update_field_id(field, field->id());
    }

    add_declared_struct(struct_);
    return true;
}

Import *Module::add_import(const string& pymodule)
{
    return add_import(pymodule.c_str());
}

Import *Module::add_import(const char *pymodule)
{
    if(pymodule[0] == '\0') { return nullptr; }
    Import *import = new Import(pymodule);
    m_imports.push_back(unique_ptr<Import>(import));
    return import;
}

bool Module::add_import(Import *import)
{
    if(import->module.empty()) { return false; }
    m_imports.push_back(unique_ptr<Import>(import));
    return import;
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

void Module::add_declared_class(Class *class_)
{
    m_classes.push_back(class_);
    m_types.insert(class_);
    m_types_by_id.push_back(class_);
    m_types_by_name.emplace(class_->name(), class_);
    m_owned_types.push_back(unique_ptr<Type>(class_));
}

void Module::add_declared_struct(Struct *struct_)
{
    m_structs.push_back(struct_);
    m_types.insert(struct_);
    m_types_by_id.push_back(struct_);
    m_types_by_name.emplace(struct_->name(), struct_);
    m_owned_types.push_back(unique_ptr<Type>(struct_));
}

void Module::register_field(Field *field)
{
    field->m_id = (int)m_fields_by_id.size();
    m_fields_by_id.push_back(field);
}


} // close namespace bamboo
