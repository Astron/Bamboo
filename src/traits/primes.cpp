// Filename: primes.cpp
#include "primes.h"
#include <vector> // for std::vector
namespace bamboo { // open namespace


static vector<unsigned int> primes{2};
unsigned int get_prime(unsigned int n) {
    // Compute the prime numbers between the last-computed prime number and n.
    unsigned int candidate = primes.back() + 1;
    while(primes.size() <= n) {
        // Is candidate prime?  It is not if any one of the already-found
        // prime numbers (up to its square root) divides it evenly.
        bool maybe_prime = true;
        unsigned int j = 0;
        while(maybe_prime && primes[j] * primes[j] <= candidate) {
            if((primes[j] * (candidate / primes[j])) == candidate) {
                // This one is not prime.
                maybe_prime = false;
            }
            j++;
        }
        if(maybe_prime) {
            // Hey, we found a prime!
            primes.push_back(candidate);
        }
        candidate++;
    }

    return primes[n];
}


} // close namespace bamboo
