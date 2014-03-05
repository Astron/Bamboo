// Filename: ObjectData.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "bits/sizetag_t.h"
namespace bamboo {


// Foward declarations
class DistributedType;
class ObjectData;
class ObjectDataHandle;

class ObjectDataBase {
  public:
    virtual ~ObjectDataBase() {}

    inline const DistributedType *type() const { return m_type; }
    inline sizetag_t num_elements() const { return m_num_elements; }

    virtual ObjectDataHandle& operator[](sizetag_t index) = 0;
    virtual const ObjectDataHandle& operator[](sizetag_t index) const = 0;
    virtual ObjectDataHandle& operator[](const std::string& element_name) = 0;
    virtual const ObjectDataHandle& operator[](const std::string& element_name) const = 0;

    // These functions are primarily for use with bindings.
    // In C++ code, use array subscription instead.
    const ObjectDataHandle& get_item(sizetag_t index) const;
    void set_item(sizetag_t index, ObjectData item);
    const ObjectDataHandle& get_item(const std::string& element_name) const;
    void set_item(const std::string& element_name, ObjectData item);

  private:
    ObjectDataBase(const DistributedType *);

    const DistributedType *m_type;
    sizetag_t m_num_elements;
    std::unordered_map<std::string, sizetag_t> m_elements_by_name;
};

class ObjectData : public ObjectDataBase {
  public:
    ObjectData(unsigned int field_id, const std::vector<uint8_t>&);
    ObjectData(const DistributedType *, const std::vector<uint8_t>&);
    ObjectData& operator=(const ObjectDataHandle&);
    virtual ~ObjectData() {}

    // TODO: Add implicit conversion too and from each C++ type represented by a subtype

  private:
    friend class ObjectDataHandle;
    std::vector<uint8_t> m_data;
};

class ObjectDataHandle : public ObjectDataBase {
  public:
    virtual ~ObjectDataHandle() {}
  private:
    friend class ObjectData;
    ObjectDataHandle(ObjectData&);

    std::vector<uint8_t> data();

    std::vector<uint8_t>& m_data;
    sizetag_t m_start, m_end;
};


} // close namespace bamboo
