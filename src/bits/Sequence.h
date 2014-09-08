// Filename: Sequence.h
#pragma once
#include "buffers.h"
#include <stdexcept>
namespace bamboo {


/* Type Defined in Sequence.h */
template<typename T> struct Sequence;
template<typename T> class SequenceIterator;
typedef Sequence<uint8_t> Bytes;

// Sequence provides structured access to T* and T[] types.
template<typename T>
struct Sequence {
    /* Fields */
    int length;
    T* items;

    /* Declare template types */
    typedef SequenceIterator<T> iterator;
    typedef SequenceIterator<const T> const_iterator;

    /* C++ Builtins */
    size_t size_of() const { return sizeof(T) * length; }
    T& operator[](int index) { return items[index]; }
    const T& operator[](int index) const { return items[index]; }
    iterator begin() { return iterator(*this); }
    iterator end() { return iterator(*this, length); }
    const_iterator begin() const { return const_iterator(Sequence<const T>{ length, items }); }
    const_iterator end() const { return const_iterator(Sequence<const T>{ length, items }, length); }

    /* Python Builtins */
    int _len_() const { return length; }
    T _getitem_(int index) const {
        if(index < 0) { index += length; } // For seq = {1, 2, 3}: seq[-1] => 3
        if(index >= length) { throw std::out_of_range("sequence index out of range"); }
        else if(index < 0) { throw std::out_of_range("sequence index out of range"); }
        return items[index];
    }
    void _setitem_(int index, T item) {
        if(index < 0) { index += length; } // For seq = {1, 2, 3}: seq[-1] => 3
        if(index >= length) { throw std::out_of_range("sequence index out of range"); }
        else if(index < 0) { throw std::out_of_range("sequence index out of range"); }
        items[index] = item;
    }
    Sequence _getslice_(int i, int j) {
        if(i < 0) { i += length; }
        if(j < 0) { j += length; }
        else if(j > length) { j = length; }

        if(j > i) { return Sequence<T>{ j - i, &items[i] }; }
        else { return Sequence<T>{ 0, nullptr }; }
    }
    Sequence<const T> _getslice_(int i, int j) const {
        if(i < 0) { i += length; }
        if(j < 0) { j += length; }
        else if(j > length) { j = length; }

        if(j > i) { return Sequence<const T>{ j - i, &items[i] }; }
        else { return Sequence<const T>{ 0, nullptr }; }
    }
    void _setslice_(int i, int j, Sequence<T> sequence) {
        if(i < 0) { i += length; }
        if(j < 0) { j += length; }
        else if(j > length) { j = length; }

        iterator k = iterator(sequence);
        while(j > i) { items[i] = *k; ++i; ++k; }
    }
    iterator _iter_() { return iterator(*this); }
};

// SequenceIterator implements Sequence::iterator and Sequence::const_iterator
#define base_t std::iterator<std::random_access_iterator_tag, T, int>
template<typename T>
class SequenceIterator : public base_t {
  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::difference_type difference_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;

    SequenceIterator() : index(0), array{0, nullptr} {}
    SequenceIterator(Sequence<T> array, int index = 0) : index(index), array(array) {}

    #define sit_t SequenceIterator<U>
    template<typename U> SequenceIterator(const sit_t& r) : index(r.index), array(r.array) {}
    template<typename U> SequenceIterator& operator=(const sit_t& r) { return sit_t(r); }
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
    SequenceIterator& operator++() { ++index; return *this; }
    SequenceIterator& operator--() { --index; return *this; }
    SequenceIterator operator++(int) { return SequenceIterator(array, index++); }
    SequenceIterator operator--(int) { return SequenceIterator(array, index--); }
    SequenceIterator operator+(const difference_type& n) const { return SequenceIterator(array, index + n); }
    SequenceIterator& operator+=(const difference_type& n) { index += n; return *this; }
    SequenceIterator operator-(const difference_type& n) const { return SequenceIterator(array, index - n); }
    SequenceIterator& operator-=(const difference_type& n) { index -= n; return *this; }
    reference operator*() { return array[index]; }
    const reference operator*() const { return array[index]; }
    pointer operator->() { return array[index]; }
    const pointer operator->() const { return array[index]; }
    reference operator[](const difference_type& n) { return array[index + n]; }
    const reference operator[](const difference_type& n) const { return array[index + n]; }

    /* Python Iteration */
    SequenceIterator _iter_() { return *this; }
    reference next() {
        //if(index >= array.length) { raise StopIteration; }
        return array[++index];
    }
    reference prev() {
        //if(index < 0) { raise StopIteration; }
        return array[--index];
    }

  private:
    int index;
    Sequence<T> array;
};
#undef base_t

#define sit_t SequenceIterator<T>
template<typename T> bool operator==(const sit_t& r1, const sit_t& r2) { return r1.index == r2.index; }
template<typename T> bool operator!=(const sit_t& r1, const sit_t& r2) { return !operator==(r1, r2); }
template<typename T> bool operator< (const sit_t& r1, const sit_t& r2) { return r1.index <  r2.index; }
template<typename T> bool operator> (const sit_t& r1, const sit_t& r2) { return  operator< (r2, r1); }
template<typename T> bool operator<=(const sit_t& r1, const sit_t& r2) { return !operator> (r1, r2); }
template<typename T> bool operator>=(const sit_t& r1, const sit_t& r2) { return !operator< (r1, r2); }
template<typename T> typename sit_t::difference_type operator+(const sit_t& r1, const sit_t& r2) { return r1.index + r2.index; }
template<typename T> typename sit_t::difference_type operator-(const sit_t& r1, const sit_t& r2) { return r1.index - r2.index; }
#undef sit_t

} // close namespace bamboo
