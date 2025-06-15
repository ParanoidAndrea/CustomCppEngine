#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>
#include "Engine/Core/ErrorWarningAssert.hpp"
typedef std::vector< std::string >		Strings;
struct Vec2;
struct Vec3;
struct Rgba8;
struct IntVec2;
struct EulerAngles;
class TypePropertyBase;
Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn = ',', bool removeEmpty = false);
int SplitStringOnDelimiter(Strings & out_splitString, std::string const& originalString, std::string const& delimiterToSplitOn);
Strings SplitStringOnDelimiter(std::string const& originalString, std::string const& delimiterToSplitOn, bool removeEmpty);

template<typename T>
std::string const ToString(T value);
std::string const ToString(int interger);
std::string const ToString(float value);
std::string const ToString(bool value);
std::string const ToString(Vec2 value);
std::string const ToString(Vec3 value);
std::string const ToString(Rgba8 value);
std::string const ToString(IntVec2 value);
std::string	const ToString(EulerAngles value);
std::string	const ToString(std::string value);

template<typename T>
std::string const ToString(T value)
{
    (void)(value);
    return "";
}

template<typename T>
T           StringToValue(std::string const& str, T defaultValue);
int         StringToValue(std::string const& str, int defailtValue);
float       StringToValue(std::string const& str, float defailtValue);
bool        StringToValue(std::string const& str, bool defailtValue);
Vec2        StringToValue(std::string const& str, Vec2 defailtValue);
Vec3        StringToValue(std::string const& str, Vec3 defailtValue);
Rgba8       StringToValue(std::string const& str, Rgba8 defailtValue);
IntVec2	    StringToValue(std::string const& str, IntVec2 defailtValue);
EulerAngles	StringToValue(std::string const& str, EulerAngles defailtValue);

template<typename T>
T StringToValue(std::string const& str, T defaultValue)
{
    (void)(str);
    ERROR_RECOVERABLE("Can't fetch std::string data value as other types!");
    return defaultValue;
}

//Stringf("CreateShaderResourceView failed for image file \"%s\".", image.GetImageFilePath().c_str())
//-----------------------------------------------------------------------------------------------

const std::string Stringf( char const* format, ... );
const std::string Stringf( int maxLength, char const* format, ... );
std::string AllLowerCase(std::string const& stringToLowerCase);
void TrimString(std::string& originalString, char delimiterToTrim);
Strings SplitStringWithQuotes(std::string const& originalString, char delimiterToSplitOn, bool IsKeepQuote = false);
bool IsEqualWithoutProperCase(std::string string1,std::string string2);
void PrintTextToDebug(std::string const& message);
void PrintLineToDebug(std::string const& message);

