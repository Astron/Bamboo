// Filename: Struct.h
#pragma once
#include <string>        // std::string
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map

#include "DistributedType.h"
namespace bamboo { // open namespace


// Foward declarations
class HashGenerator;
class Module;
class Class;
class Field;

// A Struct provides type composition by combining multiple Fields each with their own type.
//     Structs may have both anonymous and named Fields.
class Struct : public DistributedType {
  public:
    Struct(Module *file, const std::string& name);
    virtual ~Struct();

    // as_struct returns this as a Struct if it is a Struct, or NULL otherwise.
    virtual Struct *as_struct();
    virtual const Struct *as_struct() const;

    // as_class returns this Struct as a Class if it is a Class, or NULL otherwise.
    virtual Class *as_class();
    virtual const Class *as_class() const;

    // get_id returns a unique index number associated with this struct.
    inline unsigned int get_id() const;
    // get_name returns the name of this struct.
    inline const std::string& get_name() const;
    // get_module returns the Module object that contains the struct.
    inline Module *get_module();
    inline const Module *get_module() const;

    // get_num_fields returns the number of fields in the struct.
    inline size_t get_num_fields() const;
    // get_field returns the <n>th field of the struct.
    inline Field *get_field(unsigned int n);
    inline const Field *get_field(unsigned int n) const;

    // get_field_by_id returns the field with the index <id>, or NULL if no such field exists.
    inline Field *get_field_by_id(unsigned int id);
    inline const Field *get_field_by_id(unsigned int id) const;
    // get_field_by_name returns the field with <name>, or NULL if no such field exists.
    inline Field *get_field_by_name(const std::string& name);
    inline const Field *get_field_by_name(const std::string& name) const;

    // add_field adds a new Field to the struct.
    //     Returns false if the field could not be added to the struct.
    virtual bool add_field(Field *field);

  protected:
    Struct(Module *module);

    // set_id sets the index number associated with this struct.
    void set_id(unsigned int id);
    friend class Module;

    Module *m_module;
    unsigned int m_id;
    std::string m_name;

    std::vector<Field *> m_fields;
    std::unordered_map<std::string, Field *> m_fields_by_name;
    std::unordered_map<unsigned int, Field *> m_fields_by_id;
};


} // close namespace bamboo
#include "Struct.ipp"
