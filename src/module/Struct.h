// Filename: Struct.h
#pragma once
#include "Type.h"
#include <memory>        // std::unique_ptr
#include <string>        // std::string
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map
namespace bamboo   // open namespace
{


// Foward declarations
class HashGenerator;
class Module;
class Class;
class Field;

// A Struct provides type composition by combining multiple Fields each with their own type.
// Struct's constructor throws null_error if module is null.
//     Structs may have both anonymous and named Fields.
class Struct : public Type
{
  public:
    Struct(Module *module, const std::string& name); // TODO: Throw null error
    Struct(const Struct&) = delete;
    Struct& operator=(const Struct&) = delete;
    virtual ~Struct() {};

    // as_class returns this Struct as a Class if it is a Class, or nullptr otherwise.
    virtual Class *as_class();
    virtual const Class *as_class() const;

    // as_struct returns this as a Struct if it is a Struct, or nullptr otherwise.
    Struct *as_struct() override;
    const Struct *as_struct() const override;

    // id() returns a unique index number associated with this struct.
    inline unsigned int id() const;
    // name returns the name of this struct.
    inline const std::string& name() const;
    // module returns the Module object that contains the struct.
    inline Module *module();
    inline const Module *module() const;

    // num_fields returns the number of fields in the struct.
    inline size_t num_fields() const;
    // get_field returns the <n>th field of the struct.
    //     Throws std::out_of_range
    inline Field *get_field(unsigned int n);
    inline const Field *get_field(unsigned int n) const;

    // field_by_id returns the field with the index <id>, or nullptr if no such field exists.
    inline Field *field_by_id(unsigned int id);
    inline const Field *field_by_id(unsigned int id) const;
    // field_by_name returns the field with <name>, or nullptr if no such field exists.
    inline Field *field_by_name(const std::string& name);
    inline const Field *field_by_name(const std::string& name) const;

    // add_field adds a new Field to the struct.
    //     Returns false if the field could not be added to the struct.
    virtual bool add_field(std::unique_ptr<Field> field);

  protected:
    Struct(Module *module);

    // set_id sets the index number associated with this struct.
    void set_id(unsigned int id);
    friend class Module;

    Module *m_module;
    unsigned int m_id;
    std::string m_name;

    std::vector<Field *> m_fields;
    std::vector<std::unique_ptr<Field> > m_owned_fields;
    std::unordered_map<std::string, unsigned int> m_indices_by_name;
    std::unordered_map<std::string, Field *> m_fields_by_name;
    std::unordered_map<unsigned int, Field *> m_fields_by_id;
};


} // close namespace bamboo
#include "Struct.ipp"
