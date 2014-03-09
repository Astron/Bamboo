// Filename: TypeData.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "bits/sizetag.h"
namespace bamboo {


// Foward declarations
class DistributedType;
class TypeDataHandle;

class TypeData {
  public:
    TypeData(const DistributedType *, const std::vector<uint8_t>&);
    TypeData(const DistributedType *, const std::vector<uint8_t>&, sizetag_t start, sizetag_t end);
    TypeData(const DistributedType *, const uint8_t *start, const uint8_t *end);

    operator int8_t() const;
    operator int16_t() const;
    operator int32_t() const;
    operator int64_t() const;
    operator uint8_t() const;
    operator uint16_t() const;
    operator uint32_t() const;
    operator uint64_t() const;
    operator float() const;
    operator double() const;
    operator std::string() const;
    operator std::vector<uint8_t>() const;

    TypeDataHandle operator[](sizetag_t index);
    TypeDataHandle operator[](const std::string& element_name);

    // These functions are primarily for use with bindings.
    // In C++ code, use array subscription instead.
    TypeDataHandle get_item(sizetag_t index);
    TypeDataHandle get_item(const std::string& element_name);
    // TODO: Implement set_{item,data}
    //TypeDataHandle set_item(sizetag_t index, TypeData);
    //TypeDataHandle set_item(const std::string& element_name, TypeData);

    inline const DistributedType *type() const { return m_type; }
    inline sizetag_t num_elements() const { return m_num_elements; }
    inline std::vector<uint8_t> get_data() const { return std::vector<uint8_t>(m_data); }
    //inline void set_data(std::vector<uint8_t> data){ return m_data = data; }

  private:
    friend class TypeDataHandle;

    const DistributedType *m_type;
    std::vector<uint8_t> m_data;
    sizetag_t m_num_elements;
    std::unordered_map<std::string, unsigned int> m_elements_by_name;
    // m_element_offsets keeps track of the starting offset of each element we've computed.
    // N.B. In a typical case (iterating through the elements just once in order), this doesn't
    //      help with performance at all.  However when accessing elements out of order, by name,
    //      or with repeated access to elements, this will typically save us a lot of headache.
    std::unordered_map<sizetag_t, sizetag_t> m_element_offsets;
    // m_furthest_index is really criticial for variable-sized or arrays of var-sized types,
    // where it would otherwise require us to read every previous index in later indices.
    sizetag_t m_furthest_index;


    TypeDataHandle read_type(const DistributedType* type, sizetag_t offset);
};


class TypeDataHandle {
  public:
    TypeDataHandle(const DistributedType *, std::vector<uint8_t>&, sizetag_t start, sizetag_t end);

    operator int8_t() const;
    operator int16_t() const;
    operator int32_t() const;
    operator int64_t() const;
    operator uint8_t() const;
    operator uint16_t() const;
    operator uint32_t() const;
    operator uint64_t() const;
    operator float() const;
    operator double() const;
    operator std::string() const;
    operator std::vector<uint8_t>() const;

    TypeDataHandle operator[](sizetag_t index);
    TypeDataHandle operator[](const std::string& element_name);

    // These functions are primarily for use with bindings.
    // In C++ code, use array subscription instead.
    TypeDataHandle get_item(sizetag_t index);
    TypeDataHandle get_item(const std::string& element_name);

    inline const DistributedType *type() const { return m_type; }
    inline sizetag_t num_elements() const { return m_num_elements; }
    inline std::vector<uint8_t> get_data() const {
        return std::vector<uint8_t>(&m_data[m_start], &m_data[m_end]);
    }
    //inline void set_data(std::vector<uint8_t> data);


  private:
    friend class TypeData;

    const DistributedType *m_type;
    std::vector<uint8_t>& m_data;
    sizetag_t m_num_elements;
    sizetag_t m_start, m_end;

    TypeDataHandle read_type(const DistributedType* type, sizetag_t offset);
};


} // close namespace bamboo
