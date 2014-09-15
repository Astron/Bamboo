// Filename: Array.ipp
namespace bamboo { // open namespace


// element_type returns the type of the individual elements of this array.
inline Type *Array::element_type() {
    return m_element_type;
}
inline const Type *Array::element_type() const {
    return m_element_type;
}

// array_size returns the fixed number of elements in this array,
//     or 0 if the array may contain a variable number of elements.
inline size_t Array::array_size() const {
    return m_array_size;
}

// has_range returns true if there is a constraint on the range of valid array sizes.
//     This is always true for fixed-size arrays.
inline bool Array::has_range() const {
    return !m_array_range.is_empty();
}

// range returns the range of sizes that the array may have.
inline NumericRange Array::range() const {
    return m_array_range;
}


} // close namespace bamboo
