#include "Texture.hpp"
#define WIN32_LEARN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
Texture::Texture()
{

}

Texture::~Texture()
{

	DX_SAFE_RELEASE(m_texture);
	DX_SAFE_RELEASE(m_shaderResourceView);
	DX_SAFE_RELEASE(m_renderTargetView);

}

bool Texture::IsNormalTextureCompatible(Texture const* normalTexture) const
{
	// Check if normalTexture is not null
	if (normalTexture == nullptr)
	{
		return false;
	}

	// Compare dimensions
	if (m_dimensions != normalTexture->GetDimensions())
	{
		return false;
	}

	// Compare formats
	D3D11_TEXTURE2D_DESC desc;
	D3D11_TEXTURE2D_DESC normalDesc;

	m_texture->GetDesc(&desc);
	normalTexture->m_texture->GetDesc(&normalDesc);

	if (desc.Format != normalDesc.Format)
	{
		return false;
	}
	return true;
}

ID3D11Texture2D const* Texture::GetTexture() const
{
	return m_texture;
}

