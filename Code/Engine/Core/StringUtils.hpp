#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>
typedef std::vector< std::string >		Strings;

Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn = ',', bool removeEmpty = false);
int SplitStringOnDelimiter(Strings & out_splitString, std::string const& originalString, std::string const& delimiterToSplitOn);
Strings SplitStringOnDelimiter(std::string const& originalString, std::string const& delimiterToSplitOn, bool removeEmpty);

//Stringf("CreateShaderResourceView failed for image file \"%s\".", image.GetImageFilePath().c_str())
//-----------------------------------------------------------------------------------------------

const std::string Stringf( char const* format, ... );
const std::string Stringf( int maxLength, char const* format, ... );
std::string AllLowerCase(std::string stringToLowerCase);
bool IsEqualWithoutProperCase(std::string string1,std::string string2);
void PrintTextToDebug(std::string const& message);
void PrintLineToDebug(std::string const& message);

