// Filename: Field.h
#pragma once
#include <cstdint>
#include "KeywordList.h"
namespace bamboo   // open namespace
{

// Foward declarations
class Type;
class Module;
class Class;
class Struct;
struct Value;
class MolecularField;
class HashGenerator;

// A Field is a member of a class or struct.
// Field's constructor throws null_error if the type is null.
class Field : public KeywordList
{
  public:
    Field(Type *type, const std::string& name = ""); // TODO: Throw null_error
    Field(const Field&) = delete;
    Field& operator=(const Field&) = delete;
    virtual ~Field();

    // as_molecular returns this as a MolecularField if it is molecular, or nullptr otherwise.
    virtual MolecularField *as_molecular();
    virtual const MolecularField *as_molecular() const;

    // id returns a unique index number associated with this field.
    inline unsigned int id() const;
    // name returns the field's name.  An unnamed field returns the empty string.
    inline const std::string& name() const;
    // type returns the Type of the field.
    inline Type *type();
    inline const Type *type() const;
    // record returns the Struct or Class that contains this field.
    inline Struct *record();
    inline const Struct *record() const;

    // has_default_value returns true if a default value was defined for this field.
    inline bool has_default_value() const;
    // default_value returns the default value for this field.
    //     Returns nullptr if there is no default value.
    inline const Value *default_value() const;

    // set_name sets the name of this field.  Returns false if a field with
    //     the same name already exists in the containing struct.
    bool set_name(const std::string&);

    // set_type sets the distributed type of the field and clear's the default value.
    void set_type(Type *);

    // set_default_value defines a default value for this field.
    //     Returns false if the value is invalid for the field's type.
    virtual bool set_default_value(const Value&);
    virtual bool set_default_value(const Value *);
    virtual bool set_default_value(const std::vector<uint8_t>& packed);

    struct sort_by_id {
        inline bool operator()(const Field *lhs, const Field *rhs) const;
    };

  protected:
    // set_id sets the unique index number associated with the field.
    void set_id(unsigned int id);
    friend class Module;

    // set_struct sets a pointer to the struct containing the field.
    void set_struct(Struct *);
    friend class Struct;
    friend class Class;

    Type *m_type;
    std::string m_name;

    Struct *m_struct = nullptr;
    Value *m_default_value = nullptr;
    unsigned int m_id = 0;
};


} // close namespace bamboo
#include "Field.ipp"
