// Filename: Method.h
#pragma once
#include <memory>        // std::unique_ptr
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map
#include "Type.h"
namespace bamboo   // open namespace
{

// Forward declarations
class Parameter;

// A Method is a field for a distributed Class that typically represents a remote procedure call.
class Method : public Type
{
  public:
    Method();
    Method(const Method&) = delete;
    Method& operator=(const Method&) = delete;
    virtual ~Method() {};

    // as_method returns this as a Method if it is a method, or nullptr otherwise.
    Method *as_method() override;
    const Method *as_method() const override;

    // num_parameters returns the number of parameters/arguments of the method.
    inline size_t num_parameters() const;
    // get_parameter returns the <n>th parameter of the method.
    inline Parameter *get_parameter(unsigned int n);
    inline const Parameter *get_parameter(unsigned int n) const;
    // parameter_by_name returns the requested parameter or nullptr if there is no such param.
    inline Parameter *parameter_by_name(const std::string& name);
    inline const Parameter *parameter_by_name(const std::string& name) const;

    // add_parameter adds a new parameter to the method.
    //     Returns false if the parameter could not be added to the method.
    bool add_parameter(std::unique_ptr<Parameter> param);

  private:
    std::vector<std::unique_ptr<Parameter> > m_parameters;
    std::unordered_map<std::string, unsigned int> m_indices_by_name;
    std::unordered_map<std::string, Parameter *> m_parameters_by_name;
};


} // close namespace bamboo
#include "Method.ipp"
