// Filename: KeywordList.h
#pragma once
#include <string>
#include <vector>
#include <unordered_set>
namespace bamboo
{


// KeywordList this is a list of keywords (see Keyword) that may be set on a particular field.
class KeywordList
{
  public:
    KeywordList() {}
    KeywordList(const KeywordList&);
    KeywordList& operator=(KeywordList);
    friend void swap(KeywordList& lhs, KeywordList& rhs);
    virtual ~KeywordList() {} // provide vtable for python bindings

    inline bool has_keyword(const std::string& name) const
    {
        return m_keywords_by_name.find(name) != m_keywords_by_name.end();
    }
    inline bool has_matching_keywords(const KeywordList& other) const
    {
        return m_keywords_by_name == other.m_keywords_by_name; // order-independent comparison
    }
    inline size_t num_keywords() const { return m_keywords.size(); }
    inline const std::string& nth_keyword(unsigned int n) const { return m_keywords[n]; }

    void copy_keywords(const KeywordList& other);
    bool add_keyword(const std::string& keyword);

  private:
    std::vector<std::string> m_keywords;
    std::unordered_set<std::string> m_keywords_by_name;
};

void swap(KeywordList& lhs, KeywordList& rhs);


} // close namespace bamboo
