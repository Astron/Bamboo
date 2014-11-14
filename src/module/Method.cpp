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

bool Method::add_param(unique_ptr<Parameter> param)
{
    Parameter *ref = param.get();

    // Param should not be null
    if(param == nullptr) { return false; }

    // Parameters must have unique names
    if(!param->name().empty()) {
        bool inserted = m_params_by_name.emplace(param->name(), ref).second;
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
    param->set_position((unsigned int)m_params.size());
    param->set_method(this);
    m_params.push_back(move(param));

    return true;
}


} // close namespace bamboo
