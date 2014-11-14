// Filename: Module.cpp
#include "Module.h"
#include "../module/Type.h"
#include "../module/Class.h"
#include "../module/Field.h"
namespace bamboo
{


bool Module::add_class(std::unique_ptr<Class> cls)
{
    Class *ref = cls.get();

    // Classes have to have a name
    if(cls->name().empty()) { return false; }

    // A Class can't share a name with any other type.
    bool inserted = m_types_by_name.emplace(cls->name(), ref).second;
    if(!inserted) { return false; }

    // Transfer ownership of the Class to the module
    cls->set_id((unsigned int)m_types_by_id.size());
    m_types_by_id.push_back(ref);
    m_classes.push_back(move(cls));

    return true;
}

bool Module::add_struct(std::unique_ptr<Struct> container)
{
    Struct *ref = container.get();

    // Structs have to have a name
    if(container->name().empty()) { return false; }

    // A Struct can't share a name with any other type.
    bool inserted = m_types_by_name.emplace(container->name(), ref).second;
    if(!inserted) { return false; }

    // Transfer ownership of the Struct to the Module
    container->set_id((unsigned int)m_types_by_id.size());
    m_types_by_id.push_back(ref);
    m_structs.push_back(move(container));

    return true;
}

bool Module::add_typedef(const std::string& name, Type *type)
{
    // Typedefs can't use the empty string as a name
    if(name.empty()) { return false; }

    // A type alias can't share a name with any other type.
    return m_types_by_name.emplace(name, type).second;
}

bool Module::add_keyword(const std::string& name)
{
    bool inserted = m_keywords_by_name.insert(name).second;
    if(inserted) { m_keywords.push_back(name); }
    return inserted;
}

void Module::add_import(std::unique_ptr<Import> import)
{
    // TODO: Combine duplicates
    m_imports.push_back(move(import));
}

void Module::add_field(Field *field)
{
    field->set_id((unsigned int)m_fields_by_id.size());
    m_fields_by_id.push_back(field);
}


} // close namespace bamboo
