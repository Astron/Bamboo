// Filename: Field.ipp
namespace bamboo { // open namespace


// id() returns a unique index number associated with this field.
inline unsigned int Field::id() const {
    return m_id;
}

// name returns the field's name.  An unnamed field returns the empty string.
inline const std::string& Field::name() const {
    return m_name;
}

// type returns the Type of the field.
inline Type *Field::type() {
    return m_type;
}
inline const Type *Field::type() const {
    return m_type;
}

// record() returns the Struct that contains this field.
inline Struct *Field::record() {
    return m_struct;
}
inline const Struct *Field::record() const {
    return m_struct;
}

// has_default_value returns true if a default value was defined for this field.
inline bool Field::has_default_value() const {
    return m_default_value != nullptr;
}

// default_value returns the default value for this field.
//     Returns nullptr if there is no default value.
inline const Value *Field::default_value() const {
    return m_default_value;
}

inline bool Field::sort_by_id::operator()(const Field* lhs, const Field* rhs) const {
    return lhs->id() < rhs->id();
}


} // close namespace bamboo
