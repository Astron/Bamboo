// Filename: KeywordList.h
#pragma once
#include <string>        // std::string
#include <vector>        // std::vector
#include <unordered_set> // std::unordered_set
namespace bamboo   // open namespace bamboo
{


// Forward declaration
class HashGenerator;

// KeywordList this is a list of keywords (see Keyword) that may be set on a particular field.
class KeywordList
{
  public:
    KeywordList();
    KeywordList(const KeywordList&);
    KeywordList& operator=(KeywordList);
    friend void swap(KeywordList& lhs, KeywordList& rhs);
    virtual ~KeywordList() {}

    // has_keyword returns true if this list includes the indicated keyword, false otherwise.
    bool has_keyword(const std::string& name) const;
    // num_keywords returns the number of keywords in the list.
    size_t num_keywords() const;
    // get_keyword returns the nth keyword in the list.
    const std::string& get_keyword(unsigned int n) const;

    // has_matching_keywords returns true if this list has the same keywords as the other list,
    //     false if some keywords differ. Order is not considered important.
    bool has_matching_keywords(const KeywordList& other) const;

    // copy_keywords replaces this keyword list with those from the other list.
    void copy_keywords(const KeywordList& other);

    // add_keyword adds the indicated keyword to the list.
    //     Returns true if it is added, false if it was already there.
    bool add_keyword(const std::string& keyword);

  private:
    std::vector<std::string> m_keywords; // the actual list of keywords
    std::unordered_set<std::string> m_keywords_by_name; // a map of name to keywords in list
};


void swap(KeywordList& lhs, KeywordList& rhs);


} // close namespace bamboo
