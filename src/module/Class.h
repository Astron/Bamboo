// Filename: Class.h
#pragma once
#include "Struct.h"
#include <memory>  // std::unique_ptr
#include <unordered_set>
namespace bamboo   // open namespace
{


// A Class is a special type of struct that have a couple advanced object-oriented features:
//     Classes can inherit from other classes (ie. have super-/sub-classes).
//     Classes can have methods including a special constructor method.
//     Unlike other structs, classes cannot have anonymous fields.
// Class's constructor throws null_error if the module is null.
class Class : public Struct
{
  public:
    Class(Module *module, const std::string& name); // TODO: Throw null_error
    Class(const Class&) = delete;
    Class& operator=(const Class&) = delete;
    virtual ~Class() {};

    // as_class returns this Struct as a Class if it is a Class, or nullptr otherwise.
    Class *as_class() override;
    const Class *as_class() const override;

    // num_parents returns the number of superclasses this class inherits from.
    inline size_t num_parents() const;
    // get_parent returns the <n>th parent-/super-class this class inherits from.
    //     Throws std::out_of_range
    inline Class *get_parent(unsigned int n);
    inline const Class *get_parent(unsigned int n) const;
    // num_children returns the number of subclasses that inherit from this class.
    inline size_t num_children() const;
    // get_child returns the <n>th child-/sub-class that inherits this class.
    //     Throws std::out_of_range
    inline Class *get_child(unsigned int n);
    inline const Class *get_child(unsigned int n) const;

    // has_constructor returns true if this class has a constructor method,
    //     or false if it just uses the default constructor.
    inline bool has_constructor() const;
    // constructor returns the constructor method for this class if it is defined,
    //     or nullptr if the class uses the default constructor.
    inline Field *constructor();
    inline const Field *constructor() const;

    // num_base_fields returns the number of fields declared directly in this class.
    inline size_t num_base_fields() const;
    // get_base_field returns the <n>th field from the class excluding any inherited fields.
    //     Throws std::out_of_range
    inline Field *get_base_field(unsigned int n);
    inline const Field *get_base_field(unsigned int n) const;

    // add_parent set this class as a subclass to target parent.
    bool add_parent(Class *parent);

    // add_field adds a new Field to the class.
    bool add_field(std::unique_ptr<Field> field) override;

  private:
    // add_child marks a class as a child of this class.
    void add_child(Class *child);
    // add_inherited_field updates a classes's fields after a parent adds a new field.
    void add_inherited_field(Class *parent, Field *field);
    // shadow_field removes the field from all of the Class's field accessors,
    //     so that another field with the same name can be inserted.
    void shadow_field(Field *field);

    std::unique_ptr<Field> m_constructor;
    std::unordered_set<std::string> m_base_names;
    std::vector<Class *> m_parents;
    std::vector<Class *> m_children;
};


} // close namespace bamboo
#include "Class.ipp"
