#include "Engine/Render/CPUMesh.hpp"
#include "Engine/Render/ObjLoader.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include <chrono>
CPUMesh::CPUMesh()
{

}

CPUMesh::CPUMesh(std::string const& objFilename, Mat44 const& transform)
{
	Load(objFilename, transform);
}

CPUMesh::~CPUMesh()
{
	m_indexes.clear();
	m_vertexes.clear();
}

void CPUMesh::Load(std::string const& objFilename, Mat44 const& transform)
{
	bool outHasNormal = false;
	bool outHasUV = false;

	ObjLoader::Load(objFilename, m_vertexes, m_indexes, outHasNormal, outHasUV, transform);

	auto calculateStartTime = std::chrono::high_resolution_clock::now();
	CalculateTangentSpaceBasisVectors(m_vertexes, m_indexes, !outHasNormal, outHasUV);
	auto calculateEndTime = std::chrono::high_resolution_clock::now();
	auto calculateTime = std::chrono::duration_cast<std::chrono::microseconds>(calculateEndTime - calculateStartTime).count();
	PrintTextToDebug(Stringf("Calculated tangent basis time: %.8fs\n", (double)calculateTime/1000000.0));
}
