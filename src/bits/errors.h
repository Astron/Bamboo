// Filename: errors.h
#pragma once
#include <stdexcept>
namespace bamboo   // open namespace bamboo
{

    class null_error : std::invalid_argument
    {
        using std::invalid_argument::invalid_argument;
    };
    class invalid_type : std::domain_error
    {
        using std::domain_error::domain_error;
    };
}
