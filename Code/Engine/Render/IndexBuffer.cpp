#include "IndexBuffer.hpp"
#include "Engine/Render/Renderer.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
IndexBuffer::IndexBuffer(unsigned int size)
	:m_size(size)
{

}

IndexBuffer::~IndexBuffer()
{
	m_buffer->Release();
	
}

void IndexBuffer::SetIndexesSize(int indexesSize)
{
	m_indexesSize = indexesSize;
}

int IndexBuffer::GetIndexesSize() const
{
	return m_indexesSize;
}

unsigned int IndexBuffer::GetSize()
{
	return m_size;
}
