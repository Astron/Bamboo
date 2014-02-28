// Filename: KeywordList.cpp
#include "KeywordList.h"
namespace bamboo { // open namespace bamboo


// empty list constructor
KeywordList::KeywordList() {
}

// copy constructor
KeywordList::KeywordList(const KeywordList& copy) :
    m_keywords(copy.m_keywords), m_keywords_by_name(copy.m_keywords_by_name) {
}

// copy assignment operator
void KeywordList::operator=(const KeywordList& copy) {
    m_keywords = copy.m_keywords;
    m_keywords_by_name = copy.m_keywords_by_name;
}

// has_keyword returns true if this list includes the indicated keyword, false otherwise.
bool KeywordList::has_keyword(const std::string& name) const {
    return (m_keywords_by_name.find(name) != m_keywords_by_name.end());
}

// get_num_keywords returns the number of keywords in the list.
size_t KeywordList::get_num_keywords() const {
    return m_keywords.size();
}

// get_keyword returns the nth keyword in the list.
const std::string& KeywordList::get_keyword(unsigned int n) const {
    return m_keywords[n];
}

// has_matching_keywords returns true if this list has the same keywords as the other list,
//     false if some keywords differ. Order is not considered important.
bool KeywordList::has_matching_keywords(const KeywordList& other) const {
    return m_keywords_by_name == other.m_keywords_by_name;
}

// copy_keywords replaces this keyword list with those from the other list.
void KeywordList::copy_keywords(const KeywordList& other) {
    (*this) = other;
}

// add_keyword adds the indicated keyword to the list.
bool KeywordList::add_keyword(const std::string& keyword) {
    bool inserted = m_keywords_by_name.insert(keyword).second;
    if(inserted) {
        m_keywords.push_back(keyword);
    }

    return inserted;
}


} // close namespace bamboo
