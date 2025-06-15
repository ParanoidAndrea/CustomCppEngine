#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>
#include <algorithm>
#include <windows.h>
#include "Engine/Core/Timer.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/EulerAngles.hpp"
//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;

int StringToValue(std::string const& str, int defailtValue)
{
	char* endptr;
	long num = strtol(str.c_str(), &endptr, 10);
	if (*endptr != '\0') //Invalid input: The string contains non-numeric characters
	{
		return defailtValue;
	}
	return num;
}

float StringToValue(std::string const& str, float defailtValue)
{
	char* endptr;
	float num = strtof(str.c_str(), &endptr);
	if (*endptr != '\0')
	{
		return defailtValue;
	}
	return num;
}

bool StringToValue(std::string const& str, bool defailtValue)
{
	if (_stricmp("true", str.c_str()) == 0)
	{
		return true;
	}
	else if (_stricmp("false", str.c_str()) == 0)
	{
		return false;
	}
	return defailtValue;
}

Vec2 StringToValue(std::string const& str, Vec2 defailtValue)
{
	Vec2 value;
    Strings resultStrings = SplitStringOnDelimiter(str, ',');
    int numStrings = (int)resultStrings.size();
    if (numStrings != 2)
    {
        return defailtValue;
    }
    value.x = static_cast<float>(atoi(resultStrings[0].c_str()));
    value.y = static_cast<float>(atoi(resultStrings[1].c_str()));
	return value;
}

Vec3 StringToValue(std::string const& str, Vec3 defailtValue)
{
	Vec3 value;
    Strings resultStrings = SplitStringOnDelimiter(str, ',');
    int numStrings = (int)resultStrings.size();
    if (numStrings != 3)
    {
        return defailtValue;
    }
    value.x = static_cast<float>(atof(resultStrings[0].c_str()));
    value.y = static_cast<float>(atof(resultStrings[1].c_str()));
    value.z = static_cast<float>(atof(resultStrings[2].c_str()));
	return value;
}

Rgba8 StringToValue(std::string const& str, Rgba8 defailtValue)
{
	Rgba8 value;
    Strings resultStrings = SplitStringOnDelimiter(str, ',');

    if (resultStrings.size() == 3 || resultStrings.size() == 4)
    {
        value.r = static_cast<unsigned char>(atoi(resultStrings[0].c_str()));
        value.g = static_cast<unsigned char>(atoi(resultStrings[1].c_str()));
        value.b = static_cast<unsigned char>(atoi(resultStrings[2].c_str()));

        if (resultStrings.size() == 4)
        {
            value.a = static_cast<unsigned char>(atoi(resultStrings[3].c_str()));
        }
        else
        {
            value.a = 255;
        }
        return value;
    }
    else
    {
        return defailtValue;
    }
}

IntVec2 StringToValue(std::string const& str, IntVec2 defailtValue)
{
    IntVec2 value;
    Strings resultStrings = SplitStringOnDelimiter(str, ',');
    int numStrings = (int)resultStrings.size();
    if (numStrings != 2)
    {
        return defailtValue;
    }
    value.x = static_cast<int>(atoi(resultStrings[0].c_str()));
    value.y = static_cast<int>(atoi(resultStrings[1].c_str()));
    return value;
}

EulerAngles StringToValue(std::string const& str, EulerAngles defailtValue)
{
	EulerAngles value;
    Strings resultStrings = SplitStringOnDelimiter(str, ',');
    int numStrings = (int)resultStrings.size();
    if (numStrings != 3)
    {
        return defailtValue;
    }
    value.m_yawDegrees = static_cast<float>(atof(resultStrings[0].c_str()));
    value.m_pitchDegrees = static_cast<float>(atof(resultStrings[1].c_str()));
    value.m_rollDegrees = static_cast<float>(atof(resultStrings[2].c_str()));
    return value;
}

