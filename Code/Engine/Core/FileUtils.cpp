#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& filename)
{
	FILE* file;
	if (fopen_s(&file, filename.c_str(), "rb") != 0)
	{
		ERROR_AND_DIE(Stringf("Failed to Read the file located in %s", filename.c_str()));
	}

	// Seek to the end of the file to get its size
	if (fseek(file, 0, SEEK_END) != 0)
	{
		ERROR_RECOVERABLE("Failed to seek to the end");
		fclose(file);
		return -1;
	}
	long size = ftell(file);
	if (size == -1L)
	{
		ERROR_RECOVERABLE("The size of the file is invaild");
		fclose(file);
		return -1;
	}
	outBuffer.resize(size);

	if (fseek(file, 0, SEEK_SET) != 0)
	{
		ERROR_RECOVERABLE("Failed to seek to the beginning");
		fclose(file);
		return -1;
	}

	size_t bytesRead = fread(outBuffer.data(), 1, size, file);
	fclose(file);

	if (bytesRead != (size_t)size)
	{
		ERROR_RECOVERABLE("Failed to read the expected number of bytes");
		return -1;
	}

	return (int)bytesRead;
}

int FileReadToString(std::string& outString, std::string const& filename)
{
	std::vector<uint8_t> outBuffer;
	int bytesRead = FileReadToBuffer(outBuffer, filename);
	if (bytesRead == -1)
	{
		ERROR_RECOVERABLE(Stringf("Failed to Read the file located in %s", filename.c_str()));
		return -1;
	}
	outBuffer.push_back('\0');
	outString = (char*)(outBuffer.data());
	return (int)outString.length();
}

bool FileWriteToBuffer(std::vector<uint8_t> const& outBuffer, std::string const& filename)
{
	FILE* file = nullptr;
	errno_t result = fopen_s(&file, filename.c_str(), "wb");
	if (result != 0 || file == nullptr)
	{
		return false;
	}
	fwrite(outBuffer.data(), 1, outBuffer.size(), file);
	fclose(file);
	return true;
}

bool IsFileExists(std::string const& fileName)
{
	FILE* file = nullptr;
	errno_t result = fopen_s(&file, fileName.c_str(), "rb");
	if (result != 0 || file == nullptr)
	{
		return false;
		
	}
	fclose(file);
	return true;
}
