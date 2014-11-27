// Filename: Value.ipp
namespace bamboo
{ // open namespace


inline const Type *Value::type() const
{
    return m_type;
}

inline Number Value::as_numeric() const
{
    if(m_type->subtype() != Subtype_Numeric) { throw InvalidCast(m_type, Subtype_Numeric); }
    return m_numeric;
}

inline const std::string& Value::as_string() const
{
    if(m_type->subtype() != Subtype_String) { throw InvalidCast(m_type, Subtype_String); }
    return m_string;
}

inline const std::vector<uint8_t>& Value::as_blob() const
{
    if(m_type->subtype() != Subtype_Blob) { throw InvalidCast(m_type, Subtype_Blob); }
    return m_blob;
}

inline const Value::array_t& Value::as_array() const
{
    if(m_type->subtype() != Subtype_Array) { throw InvalidCast(m_type, Subtype_Array); }
    return m_array;
}

inline const Value::struct_t& Value::as_struct() const
{
    if(m_type->subtype() != Subtype_Struct) { throw InvalidCast(m_type, Subtype_Struct); }
    return m_struct;
}

inline const Value::method_t& Value::as_method() const
{
    if(m_type->subtype() != Subtype_Method) { throw InvalidCast(m_type, Subtype_Method); }
    return m_method;
}


} // close namespace bamboo
