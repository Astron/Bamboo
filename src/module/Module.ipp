// Filename: Module.ipp
namespace bamboo { // open namespace


inline Import::Import() : module("<undefined>") {}
inline Import::Import(const std::string& module_name) : module(module_name) {}


// num_classes returns the number of classes in the module
inline size_t Module::num_classes() const {
    return m_classes.size();
}
// get_class returns the <n>th class read from the dcfile(s).
inline Class *Module::get_class(unsigned int n) {
    return m_classes.at(n);
}
inline const Class *Module::get_class(unsigned int n) const {
    return m_classes.at(n);
}

// num_structs returns the number of structs in the module.
inline size_t Module::num_structs() const {
    return m_structs.size();
}
// get_struct returns the <n>th struct in the module.
inline Struct *Module::get_struct(unsigned int n) {
    return m_structs.at(n);
}
inline const Struct *Module::get_struct(unsigned int n) const {
    return m_structs.at(n);
}

// num_tyoes returns the number of types in the module.
//     All type ids will be within the range 0 <= id < num_tyoes().
inline size_t Module::num_tyoes() const {
    return m_types_by_id.size();
}

// type_by_id returns the requested type or nullptr if there is no such type.
inline Type *Module::type_by_id(unsigned int id) {
    if(id < m_types_by_id.size()) {
        return m_types_by_id[id];
    }

    return nullptr;
}
inline const Type *Module::type_by_id(unsigned int id) const {
    if(id < m_types_by_id.size()) {
        return m_types_by_id[id];
    }

    return (const Type *)nullptr;
}
// type_by_name returns the requested type or nullptr if there is no such type.
inline Type *Module::type_by_name(const std::string& name) {
    auto type_ref = m_types_by_name.find(name);
    if(type_ref != m_types_by_name.end()) {
        return type_ref->second;
    }

    return nullptr;
}
inline const Type *Module::type_by_name(const std::string& name) const {
    auto type_ref = m_types_by_name.find(name);
    if(type_ref != m_types_by_name.end()) {
        return type_ref->second;
    }

    return (const Type *)nullptr;
}

// field_by_id returns the request field or nullptr if there is no such field.
inline Field *Module::field_by_id(unsigned int id) {
    if(id < m_fields_by_id.size()) {
        return m_fields_by_id[id];
    }

    return nullptr;
}
inline const Field *Module::field_by_id(unsigned int id) const {
    if(id < m_fields_by_id.size()) {
        return m_fields_by_id[id];
    }

    return (const Field *)nullptr;
}

// num_imports returns the number of imports in the module.
inline size_t Module::num_imports() const {
    return m_imports.size();
}
// get_import retuns the <n>th import in the module.
inline Import *Module::get_import(unsigned int n) {
    return m_imports.at(n);
}
inline const Import *Module::get_import(unsigned int n) const {
    return m_imports.at(n);
}

// has_keyword returns true if a keyword with the name <keyword> is declared in the module.
inline bool Module::has_keyword(const std::string& keyword) const {
    for(auto it = m_keywords.begin(); it != m_keywords.end(); ++it) {
        if(*it == keyword) {
            return true;
        }
    }
    return false;
}
// num_keywords returns the number of keywords declared in the module.
inline size_t Module::num_keywords() const {
    return m_keywords.size();
}
// get_keyword returns the <n>th keyword declared in the module.
inline const std::string& Module::get_keyword(unsigned int n) const {
    return m_keywords.at(n);
}


} // close namespace bamboo
