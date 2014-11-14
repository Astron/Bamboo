// Filename: Value.ipp
namespace bamboo
{ // open namespace


inline const Type *Value::type() const
{
    return m_type;
}

inline char Value::as_char() const
{
    if(m_type->subtype() != kTypeChar) { throw InvalidCast(m_type, kTypeChar); }
    return m_char;
}
inline int8_t Value::as_int8() const
{
    if(m_type->subtype() != kTypeInt8) { throw InvalidCast(m_type, kTypeInt8); }
    return m_int8;
}
inline int16_t Value::as_int16() const
{
    if(m_type->subtype() != kTypeInt16) { throw InvalidCast(m_type, kTypeInt16); }
    return m_int16;
}
inline int32_t Value::as_int32() const
{
    if(m_type->subtype() != kTypeInt32) { throw InvalidCast(m_type, kTypeInt32); }
    return m_int32;
}
inline int64_t Value::as_int64() const
{
    if(m_type->subtype() != kTypeInt64) { throw InvalidCast(m_type, kTypeInt64); }
    return m_int64;
}
inline uint8_t Value::as_uint8() const
{
    if(m_type->subtype() != kTypeUint8) { throw InvalidCast(m_type, kTypeUint8); }
    return m_uint8;
}
inline uint16_t Value::as_uint16() const
{
    if(m_type->subtype() != kTypeUint16) { throw InvalidCast(m_type, kTypeUint16); }
    return m_uint16;
}
inline uint32_t Value::as_uint32() const
{
    if(m_type->subtype() != kTypeUint32) { throw InvalidCast(m_type, kTypeUint32); }
    return m_uint32;
}
inline uint64_t Value::as_uint64() const
{
    if(m_type->subtype() != kTypeUint64) { throw InvalidCast(m_type, kTypeUint64); }
    return m_uint64;
}
inline float Value::as_float32() const
{
    if(m_type->subtype() != kTypeFloat32) { throw InvalidCast(m_type, kTypeFloat32); }
    return m_float32;
}
inline double Value::as_float64() const
{
    if(m_type->subtype() != kTypeFloat64) { throw InvalidCast(m_type, kTypeFloat64); }
    return m_float64;
}
inline double Value::as_fixed() const
{
    if(m_type->subtype() != kTypeFixed) { throw InvalidCast(m_type, kTypeFixed); }
    return m_float64;
}
inline const std::string& Value::as_string() const
{
    if(m_type->subtype() != kTypeString) { throw InvalidCast(m_type, kTypeString); }
    return m_string;
}
inline const std::vector<uint8_t>& Value::as_blob() const
{
    if(m_type->subtype() != kTypeBlob) { throw InvalidCast(m_type, kTypeBlob); }
    return m_blob;
}
inline const Value::array_t& Value::as_array() const
{
    if(m_type->subtype() != kTypeArray) { throw InvalidCast(m_type, kTypeArray); }
    return m_array;
}
inline const Value::struct_t& Value::as_struct() const
{
    if(m_type->subtype() != kTypeStruct) { throw InvalidCast(m_type, kTypeStruct); }
    return m_struct;
}
inline const Value::method_t& Value::as_method() const
{
    if(m_type->subtype() != kTypeMethod) { throw InvalidCast(m_type, kTypeMethod); }
    return m_method;
}


} // close namespace bamboo
