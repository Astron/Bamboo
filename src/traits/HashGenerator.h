// Filename: HashGenerator.h
#pragma once
#include <stdint.h> // for fixed-with integers
#include <string>   // for std::string
namespace bamboo   // open namespace bamboo
{


// A HashGenerator generates an arbitrary hash number from a sequence of ints.
class HashGenerator
{
  public:
    HashGenerator();

    // add_int adds another integer to the hash so far.
    void add_int(int num);

    // add_string adds a string to the hash, by breaking it down into a sequence of integers.
    void add_string(const std::string& str);

    uint32_t hash() const;

  private:
    uint32_t m_hash;
    unsigned int m_index;
};


} // close namespace bamboo
