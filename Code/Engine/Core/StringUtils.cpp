#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>
#include <algorithm>
#include <windows.h>
#include "Engine/Core/Timer.hpp"
#include "Engine/Core/EngineCommon.hpp"
//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;

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
