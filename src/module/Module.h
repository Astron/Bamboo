// Filename: Module.h
#pragma once
#include <string>        // std::string
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map
#include <memory>        // std::unique_ptr
namespace bamboo   // open namespace
{


// Forward declarations
class Type;
class Class;
class Struct;
class Field;
class HashGenerator;

struct Import {
    std::string module;
    std::vector<std::string> symbols;

    inline Import();
    inline Import(const std::string& module_name);
};

// A Module represents the complete list of Distributed Class definitions that define a particular
// protocol. Typically, parsed from one or more ".dc" or other bamboo protocol description files.
class Module
{
  public:
    Module(); // constructor
    Module(const Module&) = delete;
    Module& operator=(const Module&) = delete;

    // num_classes returns the number of classes in the module.
    inline size_t num_classes() const;
    // get_class returns the <n>th class in the module.
    inline Class *get_class(unsigned int n);
    inline const Class *get_class(unsigned int n) const;

    // num_structs returns the number of structs in the module.
    inline size_t num_structs() const;
    // get_struct returns the <n>th struct in the module.
    inline Struct *get_struct(unsigned int n);
    inline const Struct *get_struct(unsigned int n) const;

    // class_by_id returns the requested class or nullptr if there is no such class.
    Class *class_by_id(unsigned int id);
    const Class *class_by_id(unsigned int id) const;
    // class_by_name returns the requested class or nullptr if there is no such class.
    Class *class_by_name(const std::string& name);
    const Class *class_by_name(const std::string& name) const;

    // num_types returns the number of types in the module.
    //     All type ids will be within the range 0 <= id < num_types().
    inline size_t num_types() const;
    // type_by_id returns the requested type or nullptr if there is no such type.
    inline Type *type_by_id(unsigned int id);
    inline const Type *type_by_id(unsigned int id) const;
    // type_by_name returns the requested type or nullptr if there is no such type.
    inline Type *type_by_name(const std::string& name);
    inline const Type *type_by_name(const std::string& name) const;

    // field_by_id returns the request field or nullptr if there is no such field.
    inline Field *field_by_id(unsigned int id);
    inline const Field *field_by_id(unsigned int id) const;

    // num_imports returns the number of imports in the module.
    inline size_t num_imports() const;
    // get_import retuns the <n>th import in the module.
    inline Import *get_import(unsigned int n);
    inline const Import *get_import(unsigned int n) const;

    // has_keyword returns true if a keyword with the name <keyword> is declared in the module.
    inline bool has_keyword(const std::string& keyword) const;
    // num_keywords returns the number of keywords declared in the module.
    inline size_t num_keywords() const;
    // get_keyword returns the <n>th keyword declared in the module.
    inline const std::string& get_keyword(unsigned int n) const;

    // add_class adds the newly-allocated class to the module.
    //     Returns false if there is a name conflict.
    bool add_class(std::unique_ptr<Class> dclass);

    // add_struct adds the newly-allocated struct definition to the module.
    //     Returns false if there is a name conflict.
    bool add_struct(std::unique_ptr<Struct> dstruct);

    // add_import adds a newly-allocated import to the module.
    //     Imports with duplicate modules are combined.
    void add_import(std::unique_ptr<Import> import);

    // add_typedef adds the alias <name> to the module for the type <type>.
    //     Returns false if there is a name conflict.
    bool add_typedef(const std::string& name, Type *type);

    // add_keyword adds a keyword with the name <keyword> to the list of declared keywords.
    void add_keyword(const std::string& keyword);

  private:
    // add_field gives the field a unique id within the module.
    void add_field(Field *field);
    friend class Class;
    friend class Struct;

    std::vector<std::unique_ptr<Struct> > m_structs;
    std::vector<std::unique_ptr<Class> > m_classes;
    std::vector<std::unique_ptr<Import> > m_imports; // list of python imports in the module
    std::vector<std::string> m_keywords;

    std::vector<Field *> m_fields_by_id;
    std::vector<Type *> m_types_by_id;
    std::unordered_map<std::string, Type *> m_types_by_name;
};


} // close namespace bamboo
#include "Module.ipp"
