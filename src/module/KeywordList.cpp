// Filename: KeywordList.cpp
#include "KeywordList.h"
using namespace std;
namespace bamboo
{


KeywordList::KeywordList(const KeywordList& other) :
    m_keywords(other.m_keywords),
    m_keywords_by_name(other.m_keywords_by_name)
{
}

KeywordList& KeywordList::operator=(KeywordList rhs)
{
    swap(*this, rhs);
    return *this;
}

void KeywordList::copy_keywords(const KeywordList& other)
{
    (*this) = other;
}

bool KeywordList::add_keyword(const string& keyword)
{
    bool inserted = m_keywords_by_name.insert(keyword).second;
    if(inserted) { m_keywords.push_back(keyword); }
    return inserted;
}

void swap(KeywordList& lhs, KeywordList& rhs)
{
    using std::swap;

    swap(lhs.m_keywords, rhs.m_keywords);
    swap(lhs.m_keywords_by_name, rhs.m_keywords_by_name);
}


} // close namespace bamboo
