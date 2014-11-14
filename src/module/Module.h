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
    inline Class *nth_class(unsigned int n) { return m_classes[n].get(); }
    inline const Class *nth_class(unsigned int n) const { return m_classes[n].get(); }

    inline size_t num_structs() const { return m_structs.size(); }
    inline Struct *nth_struct(unsigned int n) { return m_structs[n].get(); }
    inline const Struct *nth_struct(unsigned int n) const { return m_structs[n].get(); }

    inline Class *class_by_id(unsigned int id);
    inline const Class *class_by_id(unsigned int id) const;
    inline Class *class_by_name(const std::string& name);
    inline const Class *class_by_name(const std::string& name) const;

    // num_types is a count of all user defined types (ie. structs and classes)
    //     Struct and class ids will be within the range `[0, num_types)`.
    inline size_t num_types() const { return m_types_by_id.size(); }
    inline Type *type_by_id(unsigned int id) { return m_types_by_id[id]; }
    inline const Type *type_by_id(unsigned int id) const { return m_types_by_id[id]; }
    inline Type *type_by_name(const std::string& name);
    inline const Type *type_by_name(const std::string& name) const;

    inline Field *field_by_id(unsigned int id) { return m_fields_by_id[id]; }
    inline const Field *field_by_id(unsigned int id) const { return m_fields_by_id[id]; }

    inline size_t num_imports() const { return m_imports.size(); }
    inline Import *nth_import(unsigned int n) { return m_imports[n].get(); }
    inline const Import *nth_import(unsigned int n) const { return m_imports[n].get(); }

    inline bool has_keyword(const std::string& name);
    inline size_t num_keywords() const { return m_keywords.size(); }
    inline const std::string& nth_keyword(unsigned int n) const { return m_keywords[n]; }

    bool add_class(std::unique_ptr<Class> cls);
    bool add_struct(std::unique_ptr<Struct> container);
    bool add_typedef(const std::string& name, Type *type);
    bool add_keyword(const std::string& name);
    void add_import(std::unique_ptr<Import> import); // TODO: Combine imports with duplicate modules

  private:
    void add_field(Field *field); // sets the field's id
    friend class Class;
    friend class Struct;

    std::vector<std::unique_ptr<Struct>> m_structs;
    std::vector<std::unique_ptr<Class>> m_classes;
    std::vector<std::unique_ptr<Import>> m_imports; // list of python imports, for legacy panda3d
    std::vector<std::string> m_keywords;
    std::unordered_set<std::string> m_keywords_by_name;

    std::vector<Field *> m_fields_by_id;
    std::vector<Type *> m_types_by_id;
    std::unordered_map<std::string, Type *> m_types_by_name;
};


} // close namespace bamboo
#include "Module.ipp"