const std::string Stringf(char const* format, ...)
{
	char textLiteral[STRINGF_STACK_LOCAL_TEMP_LENGTH];
	va_list variableArgumentList;
	va_start(variableArgumentList, format);
	vsnprintf_s(textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList);
	va_end(variableArgumentList);
	textLiteral[STRINGF_STACK_LOCAL_TEMP_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string(textLiteral);
}


const std::string Stringf(int maxLength, char const* format, ...)
{
	char textLiteralSmall[STRINGF_STACK_LOCAL_TEMP_LENGTH];
	char* textLiteral = textLiteralSmall;
	if (maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH)
		textLiteral = new char[maxLength];

	va_list variableArgumentList;
	va_start(variableArgumentList, format);
	vsnprintf_s(textLiteral, maxLength, _TRUNCATE, format, variableArgumentList);
	va_end(variableArgumentList);
	textLiteral[maxLength - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue(textLiteral);
	if (maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH)
		delete[] textLiteral;

	return returnValue;
}
void TrimString(std::string& originalString, char delimiterToTrim)
{
	originalString.erase(std::remove(originalString.begin(), originalString.end(), delimiterToTrim), originalString.end());
}

Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn, bool removeEmpty)
{
	Strings resultStrings;
	std::string splitedString;
	for (size_t i = 0; i < originalString.size(); i++)
	{
		if (originalString[i] != delimiterToSplitOn)
		{
			if (!removeEmpty ||(removeEmpty && originalString[i] != ' '))
			{
				splitedString += originalString[i];
			}
		}
		else
		{
			if (!splitedString.empty() || !removeEmpty)
			{
				resultStrings.push_back(splitedString);
			}
			splitedString.clear();
		}
	}
	if (!splitedString.empty() || !removeEmpty)
	{
		resultStrings.push_back(splitedString);
	}
	return resultStrings;
}

int SplitStringOnDelimiter(Strings& out_splitString, std::string const& originalString, std::string const& delimiterToSplitOn)
{
	out_splitString.clear();
	size_t pos = 0;
	while (pos != std::string::npos)
	{
		size_t nextPos = originalString.find(delimiterToSplitOn, pos);
		if (nextPos == std::string::npos)
		{
			out_splitString.push_back(originalString.substr(pos));
			break;
		}
		out_splitString.push_back(originalString.substr(pos, nextPos - pos));
		pos = nextPos + delimiterToSplitOn.length();
	}
	return (int)out_splitString.size();
}

Strings SplitStringOnDelimiter(std::string const& originalString, std::string const& delimiterToSplitOn, bool removeEmpty)
{
	Strings resultStrings;
	std::string currentString;
	size_t delimiterLength = delimiterToSplitOn.length();
	if (delimiterLength == 1)
	{
		char delimiter = delimiterToSplitOn.c_str()[0];
		SplitStringOnDelimiter(originalString, delimiter, removeEmpty);
	}
	for (size_t i = 0; i < originalString.size(); ++i)
	{
		if (originalString[i] == '#')
		{
			while (i < originalString.size() && originalString[i] != '\n') 
			{
				++i;
			}
			continue;
		}

		if ( i + delimiterLength > originalString.size() ||
			originalString.substr(i, delimiterLength) != delimiterToSplitOn)
		{
			currentString += originalString[i];
		}
		else
		{
			if (!currentString.empty() || !removeEmpty)
			{
				resultStrings.push_back(currentString);
			}
			currentString.clear();
			i += delimiterLength - 1; // Skip the delimiter
		}
		
	}

	if (!currentString.empty() || !removeEmpty)
	{
		resultStrings.push_back(currentString);
	}

	return resultStrings;
}



std::string const ToString(int interger)
{
	return Stringf("%d", interger);
}

std::string const ToString(float value)
{
	return Stringf("%f", value);
}

std::string const ToString(bool value)
{
	if (value)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

std::string const ToString(Vec2 value)
{
	return Stringf("%f,%f", value.x, value.y);
}

std::string const ToString(Vec3 value)
{
	return Stringf("%f,%f,%f", value.x, value.y, value.z);
}

std::string const ToString(Rgba8 value)
{
	return Stringf("%c,%c,%c,%c", value.r, value.g, value.b, value.a);
}

std::string const ToString(IntVec2 value)
{
	return Stringf("%d,%d", value.x, value.y);
}

std::string const ToString(EulerAngles value)
{
	return Stringf("%f,%f,%f", value.m_yawDegrees, value.m_pitchDegrees, value.m_rollDegrees);
}

std::string	const ToString(std::string value)
{
	return value;
}

Strings SplitStringWithQuotes(std::string const& originalString, char delimiterToSplitOn, bool IsKeepQuote)
{
	Strings resultStrings;
	std::string currentToken;
	bool inQuotes = false;
	for (size_t i = 0; i < originalString.size(); ++i)
	{
		if (originalString[i] == '"')
		{
			inQuotes = !inQuotes;
			if (IsKeepQuote)
			{
				currentToken.push_back(originalString[i]);
			}
		}
		else if (originalString[i] == delimiterToSplitOn && !inQuotes)
		{
			resultStrings.push_back(currentToken);
			currentToken.clear();
		}
		else
		{
			currentToken.push_back(originalString[i]);
		}
	}
	resultStrings.push_back(currentToken);
	return resultStrings;
}




std::string AllLowerCase(std::string const& stringToLowerCase)
{
	std::string lowerCaseString = stringToLowerCase;
	std::transform(lowerCaseString.begin(), lowerCaseString.end(), lowerCaseString.begin(), [](unsigned char c)->unsigned char { return (unsigned char)std::tolower(c); });
	return lowerCaseString;
}
bool IsEqualWithoutProperCase(std::string string1, std::string string2)
{
	return AllLowerCase(string1) == AllLowerCase(string2);
}

void PrintTextToDebug(std::string const& message)
{
	OutputDebugStringA(message.c_str());
}

void PrintLineToDebug(std::string const& message)
{
	OutputDebugStringA(message.c_str());
	OutputDebugStringA("\n");
}
