#pragma once
#include <vector>
#include <string>
int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& filename);
int FileReadToString(std::string& outString, std::string const& filename);
bool FileWriteToBuffer(std::vector<uint8_t> const& outBuffer, std::string const& filename);
bool IsFileExists(std::string const& fileName);