// Filename: HashGenerator.h
#pragma once
#include <cstdint> // uint32_t
#include <string>
namespace bamboo
{


// A HashGenerator generates an arbitrary hash number from a sequence of ints.
class HashGenerator
{
  public:
    HashGenerator() {}

    void add_int(int num);
    void add_string(const std::string& str);

    uint32_t hash() const;

  private:
    uint32_t m_hash = 0;
    unsigned int m_index = 0;
};


} // close namespace bamboo
