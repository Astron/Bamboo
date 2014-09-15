// Filename: Type.ipp
namespace bamboo { // open namespace bamboo


// null constructor
inline Type::Type() :
    m_subtype(kTypeInvalid), m_size(0) {
}

// subtype returns the type's fundamental type as an integer constant.
inline Subtype Type::subtype() const {
    return m_subtype;
}

// has_fixed_size returns true if the Type has a fixed size in bytes.
inline bool Type::has_fixed_size() const {
    return (m_size != 0);
}
// fixed_size returns the size of the Type in bytes or 0 if it is variable.
inline size_t Type::fixed_size() const {
    return m_size;
}

// has_alias returns true if this type was defined the an aliased name.
inline bool Type::has_alias() const {
    return (m_alias.length() > 0);
}
// alias returns the name used to define the type, or the empty string.
inline const std::string& Type::alias() const {
    return m_alias;
}

// set_alias gives this type the alternate name <alias>.
inline void Type::set_alias(const std::string& alias) {
    m_alias = alias;
}

} // close namespace bamboo
