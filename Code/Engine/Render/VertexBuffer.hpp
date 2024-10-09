#pragma once

struct ID3D11Buffer;
struct ID3D11Device;
class VertexBuffer
{
	friend class Renderer;

public:
	VertexBuffer(size_t size , unsigned int stride = 0, bool m_isLinePrimite = false);
	VertexBuffer(const VertexBuffer& copy) = delete;
	virtual ~VertexBuffer();

	void Resize(size_t size);
	bool m_isLinePrimitive = false;
	size_t m_size = 0;
private:
	ID3D11Device* m_device = nullptr;
	ID3D11Buffer* m_buffer = nullptr;
	unsigned int m_stride = 0;

};