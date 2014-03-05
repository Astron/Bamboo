// Filename: Method.ipp
namespace bamboo { // open namespace


// get_num_parameters returns the number of parameters/arguments of the method.
inline size_t Method::get_num_parameters() const {
    return m_parameters.size();
}
// get_element returns the <n>th parameter of the method.
inline Parameter *Method::get_parameter(unsigned int n) {
    return m_parameters.at(n);
}
inline const Parameter *Method::get_parameter(unsigned int n) const {
    return m_parameters.at(n);
}
// get_parameter_names returns a mapping of param names to param number in the method.
inline const std::unordered_map<std::string, unsigned int>& Method::get_parameter_names() const {
    return m_indices_by_name;
}
// get_parameter_by_name returns the parameter with <name>, or nullptr if no such param exists.
inline Parameter *Method::get_parameter_by_name(const std::string& name) {
    auto it = m_parameters_by_name.find(name);
    if(it == m_parameters_by_name.end()) {
        return nullptr;
    }
    return it->second;
}
inline const Parameter *Method::get_parameter_by_name(const std::string& name) const {
    auto it = m_parameters_by_name.find(name);
    if(it == m_parameters_by_name.end()) {
        return nullptr;
    }
    return it->second;
}


} // close namespace bamboo
