#include "VertexBuffer.hpp"
#include "Engine/Render/Renderer.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
VertexBuffer::VertexBuffer(size_t size, unsigned int stride, bool isLinePrimitive)
	:m_size(size), m_stride(stride), m_isLinePrimitive(isLinePrimitive)
{

}

VertexBuffer::~VertexBuffer()
{
	m_buffer->Release();
}

void VertexBuffer::Resize(size_t size)
{
	m_size = size;
}
