// Filename: Parameter.h
#pragma once
#include <string> // std::string
#include <vector> // std::vector
namespace bamboo   // open namespace bamboo
{


// Foward declarations
class Type;
class Method;
struct Value;
class HashGenerator;

// A Parameter is a single argument/parameter of a Method.
// Parameter's constructor throws null_error if the type is null.
class Parameter
{
  public:
    Parameter(Type *type, const std::string& name = ""); // TODO: Throw null_error
    Parameter(const Parameter&) = delete;
    Parameter& operator=(const Parameter&) = delete;

    // name returns the parameter's name.  An unnamed parameter returns the empty string.
    inline const std::string& name() const;
    // position returns the parameter's order in the function.
    inline unsigned int position() const;
    // type returns the Type of the Parameter.
    inline Type *type();
    inline const Type *type() const;
    // get_method returns the Method that contains the Parameter.
    inline Method *get_method();
    inline const Method *get_method() const;

    // has_default_value returns true if a default value was defined for this parameter.
    inline bool has_default_value() const;
    // default_value returns the default value for this parameter.
    //     Returns nullptr if there is no default value.
    inline const Value *default_value() const;

    // set_name sets the name of this parameter.  Returns false if a parameter with
    //     the same name already exists in the containing method.
    bool set_name(const std::string& name);

    // set_type sets the distributed type of the parameter and clear's the default value.
    //     Returns false if a parameter cannot represent <type>.
    bool set_type(Type *type);

    // set_default_value defines a default value for this parameter.
    //     Returns false if the value is invalid for the field's type.
    bool set_default_value(const Value& default_value);
    bool set_default_value(const Value *default_value);
    bool set_default_value(const std::vector<uint8_t>& default_value);

    struct sort_by_position {
        inline bool operator()(const Parameter *lhs, const Parameter *rhs) const;
    };

  private:
    // set_position allows a method to tell the parameter its order in the function.
    void set_position(unsigned int);
    // set_method sets a pointer to the method containing the parameter.
    void set_method(Method *method);
    friend class Method;

    Type *m_type;
    std::string m_type_alias;
    std::string m_name;

    Method *m_method = nullptr;
    Value *m_default_value = nullptr;
    unsigned int m_position = 0;
};


} // close namespace bamboo
#include "Parameter.ipp"
