// Filename: Field.h
#pragma once
#include "KeywordList.h"
namespace bamboo
{


class Type;
class Module;
class Class;
class Struct;
class Value;
class MolecularField;

// A Field is a member variable of a class or struct.
class Field : public KeywordList
{
  public:
    Field(Type *type, bool transfer_ownership);
    Field(const std::string& name, Type *type, bool transfer_ownership);
    Field(const Field&) = delete;
    Field& operator=(const Field&) = delete;
    virtual ~Field();
    virtual MolecularField *as_molecular();
    virtual const MolecularField *as_molecular() const;

    inline int id() const { return m_id; }
    inline const std::string& name() const { return m_name; }

    inline Type *type() { return m_type; }
    inline const Type *type() const { return m_type; }

    inline Struct *container() { return m_struct; }
    inline const Struct *container() const { return m_struct; }

    inline bool has_default_value() const { return m_default_value != nullptr; }
    inline const Value *default_value() const { return m_default_value; }


    bool set_name(const std::string&); // returns false if it would cause a name conflict
    virtual bool set_type(Type *type, bool transfer_ownership);
    virtual bool set_default_value(const Value&);
    virtual bool set_default_value(const Value *);

    struct SortById
    {
        inline bool operator()(const Field *lhs, const Field *rhs) const
        {
            return lhs->id() < rhs->id();
        }
    };

  protected:
    friend class Module;
    friend class Struct;
    friend class Class;

    std::string m_name;
    Type *m_type = nullptr;
    bool m_type_owned = false;

    int m_id = -1;
    Struct *m_struct = nullptr;
    Value *m_default_value = nullptr;
};


} // close namespace bamboo
