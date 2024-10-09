#pragma once
struct  ID3D11Buffer;
class IndexBuffer
{
	friend class Renderer;
public:
	IndexBuffer(unsigned int size);
	IndexBuffer(const IndexBuffer& copy) = delete;
	virtual ~IndexBuffer();
	void SetIndexesSize(int indexesSize);
	int GetIndexesSize() const;
	unsigned int GetSize();

private:
	int m_indexesSize = 0;
	ID3D11Buffer* m_buffer = nullptr;
	unsigned int m_size = 0;
};