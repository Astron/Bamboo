// Filename: Class.h
#pragma once
#include <memory> // std::unique_ptr
#include <unordered_set>
#include "Struct.h"
#include "Field.h"
namespace bamboo
{


// A Class is a struct with object-oriented features:
//     Classes can inherit from other classes (ie. have super-/sub-classes).
//     Classes can have methods.
//     Unlike other structs, classes cannot have anonymous fields.
// Class's constructor throws null_error if the module is null.
class Class : public Struct
{
  public:
    explicit Class(const std::string& name) : Struct(name) {}
    Class(const Class&) = delete;
    Class& operator=(const Class&) = delete;
    virtual ~Class() {};

    Class *as_class() override;
    const Class *as_class() const override;

    inline size_t num_parents() const { return m_parents.size(); }
    inline Class *nth_parent(unsigned int n) { return m_parents[n]; }
    inline const Class *nth_parent(unsigned int n) const { return m_parents[n]; }

    inline size_t num_children() const { return m_children.size(); }
    inline Class *nth_child(unsigned int n) { return m_children[n]; }
    inline const Class *nth_child(unsigned int n) const { return m_children[n]; }

    inline size_t num_declared_fields() const { return m_declared_fields.size(); }
    inline Field *nth_declared_field(unsigned int n) { return m_declared_fields[n].get(); }
    inline const Field *nth_declared_field(unsigned int n) const { return m_declared_fields[n].get(); }

    bool add_parent(Class *parent);

    Field *add_field(const std::string& name, Type *type, bool field_owns_type) override;
    bool add_field(Field *) override;  // If true, Class owns field

  private:
    friend class Module;

    Class(Module *module, const std::string& name, int id) : Struct(module, name, id) {}

    void add_child(Class *child);

    // add_inherited_field updates this Class's fields after a parent adds a new field.
    void add_inherited_field(Class *parent, Field *field);

    // shadow_field removes the field from all of this Class's field accessors,
    //     so that another field with the same name can be inserted.
    void shadow_field(Field *field);

    std::unordered_set<std::string> m_declared_names;
    std::vector<Class *> m_parents;
    std::vector<Class *> m_children;
};


} // close namespace bamboo
