// Filename: Method.cpp
#include "Method.h"
#include "../module/Parameter.h"
using namespace std;
namespace bamboo
{


Method::Method()
{
    m_subtype = kTypeMethod;
}

Method *Method::as_method()
{
    return this;
}

const Method *Method::as_method() const
{
    return this;
}

Parameter *add_param(const std::string& name, Type *type, bool param_owns_type)
{
    // TODO: Implement
}

bool Method::add_param(Parameter *param)
{
    // Parameters must have unique names
    if(!param->name().empty()) {
        bool inserted = m_params_by_name.emplace(param->name(), param).second;
        if(!inserted) { return false; }
        m_positions_by_name[param->name()] = (unsigned int) m_params.size();
    }

    // Update our size
    if(has_fixed_size() || m_params.size() == 1) {
        if(param->type()->has_fixed_size()) {
            m_size += param->type()->fixed_size();
        } else {
            m_size = 0;
        }
    }

    // Transfer ownership of the Parameter to the Method
    param->m_method = this;
    param->m_position = (unsigned int)m_params.size();
    m_params.push_back(unique_ptr<Parameter>(param));

    return true;
}

void Method::update_param_type(Parameter *param, Type *new_type, Type *old_type)
{
    // TODO: Implement
}



} // close namespace bamboo
