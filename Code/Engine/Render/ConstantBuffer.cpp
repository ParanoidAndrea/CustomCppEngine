#include "Engine/Render/ConstantBuffer.hpp"
#define WIN32_LEARN_AND_MEAN

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

ConstantBuffer::ConstantBuffer(size_t size)
	:m_size(size)
{

}

ConstantBuffer::~ConstantBuffer()
{
	m_buffer->Release();
}
