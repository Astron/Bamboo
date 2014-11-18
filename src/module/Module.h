// Filename: Module.h
#pragma once
#include <memory> // std::unique_ptr
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Class.h" // Class, Struct, Field
namespace bamboo
{


// An Import corresponds to a production of the form `from module.path import ClassName/Suffixes`
//     which defines a source module to import/include to Class implementations from.
// LEGACY: This is for use by Panda3d, new code should compile to a target language a la protobuf.
struct Import {
    std::string module;
    std::vector<std::string> symbols;

    inline Import() : module("<undefined>") {}
    inline explicit Import(const std::string& module_name) : module(module_name) {}
};

// A Module is a list of Class definitions that define an independent network protocol.
//     Typically, parsed from one or more `.dc` files or other bamboo protocol descriptions.
class Module
{
  public:
    Module() {}
    Module(const Module&) = delete;
    Module& operator=(const Module&) = delete;

    inline size_t num_classes() const { return m_classes.size(); }
    inline Class *nth_class(unsigned int n) { return m_classes[n]; }
    inline const Class *nth_class(unsigned int n) const { return m_classes[n]; }

    inline size_t num_structs() const { return m_structs.size(); }
    inline Struct *nth_struct(unsigned int n) { return m_structs[n]; }
    inline const Struct *nth_struct(unsigned int n) const { return m_structs[n]; }

    inline Class *class_by_id(int id);
    inline const Class *class_by_id(int id) const;
    inline Class *class_by_name(const std::string& name);
    inline const Class *class_by_name(const std::string& name) const;

    inline size_t num_ids() const { return m_types_by_id.size(); }
    inline Type *type_by_id(int id) { return m_types_by_id[id]; }
    inline const Type *type_by_id(int id) const { return m_types_by_id[id]; }
    inline Type *type_by_name(const std::string& name);
    inline const Type *type_by_name(const std::string& name) const;

    inline Field *field_by_id(int id) { return m_fields_by_id[id]; }
    inline const Field *field_by_id(int id) const { return m_fields_by_id[id]; }

    inline size_t num_imports() const { return m_imports.size(); }
    inline Import *nth_import(unsigned int n) { return m_imports[n].get(); }
    inline const Import *nth_import(unsigned int n) const { return m_imports[n].get(); }
    inline Import *import_by_module(const std::string& pymodule);
    inline const Import *import_by_module(const std::string& pymodule) const;

    inline bool has_keyword(const std::string& name);
    inline size_t num_keywords() const { return m_keywords.size(); }
    inline const std::string& nth_keyword(unsigned int n) const { return m_keywords[n]; }

    Class *add_class(const char *name);
    Class *add_class(const std::string& name);
    bool add_class(Class *class_, bool transfer_ownership);
    Struct *add_struct(const char *name);
    Struct *add_struct(const std::string& name);
    bool add_struct(Struct *struct_, bool transfer_ownership);
    Import *add_import(const char *name);
    Import *add_import(const std::string& pymodule);
    bool add_import(Import *import, bool transfer_ownership);
    bool add_typedef(const std::string& name, Type *type, bool transfer_ownership);
    bool add_keyword(const std::string& name);

  private:
    friend class Class;
    friend class Struct;

    void register_field(Field *field); // sets the field's id

    std::unordered_set<Type *> m_types;
    std::vector<Struct *> m_structs;
    std::vector<Class *> m_classes;

    std::vector<std::unique_ptr<Type>> m_owned_types;
    std::vector<std::unique_ptr<Import>> m_imports; // list of python imports, for legacy panda3d

    std::vector<Type *> m_types_by_id;
    std::unordered_map<std::string, Type *> m_types_by_name;
    std::unordered_map<std::string, Import *> m_imports_by_module;

    std::vector<Field *> m_fields_by_id;

    std::vector<std::string> m_keywords;
    std::unordered_set<std::string> m_keywords_by_name;
};


} // close namespace bamboo
#include "Module.ipp"
