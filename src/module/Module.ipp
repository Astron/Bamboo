// Filename: Module.ipp
namespace bamboo
{


Class *Module::class_by_id(int id)
{
    Type *dt = type_by_id(id);
    if(dt == nullptr) { return nullptr; }
    if(dt->as_struct() == nullptr) { return nullptr; }
    return dt->as_struct()->as_class();
}

const Class *Module::class_by_id(int id) const
{
    const Type *dt = type_by_id(id);
    if(dt == nullptr) { return nullptr; }
    if(dt->as_struct() == nullptr) { return nullptr; }
    return dt->as_struct()->as_class();
}

Class *Module::class_by_name(const std::string& name)
{
    Type *dt = type_by_name(name);
    if(dt == nullptr) { return nullptr; }
    if(dt->as_struct() == nullptr) { return nullptr; }
    return dt->as_struct()->as_class();
}

const Class *Module::class_by_name(const std::string& name) const
{
    const Type *dt = type_by_name(name);
    if(dt == nullptr) { return nullptr; }
    if(dt->as_struct() == nullptr) { return nullptr; }
    return dt->as_struct()->as_class();
}

inline Type *Module::type_by_name(const std::string& name)
{
    auto it = m_types_by_name.find(name);
    return it == m_types_by_name.end() ? nullptr : it->second;
}

inline const Type *Module::type_by_name(const std::string& name) const
{
    auto it = m_types_by_name.find(name);
    return it == m_types_by_name.end() ? nullptr : it->second;
}

inline Import *Module::import_by_module(const std::string& pymodule)
{
    auto it = m_imports_by_module.find(pymodule);
    return it == m_imports_by_module.end() ? nullptr : it->second;
}
inline const Import *Module::import_by_module(const std::string& pymodule) const
{
    auto it = m_imports_by_module.find(pymodule);
    return it == m_imports_by_module.end() ? nullptr : it->second;
}

inline bool Module::has_keyword(const std::string& name)
{
    return m_keywords_by_name.find(name) != m_keywords_by_name.end();
}


} // close namespace bamboo
