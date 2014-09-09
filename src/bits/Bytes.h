// Filename: binary.h
#pragma once
#include <stdexcept>
#include <string>
#include <type_traits>
namespace bamboo {


// Foward declare
template<typename T> struct binary;
template<typename T> class BytesIterator;
typedef binary<uint8_t> Bytes;
typedef binary<char> Ascii;

// binary provides structured access to T* and T[] types.
template<typename T>
struct binary {
    static_assert(std::is_pod<T>::value && sizeof(T) == 1, "Binary type must consist of bytes");

    /* Fields */
    int len;
    T* bytes;

    /* Typedefs */
    typedef BytesIterator<T> iterator;
    typedef BytesIterator<const T> const_iterator;

    /* C++ Builtins */
    int length() const { return len; }
    size_t size_of() const { return sizeof(T) * len; }
    T& operator[](int index) { return bytes[index]; }
    const T& operator[](int index) const { return bytes[index]; }
    iterator begin() { return iterator(*this); }
    iterator end() { return iterator(*this, len); }
    const_iterator begin() const { return const_iterator(*this); }
    const_iterator end() const { return const_iterator(*this, len); }

    /* Conversion */
    operator std::string() const { return std::string((const char*)bytes, len); }
    // Implict conversion of const binary<T> to binary<const T>
    operator binary<const T>() const { return binary<const T>{len, bytes}; }

    /* Python Builtins */
    int _len_() const { return len; }
    T _getitem_(int index) const {
        if(index < 0) { index += len; } // For seq = {1, 2, 3}: seq[-1] => 3
        if(index >= len) { throw std::out_of_range("sequence index out of range"); }
        else if(index < 0) { throw std::out_of_range("sequence index out of range"); }
        return bytes[index];
    }
    void _setitem_(int index, T byte) {
        if(index < 0) { index += len; } // For seq = {1, 2, 3}: seq[-1] => 3
        if(index >= len) { throw std::out_of_range("sequence index out of range"); }
        else if(index < 0) { throw std::out_of_range("sequence index out of range"); }
        bytes[index] = byte;
    }
    binary<T> _getslice_(int i, int j) {
        if(i < 0) { i += len; }
        if(j < 0) { j += len; }
        else if(j > len) { j = len; }

        if(j > i) { return binary<T>{ j - i, &bytes[i] }; }
        else { return binary<T>{ 0, nullptr }; }
    }
    binary<const T> _getslice_(int i, int j) const {
        if(i < 0) { i += len; }
        if(j < 0) { j += len; }
        else if(j > len) { j = len; }

        if(j > i) { return binary<const T>{ j - i, &bytes[i] }; }
        else { return binary<const T>{ 0, nullptr }; }
    }
    void _setslice_(int i, int j, binary<T> sequence) {
        if(i < 0) { i += len; }
        if(j < 0) { j += len; }
        else if(j > len) { j = len; }

        iterator k = iterator(sequence);
        while(j > i) { bytes[i] = *k; ++i; ++k; }
    }
    iterator _iter_() { return iterator(*this); }
    std::string _add_(const std::string& rhs) const { return *this + rhs; }
    std::string _radd_(const std::string& lhs) const { return lhs + *this; }
};

template<typename T> std::string operator+(const binary<T>& lhs, const std::string& rhs) {
  return std::string(lhs) + rhs;
}
template<typename T> std::string operator+(const std::string& lhs, const binary<T>& rhs) {
  return lhs + std::string(rhs);
}

// BytesIterator implements binary::iterator and binary::const_iterator
#define base_t std::iterator<std::random_access_iterator_tag, T, int>
template<typename T>
class BytesIterator : public base_t {
  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::difference_type difference_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;

    BytesIterator() : index(0), array{0, nullptr} {}
    BytesIterator(binary<T> array, int index = 0) : index(index), array(array) {}

    #define sit_t BytesIterator<U>
    template<typename U> BytesIterator(const sit_t& r) : index(r.index), array(r.array) {}
    template<typename U> BytesIterator& operator=(const sit_t& r) { return sit_t(r); }
    template<typename U> friend bool operator==(const sit_t&, const sit_t&);
    template<typename U> friend bool operator!=(const sit_t&, const sit_t&);
    template<typename U> friend bool operator< (const sit_t&, const sit_t&);
    template<typename U> friend bool operator> (const sit_t&, const sit_t&);
    template<typename U> friend bool operator<=(const sit_t&, const sit_t&);
    template<typename U> friend bool operator>=(const sit_t&, const sit_t&);
    template<typename U> friend typename sit_t::difference_type operator+(const sit_t&, const sit_t&);
    template<typename U> friend typename sit_t::difference_type operator-(const sit_t&, const sit_t&);
    #undef sit_t

    /* C++ Iteration */
    BytesIterator& operator++() { ++index; return *this; }
    BytesIterator& operator--() { --index; return *this; }
    BytesIterator operator++(int) { return BytesIterator(array, index++); }
    BytesIterator operator--(int) { return BytesIterator(array, index--); }
    BytesIterator operator+(const difference_type& n) const { return BytesIterator(array, index + n); }
    BytesIterator& operator+=(const difference_type& n) { index += n; return *this; }
    BytesIterator operator-(const difference_type& n) const { return BytesIterator(array, index - n); }
    BytesIterator& operator-=(const difference_type& n) { index -= n; return *this; }
    reference operator*() { return array[index]; }
    const reference operator*() const { return array[index]; }
    pointer operator->() { return array[index]; }
    const pointer operator->() const { return array[index]; }
    reference operator[](const difference_type& n) { return array[index + n]; }
    const reference operator[](const difference_type& n) const { return array[index + n]; }

    /* Python Iteration */
    BytesIterator _iter_() { return *this; }
    reference next() {
        //if(index >= array.len) { raise StopIteration; }
        return array[++index];
    }
    reference prev() {
        //if(index < 0) { raise StopIteration; }
        return array[--index];
    }

  private:
    int index;
    binary<T> array;
};
#undef base_t

#define sit_t BytesIterator<T>
template<typename T> bool operator==(const sit_t& r1, const sit_t& r2) { return r1.index == r2.index; }
template<typename T> bool operator!=(const sit_t& r1, const sit_t& r2) { return !operator==(r1, r2); }
template<typename T> bool operator< (const sit_t& r1, const sit_t& r2) { return r1.index <  r2.index; }
template<typename T> bool operator> (const sit_t& r1, const sit_t& r2) { return  operator< (r2, r1); }
template<typename T> bool operator<=(const sit_t& r1, const sit_t& r2) { return !operator> (r1, r2); }
template<typename T> bool operator>=(const sit_t& r1, const sit_t& r2) { return !operator< (r1, r2); }
template<typename T> typename sit_t::difference_type operator+(const sit_t& r1, const sit_t& r2) { return r1.index + r2.index; }
template<typename T> typename sit_t::difference_type operator-(const sit_t& r1, const sit_t& r2) { return r1.index - r2.index; }
#undef sit_t


/* Static assertions */
// Verifies we don't accidently complicate the type
static_assert(std::is_pod<Bytes>::value,
    "A binary of fundamental types should be Plain Old Data.");
static_assert(std::is_pod<Ascii>::value,
    "A binary of fundamental types should be Plain Old Data.");


} // close namespace bamboo
