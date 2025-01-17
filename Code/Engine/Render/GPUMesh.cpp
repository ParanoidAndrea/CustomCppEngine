#include "Engine/Render/GPUMesh.hpp"
#include "Engine/Render/IndexBuffer.hpp"
#include "Engine/Render/VertexBuffer.hpp"
#include "Engine/Render/Renderer.hpp"
GPUMesh::GPUMesh(Renderer* renderer)
	:m_renderer(renderer)
{
}

GPUMesh::GPUMesh(CPUMesh const* cpuMesh, Renderer* renderer)
	:m_renderer(renderer)
{
	Create(cpuMesh);
}

GPUMesh::~GPUMesh()
{
	delete m_vertexBuffer;
	m_vertexBuffer = nullptr;
	delete m_indexBuffer;
	m_indexBuffer = nullptr;

}

void GPUMesh::Create(CPUMesh const* cpuMesh)
{

	size_t indexBuffersize = sizeof(unsigned int) * (int)cpuMesh->m_indexes.size();
	size_t vertexBuffersize = sizeof(Vertex_PCUTBN) * (int)cpuMesh->m_vertexes.size();
	m_indexBuffer = m_renderer->CreateIndexBuffer(indexBuffersize);
	m_vertexBuffer = m_renderer->CreateVertexBuffer(vertexBuffersize);

	m_renderer->CopyCPUToGPU(cpuMesh->m_vertexes.data(), vertexBuffersize, m_vertexBuffer);
	m_renderer->CopyCPUToGPU(cpuMesh->m_indexes.data(), indexBuffersize, m_indexBuffer);

	m_indexBuffer->SetIndexesSize((int)cpuMesh->m_indexes.size());


}

void GPUMesh::Render() const
{
	m_renderer->DrawVertexTBN(m_indexBuffer->GetIndexesSize(), m_vertexBuffer, m_indexBuffer);

}
