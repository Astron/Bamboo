// Filename: Module.cpp
#include "Module.h"
#include "../module/Type.h"
#include "../module/Class.h"
#include "../module/Field.h"
namespace bamboo   // open namespace
{


typedef std::unordered_map<std::string, Type *>::value_type TypeName;

// constructor
Module::Module()
{
}

// class_by_id returns the requested class or nullptr if there is no such class.
Class *Module::class_by_id(unsigned int id)
{
    Type *dt = type_by_id(id);
    if(dt == nullptr) {
        return nullptr;
    }
    if(dt->as_struct() == nullptr) {
        return nullptr;
    }
    return dt->as_struct()->as_class();
}
const Class *Module::class_by_id(unsigned int id) const
{
    const Type *dt = type_by_id(id);
    if(dt == nullptr) {
        return nullptr;
    }
    if(dt->as_struct() == nullptr) {
        return nullptr;
    }
    return dt->as_struct()->as_class();
}
// class_by_name returns the requested class or nullptr if there is no such class.
Class *Module::class_by_name(const std::string& name)
{
    Type *dt = type_by_name(name);
    if(dt == nullptr) {
        return nullptr;
    }
    if(dt->as_struct() == nullptr) {
        return nullptr;
    }
    return dt->as_struct()->as_class();
}
const Class *Module::class_by_name(const std::string& name) const
{
    const Type *dt = type_by_name(name);
    if(dt == nullptr) {
        return nullptr;
    }
    if(dt->as_struct() == nullptr) {
        return nullptr;
    }
    return dt->as_struct()->as_class();
}

// add_class adds the newly-allocated class to the module.
//     Returns false if there is a name conflict.
//     The Module becomes the owner of the pointer and will delete it when it destructs.
bool Module::add_class(std::unique_ptr<Class> cls)
{
    Class *ref = cls.get();

    // Classes have to have a name
    if(cls->name().empty()) {
        return false;
    }

    // A Class can't share a name with any other type.
    bool inserted = m_types_by_name.insert(TypeName(cls->name(), ref)).second;
    if(!inserted) {
        return false;
    }

    cls->set_id(m_types_by_id.size());
    m_types_by_id.push_back(ref);
    m_classes.push_back(move(cls));
    return true;
}

// add_struct adds the newly-allocated struct to the module.
//     Returns false if there is a name conflict.
//     The Module becomes the owner of the pointer and will delete it when it destructs.
bool Module::add_struct(std::unique_ptr<Struct> record)
{
    Struct *ref = record.get();

    // Structs have to have a name
    if(record->name().empty()) {
        return false;
    }

    // A Struct can't share a name with any other type.
    bool inserted = m_types_by_name.insert(TypeName(record->name(), ref)).second;
    if(!inserted) {
        return false;
    }

    record->set_id(m_types_by_id.size());
    m_types_by_id.push_back(ref);
    m_structs.push_back(move(record));
    return true;
}

// add_typedef adds the alias <name> to the module for the type <type>.
//     Returns false if there is a name conflict.
bool Module::add_typedef(const std::string& name, Type *type)
{
    // Typedefs can't use the empty string as a name
    if(name.empty()) {
        return false;
    }

    // A type alias can't share a name with any other type.
    return m_types_by_name.insert(TypeName(name, type)).second;
}

// add_import adds a newly-allocated import to the module.
//     Imports with duplicate modules are combined.
void Module::add_import(std::unique_ptr<Import> import)
{
    // TODO: Combine duplicates
    m_imports.push_back(move(import));
}

// add_keyword adds a keyword with the name <keyword> to the list of declared keywords.
void Module::add_keyword(const std::string& keyword)
{
    if(!has_keyword(keyword)) {
        m_keywords.push_back(keyword);
    }
}

// add_field gives the field a unique id within the module.
void Module::add_field(Field *field)
{
    field->set_id((unsigned int)m_fields_by_id.size());
    m_fields_by_id.push_back(field);
}


} // close namespace bamboo
