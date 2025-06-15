#pragma once
#include <map>
#include "Engine/Core/XmlUtils.hpp"
struct CaseInsensitiveComparator 
{
	bool operator()(const std::string& s1, const std::string& s2) const 
	{
		return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),
			[](char c1, char c2) { return std::tolower(c1) < std::tolower(c2); });
	}
};

class NamedStrings
{
public:
	void			PopulateFromXmlElementAttributes(XmlElement const& element);
	void			SetValue(std::string const& keyName, std::string const& newValue);
	std::string		GetValue(std::string const& keyName, std::string const& defaultValue) const;
	bool			GetValue(std::string const& keyName, bool defaultValue) const;
	int			    GetValue(std::string const& keyName, int defaultValue) const;
	float			GetValue(std::string const& keyName, float defaultValue) const;
	std::string		GetValue(std::string const& keyName, char const* defaultValue) const;
	Rgba8			GetValue(std::string const& keyName, Rgba8 const& defaultValue) const;
	Vec2			GetValue(std::string const& keyName, Vec2 const& defaultValue) const;
	Vec3			GetValue(std::string const& keyName, Vec3 const& defaultValue) const;
	IntVec2		    GetValue(std::string const& keyName, IntVec2 const& defaultValue) const;
	EulerAngles	    GetValue(std::string const& keyName, EulerAngles const& defaultValue) const;
private:
	std::map< std::string, std::string, CaseInsensitiveComparator >	m_keyValuePairs;

};