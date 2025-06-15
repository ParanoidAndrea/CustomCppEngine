#include "Engine/Core/NamedProperties.hpp"
#include "Engine/Core/EventSystem.hpp"

std::map<HashedCaseInsensitiveString, TypePropertyBase*> const NamedProperties::GetProperties() const
{
    return m_properties;
}

NamedProperties::~NamedProperties()
{
    for (auto iter : m_properties) 
    {
        delete iter.second;
    }

    m_properties.clear();
}


void NamedProperties::PopulateFromXmlElementAttributes(XmlElement const& element)
{
    const XmlAttribute* attribute = element.FirstAttribute();
    while (attribute != nullptr)
    {
        SetValue(attribute->Name(), attribute->Value());
        attribute = attribute->Next();
    }
}

std::string NamedProperties::GetValue(std::string const& keyNameString, char const* val) const
{
    return GetValue<std::string>(keyNameString, val);
}

void NamedProperties::SetValue(std::string const& keyName, char const* value)
{
    SetValue<std::string>(keyName, value);
}
