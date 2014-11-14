// Filename: Struct.ipp
namespace bamboo
{


inline unsigned int Struct::id() const
{
    return m_id;
}

inline const std::string& Struct::name() const
{
    return m_name;
}

inline Module *Struct::module()
{
    return m_module;
}

inline const Module *Struct::module() const
{
    return m_module;
}

inline size_t Struct::num_fields() const
{
    return m_fields.size();
}

inline Field *Struct::nth_field(unsigned int n)
{
    return n < m_fields.size() ? m_fields[n] : nullptr;
}

inline const Field *Struct::nth_field(unsigned int n) const
{
    return n < m_fields.size() ? m_fields[n] : nullptr;
}

inline Field *Struct::field_by_id(unsigned int id)
{
    auto it = m_fields_by_id.find(id);
    return it == m_fields_by_id.end() ? nullptr : it->second;
}

inline const Field *Struct::field_by_id(unsigned int id) const
{
    auto it = m_fields_by_id.find(id);
    return it == m_fields_by_id.end() ? nullptr : it->second;
}

inline Field *Struct::field_by_name(const std::string& name)
{
    auto it = m_fields_by_name.find(name);
    return it == m_fields_by_name.end() ? nullptr : it->second;
}

inline const Field *Struct::field_by_name(const std::string& name) const
{
    auto it = m_fields_by_name.find(name);
    return it == m_fields_by_name.end() ? nullptr : it->second;
}

inline void Struct::set_id(unsigned int id)
{
    m_id = id;
}


} // close namespace bamboo
