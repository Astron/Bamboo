// Filename: Parameter.ipp
namespace bamboo { // open namespace


// name returns the parameter's name.  An unnamed parameter returns the empty string.
inline const std::string& Parameter::name() const {
    return m_name;
}

// position returns the parameter's order in the function.
inline unsigned int Parameter::position() const {
    return m_position;
}

// type returns the Type of the Parameter.
inline Type *Parameter::type() {
    return m_type;
}
inline const Type *Parameter::type() const {
    return m_type;
}

// get_method returns the Method that contains the Parameter.
inline Method *Parameter::get_method() {
    return m_method;
}
inline const Method *Parameter::get_method() const {
    return m_method;
}

// has_default_value returns true if a default value was defined for this parameter.
inline bool Parameter::has_default_value() const {
    return m_default_value != nullptr;
}

// default_value returns the default value for this parameter.
//     Returns nullptr if there is no default value.
inline const Value *Parameter::default_value() const {
    return m_default_value;
}

inline bool Parameter::sort_by_position::operator()(const Parameter* lhs,
                                                    const Parameter* rhs) const {
    return lhs->position() < rhs->position();
}


} // close namespace bamboo
