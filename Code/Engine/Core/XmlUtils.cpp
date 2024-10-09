#include "XmlUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Render/SpriteAnimDefinition.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Render/Renderer.hpp"
int ParseXmlAttribute(XmlElement const& element, char const* attributeName, int defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		int value;
		if (attribute->QueryIntValue(&value) == XmlResult::XML_SUCCESS)
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the int value of the attribute");
		}
	}
	return defaultValue;
}

char ParseXmlAttribute(XmlElement const& element, char const* attributeName, char defaultValue)
{
 
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		const char* value = attribute->Value();
		if (value!=nullptr && value[0]!='\0')
		{
			return value[0];
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the chat value of the attribute");
		}
	}
	return defaultValue;
}

bool ParseXmlAttribute(XmlElement const& element, char const* attributeName, bool defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		bool value;
		if (attribute->QueryBoolValue(&value) == XmlResult::XML_SUCCESS)
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the bool value of the attribute");
		}
	}
	return defaultValue;
}

float ParseXmlAttribute(XmlElement const& element, char const* attributeName, float defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		float value;
		if (attribute->QueryFloatValue(&value) == XmlResult::XML_SUCCESS)
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the float value of the attribute");
		}
	}
	return defaultValue;
}

Rgba8 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Rgba8 const& defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		Rgba8 value;
		if (value.SetFromText(attribute->Value()))
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the correct rgb8 value of the attribute");
		}
	}
	return defaultValue;
}

Vec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec2 const& defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		Vec2 value;
		if (value.SetFromText(attribute->Value()))
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the correct Vec2 value of the attribute");
		}
	}
	return defaultValue;
}

IntVec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, IntVec2 const& defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		IntVec2 value;
		if (value.SetFromText(attribute->Value()))
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the correct IntVec2 value of the attribute");
		}
	}
	return defaultValue;
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, std::string const& defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		std::string value = attribute->Value();
		return value;
	}
// 	else
// 	{
// 		ERROR_RECOVERABLE("Can't find the string value of the attribute");
// 	}
	return defaultValue;
}

Strings ParseXmlAttribute(XmlElement const& element, char const* attributeName, Strings const& defaultValues)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		return SplitStringOnDelimiter(attribute->Value(), ';');
	}
// 	else
// 	{
// 		ERROR_RECOVERABLE("Can't find the strings of the attribute");
// 	}
	return defaultValues;
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, char const* defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		std::string value = attribute->Value();
		return value;
	}
// 	else
// 	{
// 		ERROR_RECOVERABLE("Can't find the string value of the attribute");
// 	}
	return std::string(defaultValue);
}
SpriteAnimPlaybackType ParseXmlAttribute(XmlElement const& element, char const* attributeName, SpriteAnimPlaybackType defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		std::string value = attribute->Value();
		if (value.compare("Once") == 0)
		{
			return SpriteAnimPlaybackType::ONCE;
		}
		else if (value.compare("Loop") == 0)
		{
			return SpriteAnimPlaybackType::LOOP;
		}
		else if (value.compare("PINGPONG") == 0)
		{
			return SpriteAnimPlaybackType::PINGPONG;
		}
	}
	// 	else
		// 	{
		// 		ERROR_RECOVERABLE("Can't find the string value of the attribute");
		// 	}
	return defaultValue;
}
BillboardType ParseXmlAttribute(XmlElement const& element, char const* attributeName, BillboardType defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		std::string value = attribute->Value();
		if (value.compare("FullOpposing") == 0)
		{
			return BillboardType::FULL_OPPOSING;
		}
		else if (value.compare("FullFacing") == 0)
		{
			return BillboardType::FULL_FACING;
		}
		else if (value.compare("WorldUpFacing") == 0)
		{
			return BillboardType::WORLD_UP_FACING;
		}
		else if (value.compare("WorldUpOpposing") == 0)
		{
			return BillboardType::WORLD_UP_OPPOSING;
		}
	}
	return defaultValue;
}
Vec3 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec3 const& defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		Vec3 value;
		if (value.SetFromText(attribute->Value()))
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the correct Vec3 value of the attribute");
		}
	}
	return defaultValue;
}

EulerAngles ParseXmlAttribute(XmlElement const& element, char const* attributeName, EulerAngles const& defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		EulerAngles value;
		if (value.SetFromText(attribute->Value()))
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the correct EulerAngles value of the attribute");
		}
	}
	return defaultValue;
}

FloatRange ParseXmlAttribute(XmlElement const& element, char const* attributeName, FloatRange const& defaultValue)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		FloatRange value;
		if (value.SetFromText(attribute->Value()))
		{
			return value;
		}
		else
		{
			ERROR_RECOVERABLE("Can't find the correct FloatRange value of the attribute");
		}
	}
	return defaultValue;
}

VertexType ParseXmlAttribute(XmlElement const& element, char const* attributeName, VertexType defaultType)
{
	const XmlAttribute* attribute = element.FindAttribute(attributeName);
	if (attribute != nullptr)
	{
		std::string value = attribute->Value();
		if (value.compare("Vertex_PCU") == 0)
		{
			return VertexType::Vertex_PCU;
		}
		else if (value.compare("Vertex_PCUTBN") == 0)
		{
			return VertexType::Vertex_PCUTBN;
		}
	}
	return defaultType;
}


