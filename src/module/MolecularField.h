// Filename: MolecularField.h
#pragma once
#include "Field.h"
#include "Struct.h"
namespace bamboo { // open namespace bamboo


// A MolecularField is an abstract field which provides an interface that can
//     be used to access multiple other fields at the same time.
class MolecularField : public Field, public Struct {
  public:
    MolecularField(Class *cls, const std::string& name);
    virtual ~MolecularField();

    // as_molecular returns this as a MolecularField if it is molecular, or nullptr otherwise.
    virtual MolecularField *as_molecular();
    virtual const MolecularField *as_molecular() const;

    // Use the field interface by default and not the struct interface
    using Field::get_id;
    using Field::get_name;
    using Field::get_type;
    using Field::set_name;

    // add_field adds a new Field as part of the Molecular.
    //     Returns false if the field could not be added.
    virtual bool add_field(Field *field);

    // set_default_value defines a default value for this field.
    //     For a molecular field, this always returns false (molecular defaults are implict).
    virtual bool set_default_value(const Value default_value);
    virtual bool set_default_value(const std::vector<uint8_t>& default_value);

  protected:
    using Field::set_id;
};


} // close namespace bamboo
