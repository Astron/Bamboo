// Filename: TypeAlias.h
#pragma once
#include "../module/Type.h"
namespace bamboo
{


// A TypeAlias is used to repesent a typedef
// It wraps a type with a custom name
class TypeAlias : public Type
{
  public:
    TypeAlias(const std::string& alias, Type *type, bool transfer_ownership) :
        Type(type->subtype()), m_alias(alias), m_type(type), m_type_owned(transfer_ownership)
    {
        m_size = m_type->fixed_size();
    }
    TypeAlias(const TypeAlias&) = delete;
    TypeAlias& operator=(const TypeAlias&) = delete;
    virtual ~TypeAlias() {}

    inline const std::string& alias() const { return m_alias; }

    Numeric *as_numeric() override { return m_type->as_numeric(); }
    const Numeric *as_numeric() const override { return m_type->as_numeric(); }
    Array *as_array() override { return m_type->as_array(); }
    const Array *as_array() const override { return m_type->as_array(); }
    Struct *as_struct() override { return m_type->as_struct(); }
    const Struct *as_struct() const override { return m_type->as_struct(); }
    Method *as_method() override { return m_type->as_method(); }
    const Method *as_method() const override { return m_type->as_method(); }

    std::string to_string() const override { return m_alias; }

  private:
    std::string m_alias;
    Type *m_type = nullptr;
    bool m_type_owned = false;
};


} // close namespace bamboo
