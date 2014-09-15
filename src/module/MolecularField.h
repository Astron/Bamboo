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
    virtual ~MolecularField();

    // as_molecular returns this as a MolecularField if it is molecular, or nullptr otherwise.
    MolecularField *as_molecular() override;
    const MolecularField *as_molecular() const override;

    // Use the field interface by default and not the struct interface
    using Field::id;
    using Field::name;
    using Field::type;
    using Field::set_name;

    // set_default_value always returns false; molecular fields don't have default values.
    bool set_default_value(const Value&) override;
    bool set_default_value(const Value *) override;
    bool set_default_value(const std::vector<uint8_t>&) override;

    // add_field adds a new Field as part of the Molecular.
    //     Returns false if the field could not be added.
    bool add_field(Field *field) override;

  protected:
    using Field::set_id;
};


} // close namespace bamboo
