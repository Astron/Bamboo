// Filename: primes.h
#pragma once
namespace bamboo   // open namespace bamboo
{


// get_prime returns the nth prime number.  This[0] returns 2, this[1] returns 3;
//     Successively larger values of n return larger prime numbers, up to the
//     largest prime number that can be represented in an unsigned int.
unsigned int get_prime(unsigned int n);


} // close namespace bamboo
