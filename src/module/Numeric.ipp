// Filename: Numeric.ipp
namespace bamboo { // open namespace


// divisor returns the divisor of the numeric, with a default value of one.
inline unsigned int Numeric::divisor() const {
    return m_divisor;
}

// has_modulus returns true if the numeric is constrained by a modulus.
inline bool Numeric::has_modulus() const {
    return m_orig_modulus != 0.0;
}

// modulus returns a double representation of the modulus value.
inline double Numeric::modulus() const {
    return m_orig_modulus;
}

// has_range returns true if the numeric is constrained by a range.
inline bool Numeric::has_range() const {
    return !m_orig_range.is_empty();
}
// range returns the NumericRange that constrains the type's values.
inline NumericRange Numeric::range() const {
    return m_orig_range;
}


} // close namespace bamboo
