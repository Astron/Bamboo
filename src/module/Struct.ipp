// Filename: Struct.ipp
namespace bamboo { // open namespace


// id() returns a unique index number associated with this struct.
inline unsigned int Struct::id() const {
    return m_id;
}

// name returns the name of this struct.
inline const std::string& Struct::name() const {
    return m_name;
}

// module returns the Module object that contains the struct.
inline Module *Struct::module() {
    return m_module;
}
inline const Module *Struct::module() const {
    return m_module;
}


// num_fields returns the number of fields in the struct.
inline size_t Struct::num_fields() const {
    return m_fields.size();
}

// get_field returns the <n>th field of the struct or nullptr if out-of-range.
inline Field *Struct::get_field(unsigned int n) {
    return m_fields.at(n);
}
inline const Field *Struct::get_field(unsigned int n) const {
    return m_fields.at(n);
}

// field_by_id returns the field with the index <id>, or nullptr if no such field exists.
inline Field *Struct::field_by_id(unsigned int id) {
    auto it = m_fields_by_id.find(id);
    if(it == m_fields_by_id.end()) {
        return nullptr;
    }
    return it->second;
}
inline const Field *Struct::field_by_id(unsigned int id) const {
    auto it = m_fields_by_id.find(id);
    if(it == m_fields_by_id.end()) {
        return nullptr;
    }
    return it->second;
}

// field_by_name returns the field with <name>, or nullptr if no such field exists.
inline Field *Struct::field_by_name(const std::string& name) {
    auto it = m_fields_by_name.find(name);
    if(it == m_fields_by_name.end()) {
        return nullptr;
    }
    return it->second;
}
inline const Field *Struct::field_by_name(const std::string& name) const {
    auto it = m_fields_by_name.find(name);
    if(it == m_fields_by_name.end()) {
        return nullptr;
    }
    return it->second;
}

// set_id sets the index number associated with this struct.
inline void Struct::set_id(unsigned int id) {
    m_id = id;
}


} // close namespace bamboo
