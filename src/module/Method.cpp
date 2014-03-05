// Filename: Method.cpp
#include "Method.h"
#include "module/Parameter.h"
using namespace std;
namespace bamboo { // open namespace


// constructor
Method::Method() {
    m_subtype = kTypeMethod;
}

// destructor
Method::~Method() {
    for(auto it = m_parameters.begin(); it != m_parameters.end(); ++it) {
        delete(*it);
    }
    m_parameters.clear();
}

// as_method returns this as a Method if it is a method, or nullptr otherwise.
Method *Method::as_method() {
    return this;
}
const Method *Method::as_method() const {
    return this;
}

// add_parameter adds a new parameter to the method.
bool Method::add_parameter(Parameter *param) {
    // Param should not be null
    if(param == nullptr) {
        return false;
    }

    if(!param->get_name().empty()) {
        // Try to add the parameter
        bool inserted = m_parameters_by_name.insert(
                            unordered_map<string, Parameter *>::value_type(param->get_name(), param)).second;
        if(!inserted) {
            // But the parameter had a name conflict
            return false;
        }
        // The size of the list is the index of the next item in the list
        m_indices_by_name[param->get_name()] = m_parameters.size();
    }

    // Add the parameter to the main list
    param->set_method(this);
    m_parameters.push_back(param);

    // Update our size
    if(has_fixed_size() || m_parameters.size() == 1) {
        if(param->get_type()->has_fixed_size()) {
            m_size += param->get_type()->get_size();
        } else {
            m_size = 0;
        }
    }

    return true;
}


} // close namespace bamboo
