#pragma once
#include "Engine/Core/StringUtils.hpp"
#include <map>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/HashedCaseInsensitiveString.hpp"
#include "Engine/Core/XmlUtils.hpp"
class TypePropertyBase
{
public:
    virtual ~TypePropertyBase() = default;
    virtual std::string const GetAsString() const = 0;
    virtual TypePropertyBase* Clone() const = 0;
};

template<typename T>
class TypeProperty : public TypePropertyBase
{
    friend class NamedProperties;
private:
    TypeProperty(T const& initializeValue)
        :m_data(initializeValue)
    {

    }
    virtual std::string const GetAsString() const override
    { 
        return ToString(m_data); 
    }

    virtual TypeProperty<T>* Clone() const override 
    {
        return new TypeProperty<T>(m_data);
    }
private:
    T m_data;
};

class NamedProperties
{
public:
    NamedProperties() = default;
    NamedProperties(NamedProperties const& copy)
    {
        for (auto& pair : copy.m_properties) 
        {
            m_properties[pair.first] = pair.second->Clone();
        }
    }
    std::map<HashedCaseInsensitiveString, TypePropertyBase*> const GetProperties() const;
    ~NamedProperties();
    void PopulateFromXmlElementAttributes(XmlElement const& element);
    template<typename T>
    void SetValue(std::string const& keyName, T const& value);
    template<typename T>
    T GetValue(std::string const& keyNameString, T const& defaultValue) const;

    void SetValue(std::string const& keyName, char const* value);
    std::string GetValue(std::string const& keyNameString, char const* val) const;
private:
    std::map<HashedCaseInsensitiveString, TypePropertyBase*> m_properties;

};

template<typename T>
void NamedProperties::SetValue(std::string const& keyName, T const& value)
{
    //HashedCaseInsensitiveString keyName(keyNameString);
    auto found = m_properties.find(keyName);
    if (found == m_properties.end()) // the value doesn't exit yet; 
    {
        HashedCaseInsensitiveString hashKey(keyName);
        m_properties[hashKey] = new TypeProperty<T>(value);
    }
    else //Already Exit
    {
        
        TypeProperty<T>* asSameType = dynamic_cast<TypeProperty<T>*> (found->second);
        if (found->first.GetOriginalString().compare(keyName) == 0)
        {
            if (asSameType != nullptr)
            {
                asSameType->m_data = value;
            }
            else
            {
                delete found->second;
                m_properties[keyName] = new TypeProperty<T>(value);
            }
        }
        else
        {
            HashedCaseInsensitiveString hashKey(keyName);
            m_properties[hashKey] = new TypeProperty<T>(value);
        }
    }
}

template<typename T>
T NamedProperties::GetValue(std::string const& keyNameString, T const& defaultValue) const
{
    HashedCaseInsensitiveString keyName(keyNameString);
    auto found = m_properties.find(keyName);
    if (found == m_properties.end())
    {
        return defaultValue;
    }
    else
    {

        TypeProperty<T>* asTypeCastPtr = dynamic_cast<TypeProperty<T>*> (found->second);
        if (asTypeCastPtr != nullptr)
        {
            return asTypeCastPtr->m_data;
        }
        else
        {
            TypeProperty<std::string>* asStringPtr = dynamic_cast<TypeProperty<std::string>*>(found->second);
            if (asStringPtr == nullptr)
            {
                ERROR_RECOVERABLE(Stringf("Asked for NamedProperty failed"));
                return defaultValue;
            }
            else
            {
                return StringToValue(asStringPtr->m_data, defaultValue);
            }
        }
    }
}
