// Filename: Struct.h
#pragma once
#include <memory> // std::unique_ptr
#include <string>
#include <vector>
#include <unordered_map>
#include "Type.h"
#include "Field.h"
namespace bamboo
{


class Module;
class Class;

// A Struct provides type composition by combining multiple Fields each with their own type.
//     Structs may have both anonymous and named Fields.
// Struct's constructor throws null_error if module is null.
class Struct : public Type
{
  public:
    Struct(Module *module, const std::string& name); // TODO: Throw null error
    Struct(const Struct&) = delete;
    Struct& operator=(const Struct&) = delete;
    virtual ~Struct() {};

    virtual Class *as_class();
    virtual const Class *as_class() const;

    Struct *as_struct() override;
    const Struct *as_struct() const override;

    inline unsigned int id() const;
    inline const std::string& name() const;

    inline Module *module();
    inline const Module *module() const;

    inline size_t num_fields() const;
    inline Field *nth_field(unsigned int n);
    inline const Field *nth_field(unsigned int n) const;
    inline Field *field_by_id(unsigned int id);
    inline const Field *field_by_id(unsigned int id) const;
    inline Field *field_by_name(const std::string& name);
    inline const Field *field_by_name(const std::string& name) const;

    virtual bool add_field(std::unique_ptr<Field> field);

  protected:
    Struct(Module *module);

    void set_id(unsigned int id);
    friend class Module;

    unsigned int m_id = 0;
    Module *m_module;
    std::string m_name;

    std::vector<Field *> m_fields;
    std::vector<std::unique_ptr<Field>> m_owned_fields;
    std::unordered_map<std::string, unsigned int> m_indices_by_name;
    std::unordered_map<std::string, Field *> m_fields_by_name;
    std::unordered_map<unsigned int, Field *> m_fields_by_id;
};


} // close namespace bamboo
#include "Struct.ipp"
