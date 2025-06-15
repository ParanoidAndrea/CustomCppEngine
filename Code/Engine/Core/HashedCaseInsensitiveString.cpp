#include "Engine/Core/HashedCaseInsensitiveString.hpp"

HashedCaseInsensitiveString::HashedCaseInsensitiveString(HashedCaseInsensitiveString const& copyFrom)
    :m_castInsensitiveHash(copyFrom.m_castInsensitiveHash)
    , m_orginalString(copyFrom.m_orginalString)
{

}

HashedCaseInsensitiveString::HashedCaseInsensitiveString(char const* text)
    :m_orginalString(text)
{
    m_castInsensitiveHash = GenerateCaseInsensitiveHash(text);
}

HashedCaseInsensitiveString::HashedCaseInsensitiveString(std::string const& text)
    :m_orginalString(text)
{
    m_castInsensitiveHash = GenerateCaseInsensitiveHash(text.c_str());
}

unsigned int HashedCaseInsensitiveString::GetHash() const
{
    return m_castInsensitiveHash;
}

std::string const& HashedCaseInsensitiveString::GetOriginalString() const
{
    return m_orginalString;
}

char const* HashedCaseInsensitiveString::c_str() const
{
    return m_orginalString.c_str();
}

bool HashedCaseInsensitiveString::operator==(HashedCaseInsensitiveString const& rsh) const
{
    if (m_castInsensitiveHash != rsh.m_castInsensitiveHash)
    {
        return false;
    }
    return _stricmp(m_orginalString.c_str(), rsh.m_orginalString.c_str()) == 0;
}

bool HashedCaseInsensitiveString::operator!=(HashedCaseInsensitiveString const& rsh) const
{
    if (m_castInsensitiveHash != rsh.m_castInsensitiveHash)
    {
        return true;
    }
    return _stricmp(m_orginalString.c_str(), rsh.m_orginalString.c_str()) != 0;
}

bool HashedCaseInsensitiveString::operator==(char const* text) const
{
    unsigned int textHash = GenerateCaseInsensitiveHash(text);
    if (m_castInsensitiveHash != textHash)
    {
        return false;
    }
    return _stricmp(m_orginalString.c_str(),text) == 0;
}

bool HashedCaseInsensitiveString::operator!=(char const* text) const
{
    unsigned int textHash = GenerateCaseInsensitiveHash(text);
    if (m_castInsensitiveHash != textHash)
    {
        return true;
    }
    return _stricmp(m_orginalString.c_str(), text) != 0;
}

bool HashedCaseInsensitiveString::operator==(std::string const& text) const
{
    unsigned int textHash = GenerateCaseInsensitiveHash(text.c_str());
    if (m_castInsensitiveHash != textHash)
    {
        return false;
    }
    return _stricmp(m_orginalString.c_str(), text.c_str()) == 0;
}

bool HashedCaseInsensitiveString::operator!=(std::string const& text) const
{
    unsigned int textHash = GenerateCaseInsensitiveHash(text.c_str());
    if (m_castInsensitiveHash != textHash)
    {
        return true;
    }
    return _stricmp(m_orginalString.c_str(), text.c_str()) != 0;
}

bool HashedCaseInsensitiveString::operator>(HashedCaseInsensitiveString const& rsh) const
{
    if (m_castInsensitiveHash > rsh.m_castInsensitiveHash)
    {
        return true;
    }
    if (m_castInsensitiveHash < rsh.m_castInsensitiveHash)
    {
        return false;
    }

    return _stricmp(m_orginalString.c_str(), rsh.m_orginalString.c_str()) < 0;
}

bool HashedCaseInsensitiveString::operator<(HashedCaseInsensitiveString const& rsh) const
{
    if (m_castInsensitiveHash < rsh.m_castInsensitiveHash)
    {
        return true;
    }
    if (m_castInsensitiveHash > rsh.m_castInsensitiveHash)
    {
        return false;
    }

    return _stricmp(m_orginalString.c_str(), rsh.m_orginalString.c_str()) > 0;
}

void HashedCaseInsensitiveString::operator=(char const* text)
{
    m_orginalString = text;
    m_castInsensitiveHash = GenerateCaseInsensitiveHash(text);
}

void HashedCaseInsensitiveString::operator=(std::string const& text)
{
    m_orginalString = text;
    m_castInsensitiveHash = GenerateCaseInsensitiveHash(text.c_str());
}

unsigned int HashedCaseInsensitiveString::GenerateCaseInsensitiveHash(char const* text) 
{
    unsigned int hash = 0;//32 bits
    for (char const* scan = text; *scan != '\0'; ++ scan)
    {
        //hash &= 0b0000111'11111111'11111111'11111111;
        hash *= 31; // Any prime number, can be large
        hash += (unsigned int)tolower(*scan);
    }
    return hash;
}