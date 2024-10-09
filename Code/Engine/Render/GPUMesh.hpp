#pragma once
#include "Engine/Render/CPUMesh.hpp"
class IndexBuffer;
class VertexBuffer;
class Renderer;
class GPUMesh
{
	friend class Renderer;
public:
	GPUMesh(Renderer* renderer);
	GPUMesh(CPUMesh const* cpuMesh, Renderer* renderer);
	virtual ~GPUMesh();

	void Create(CPUMesh const* cpuMesh);
	void Render() const;

protected:
	IndexBuffer* m_indexBuffer = nullptr;
	VertexBuffer* m_vertexBuffer = nullptr;
	Renderer* m_renderer = nullptr;
};
