#pragma once
#include <string>
class HashedCaseInsensitiveString
{
public:
    HashedCaseInsensitiveString() = default;
    HashedCaseInsensitiveString(HashedCaseInsensitiveString const& copyFrom);
    HashedCaseInsensitiveString(char const* text);
    HashedCaseInsensitiveString(std::string const& text);

    unsigned int GetHash() const;
    std::string const& GetOriginalString() const;
    char const* c_str() const;

    bool operator == (HashedCaseInsensitiveString const& rsh) const;
    bool operator != (HashedCaseInsensitiveString const& rsh) const;
    bool operator == (char const* text) const;
    bool operator != (char const* text) const;
    bool operator == (std::string const& text) const;
    bool operator != (std::string const& text) const;
    bool operator > (HashedCaseInsensitiveString const& rsh) const;
    bool operator < (HashedCaseInsensitiveString const& rsh) const;

    void operator= (char const* text);
    void operator= (std::string const& text);

    static unsigned int GenerateCaseInsensitiveHash(char const* text);

private:
    std::string m_orginalString;
    unsigned int m_castInsensitiveHash = 0;
};