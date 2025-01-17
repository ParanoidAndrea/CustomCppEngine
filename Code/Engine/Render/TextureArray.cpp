#include "Engine/Render/TextureArray.hpp"
#define WIN32_LEARN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
TextureArray::~TextureArray()
{
    DX_SAFE_RELEASE(m_textureArray);
    DX_SAFE_RELEASE(m_shaderResourceView);
}

