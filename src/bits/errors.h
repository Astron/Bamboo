// Filename: errors.h
#pragma once
#include <stdexcept>
namespace bamboo   // open namespace bamboo
{


class null_error : std::invalid_argument
{
  public:
    null_error(const std::string& reason) :
        invalid_argument("Nullptr " + reason), m_reason(reason) {}

    inline const std::string& reason() { return m_reason; }

  private:
    std::string m_reason;
};

class invalid_type : std::domain_error
{
  public:
    invalid_type(const std::string& what) : std::domain_error(what) {}
};


} // close namespace bamboo
