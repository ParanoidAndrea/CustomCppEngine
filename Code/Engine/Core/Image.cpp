#include "Engine/Core/Image.hpp"
#define STB_IMAGE_IMPLEMENTATION // Exactly one .CPP (this Image.cpp) should #define this before #including stb_image.h
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
Image::Image()
{
}
Image::~Image()
{
	m_rgbaTexels.clear();
}
Image::Image(char const* imageFilePath)
	:m_imageFilePath(std::string(imageFilePath))
{
	stbi_set_flip_vertically_on_load(1);
	int width, height, numsColorChannels;
	unsigned char* imageData = stbi_load(imageFilePath, &width, &height, &numsColorChannels, STBI_rgb_alpha);

	if (imageData)
	{
		m_dimensions = IntVec2(width, height);
		size_t texelCount = (size_t)width * (size_t)height;
		m_rgbaTexels.resize(texelCount);

		// Since we requested STBI_rgb_alpha, we know numsColorChannels will be 4
		memcpy(m_rgbaTexels.data(), imageData, texelCount * 4);

		stbi_image_free(imageData);
	}
	else
	{
		ERROR_AND_DIE(Stringf("Couldn't loaded the image for the file path: %s", imageFilePath));
	}
	// Free the raw image texel data now that we've sent a copy of it down to the GPU to be stored in video memory
	
}

Image::Image(IntVec2 size, Rgba8 color)
	:m_dimensions(size)
{
	int dataSize = m_dimensions.x * m_dimensions.y;
	m_rgbaTexels.reserve(dataSize);
	for (int i = 0; i <dataSize ; i++)
	{
		m_rgbaTexels.push_back(color);
	}
}

std::string const& Image::GetImageFilePath() const
{
	return m_imageFilePath;
}

IntVec2 Image::GetDimensions() const
{
	return m_dimensions;
}

const void* Image::GetRawData() const
{
	return (void*)m_rgbaTexels.data();
}

 unsigned char const* Image::GetRawCharData() const
{
    return reinterpret_cast<const unsigned char*>(m_rgbaTexels.data());
}

Rgba8 Image::GetTexelColor(IntVec2 const& texelCoords) const
{
	int texelIndex = texelCoords.y * m_dimensions.x + texelCoords.x;
	if (texelIndex >= 0 && texelIndex <= (int)m_rgbaTexels.size())
	{
		return m_rgbaTexels[texelIndex];
	}
	return Rgba8::WHITE;
}

void Image::SetTexelColor(IntVec2 const& texelCoords, Rgba8 const& newColor)
{
	int texelIndex = texelCoords.y * m_dimensions.x + texelCoords.x;
	if (texelIndex >= 0 && texelIndex <= (int)m_rgbaTexels.size())
	{
		m_rgbaTexels[texelIndex] = newColor;
	}
	else
	{
		if (m_dimensions.x <= texelCoords.x + 1)
		{
			m_dimensions.x = texelCoords.x +1;
		}
		if (m_dimensions.y <= texelCoords.y + 1)
		{
			m_dimensions.y = texelCoords.y + 1;
		}
		int newTexelCounts = m_dimensions.x * m_dimensions.y;
		m_rgbaTexels.resize(newTexelCounts, Rgba8(0,0,0,255));
		m_rgbaTexels[texelIndex] = newColor;
	}
}

Image* Image::CreatePaddedImage(int padding) const
{
    // Create new image with padding on all sides
    IntVec2 paddedSize
	(
        m_dimensions.x + 2 * padding,
        m_dimensions.y + 2 * padding
    );
    Image* paddedImage = new Image(paddedSize, Rgba8::WHITE);

    // Copy original image to center of padded image
    for (int y = 0; y < m_dimensions.y; y++)
    {
        for (int x = 0; x < m_dimensions.x; x++)
        {
            Rgba8 color = GetTexelColor(IntVec2(x, y));
            paddedImage->SetTexelColor(IntVec2(x + padding, y + padding), color);
        }
    }

    // Pad edges by extending edge pixels
    // Pad left and right edges
    for (int y = 0; y < paddedSize.y; y++)
    {
        // Get the nearest valid y coordinate
        int sourceY = GetClamped(y - padding, 0, m_dimensions.y - 1);

        // Left edge
        Rgba8 leftColor = GetTexelColor(IntVec2(0, sourceY));
        for (int x = 0; x < padding; x++)
        {
            paddedImage->SetTexelColor(IntVec2(x, y), leftColor);
        }

        // Right edge
        Rgba8 rightColor = GetTexelColor(IntVec2(m_dimensions.x - 1, sourceY));
        for (int x = m_dimensions.x + padding; x < paddedSize.x; x++)
        {
            paddedImage->SetTexelColor(IntVec2(x, y), rightColor);
        }
    }

    // Pad top and bottom edges
    for (int x = 0; x < paddedSize.x; x++)
    {
        // Get the nearest valid x coordinate
        int sourceX = GetClamped(x - padding, 0, m_dimensions.x - 1);

        // Bottom edge
        Rgba8 bottomColor = GetTexelColor(IntVec2(sourceX, 0));
        for (int y = 0; y < padding; y++)
        {
            paddedImage->SetTexelColor(IntVec2(x, y), bottomColor);
        }

        // Top edge
        Rgba8 topColor = GetTexelColor(IntVec2(sourceX, m_dimensions.y - 1));
        for (int y = m_dimensions.y + padding; y < paddedSize.y; y++)
        {
            paddedImage->SetTexelColor(IntVec2(x, y), topColor);
        }
    }

    return paddedImage;
}
