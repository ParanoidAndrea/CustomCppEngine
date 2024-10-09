#include "NamedStrings.hpp"

void NamedStrings::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	const XmlAttribute* attribute = element.FirstAttribute();
	while (attribute != nullptr)
	{
		std::string name = attribute->Name();
		std::string value = attribute->Value();
		m_keyValuePairs[name] = value;
		attribute = attribute->Next();
	}
}

void NamedStrings::SetValue(std::string const& keyName, std::string const& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}

std::string NamedStrings::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		return found->second;
	}
	return defaultValue;
}

bool NamedStrings::GetValue(std::string const& keyName, bool defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		if (found->second == "true")
		{
			return true;
		}
		else if (found->second == "false")
		{
			return false;
		}
	}
	return defaultValue;
}

int NamedStrings::GetValue(std::string const& keyName, int defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		int value = atoi(found->second.c_str());
		return value;
	}
	return defaultValue;
}

float NamedStrings::GetValue(std::string const& keyName, float defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		float value = static_cast<float>(atof(found->second.c_str()));
		return value;
	}
	return defaultValue;
}

std::string NamedStrings::GetValue(std::string const& keyName, char const* defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		return found->second;
	}
	return std::string(defaultValue);
}

Rgba8 NamedStrings::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		Rgba8 value; 
		if (value.SetFromText(found->second.c_str()))
		{
			return value;
		}
	}
	return defaultValue;
}

Vec2 NamedStrings::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		Vec2 value;
		if (value.SetFromText(found->second.c_str()))
		{
			return value;
		}
	}
	return defaultValue;
}

IntVec2 NamedStrings::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	auto found = m_keyValuePairs.find(keyName);
	if (found != m_keyValuePairs.end())
	{
		IntVec2 value;
		if (value.SetFromText(found->second.c_str()))
		{
			return value;
		}
	}
	return defaultValue;
}
