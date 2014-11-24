// Filename: Parameter.h
#pragma once
#include <string>
#include <vector>
namespace bamboo
{


class Type;
class Method;
class Value;

// A Parameter is a single argument/parameter of a Method.
class Parameter
{
  public:
    Parameter(Type *type, bool transfer_ownership);
    Parameter(const std::string& name, Type *type, bool transfer_ownership);
    Parameter(const Parameter&) = delete;
    Parameter& operator=(const Parameter&) = delete;
    ~Parameter();

    inline const std::string& name() const { return m_name; }
    inline unsigned int position() const { return m_position; }

    inline Type *type() { return m_type; }
    inline const Type *type() const { return m_type; }

    inline Method *method() { return m_method; }
    inline const Method *method() const { return m_method; }

    inline bool has_default_value() const { return m_default_value != nullptr; }
    inline const Value *default_value() const { return m_default_value; }

    bool set_name(const std::string& name);  // return false if it would cause a name conflict
    bool set_type(Type *type, bool transfer_ownership); // return false if the parameter can't represent the type
    bool set_default_value(const Value& default_value);
    bool set_default_value(const Value *default_value);

    struct SortByPosition
    {
        inline bool operator()(const Parameter *lhs, const Parameter *rhs) const
        {
            return lhs->position() < rhs->position();
        }
    };

  private:
    friend class Method;

    std::string m_name;
    Type *m_type = nullptr;
    bool m_type_owned = false;

    Method *m_method = nullptr;
    Value *m_default_value = nullptr;
    unsigned int m_position = 0; // argument position in function's prototype
};


} // close namespace bamboo
