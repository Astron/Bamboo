// Filename: MolecularField.h
#pragma once
#include "Field.h"
#include "Struct.h"
namespace bamboo   // open namespace bamboo
{


// A MolecularField is an abstract field which provides an interface that can
//     be used to access multiple other fields at the same time.
class MolecularField : public Field, private Struct
{
  public:
    MolecularField(const std::string& name);
    MolecularField(const MolecularField&) = delete;
    MolecularField& operator=(const MolecularField&) = delete;
    virtual ~MolecularField() {}
    MolecularField *as_molecular() override;
    const MolecularField *as_molecular() const override;

    inline size_t num_atomics() const { return m_fields.size(); }
    inline Field *nth_atomic(unsigned int n) const { return m_fields[n]; }

    bool add_atomic(Field* field);

    // MolecularFields have implicit type
    bool set_type(Type *, bool) override { return false; }

    // MolecularFields have implicit default values
    bool set_default_value(const Value&) override { return false; }
    bool set_default_value(const Value *) override { return false; }

  private:
    // MolecularFields have implicit fields
    Field *add_field(const std::string&, Type *) override { return nullptr; }
    bool register_field(std::unique_ptr<Field>) override { return false; }
};


} // close namespace bamboo
