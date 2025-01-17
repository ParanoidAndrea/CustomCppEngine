#pragma once
#include "Engine/Render/Renderer.hpp"
class TextureArray
{
    friend class Renderer;

private:
    TextureArray() {}
    TextureArray(TextureArray const& copy) = delete;
    ~TextureArray();

public:
    IntVec2 const& GetDimensions() const { return m_dimensions; }
    int GetArraySize() const { return m_arraySize; }

protected:
    std::string m_name = "";
    IntVec2 m_dimensions;
    int m_arraySize = 0;

    ID3D11Texture2D* m_textureArray = nullptr;
    ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
};
