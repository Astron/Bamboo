// Filename: Method.cpp
#include "Method.h"
#include "../module/Parameter.h"
using namespace std;
namespace bamboo   // open namespace
{


// constructor
Method::Method()
{
    m_subtype = kTypeMethod;
}

// as_method returns this as a Method if it is a method, or nullptr otherwise.
Method *Method::as_method()
{
    return this;
}
const Method *Method::as_method() const
{
    return this;
}

// add_parameter adds a new parameter to the method.
bool Method::add_parameter(unique_ptr<Parameter> param)
{
    Parameter *ref = param.get();

    // Param should not be null
    if(param == nullptr) {
        return false;
    }

    if(!param->name().empty()) {
        // Try to add the parameter
        bool inserted = m_parameters_by_name.insert(
                            unordered_map<string, Parameter *>::value_type(param->name(), ref)).second;
        if(!inserted) {
            // But the parameter had a name conflict
            return false;
        }
        // The size of the list is the index of the next item in the list
        m_indices_by_name[param->name()] = (unsigned int)m_parameters.size();
    }

    // Update our size
    if(has_fixed_size() || m_parameters.size() == 1) {
        if(param->type()->has_fixed_size()) {
            m_size += param->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    // Transfer ownership of the Parameter to the Method
    param->set_position((unsigned int)m_parameters.size());
    param->set_method(this);
    m_parameters.push_back(move(param));

    return true;
}


} // close namespace bamboo
