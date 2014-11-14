// Filename: Method.h
#pragma once
#include <memory> // std::unique_ptr
#include <vector>
#include <unordered_map>
#include "Type.h"
#include "Parameter.h"
namespace bamboo
{


// A Method is a field for a distributed Class that typically represents a remote procedure call.
class Method : public Type
{
  public:
    Method();
    Method(const Method&) = delete;
    Method& operator=(const Method&) = delete;
    virtual ~Method() {};
    Method *as_method() override;
    const Method *as_method() const override;

    inline size_t num_params() const { return m_params.size(); }
    inline Parameter *nth_param(unsigned int n) { return m_params[n].get(); }
    inline const Parameter *nth_param(unsigned int n) const { return m_params[n].get(); }
    inline Parameter *param_by_name(const std::string& name)
    {
        auto it = m_params_by_name.find(name);
        return it == m_params_by_name.end() ? nullptr : it->second;
    }
    inline const Parameter *param_by_name(const std::string& name) const
    {
        auto it = m_params_by_name.find(name);
        return it == m_params_by_name.end() ? nullptr : it->second;
    }

    bool add_param(std::unique_ptr<Parameter> param);

  private:
    std::vector<std::unique_ptr<Parameter>> m_params;
    std::unordered_map<std::string, unsigned int> m_positions_by_name;
    std::unordered_map<std::string, Parameter *> m_params_by_name;
};


} // close namespace bamboo
