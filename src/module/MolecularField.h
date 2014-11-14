// Filename: MolecularField.h
#pragma once
#include "Field.h"
#include "Struct.h"
namespace bamboo   // open namespace bamboo
{


// A MolecularField is an abstract field which provides an interface that can
//     be used to access multiple other fields at the same time.
// MolecularField's constructor throws null_error if the class is null.
class MolecularField : public Field, public Struct
{
  public:
    MolecularField(Class *cls, const std::string& name); // TODO: Throw null error
    MolecularField(const MolecularField&) = delete;
    MolecularField& operator=(const MolecularField&) = delete;
    virtual ~MolecularField() {}
    MolecularField *as_molecular() override;
    const MolecularField *as_molecular() const override;

    // Use the field interface by default and not the struct interface
    using Field::id;
    using Field::name;
    using Field::type;
    using Field::set_name;

    bool set_default_value(const Value&) override; // always returns false; no explicit defaults
    bool set_default_value(const Value *) override;

    bool add_field(Field *field);
    bool add_field(std::unique_ptr<Field> field) override;

  protected:
    using Field::set_id;
};


} // close namespace bamboo
