#include "Engine/Render/CPUMesh.hpp"
#include "Engine/Render/ObjLoader.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include <chrono>
#include "Game/EngineBuildPreferences.hpp"
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
#if !defined ENGINE_DISABLE_MESH_DEBUGTIME
	auto calculateStartTime = std::chrono::high_resolution_clock::now();
#endif

	CalculateTangentSpaceBasisVectors(m_vertexes, m_indexes, !outHasNormal, outHasUV);
#if !defined ENGINE_DISABLE_MESH_DEBUGTIME

	auto calculateEndTime = std::chrono::high_resolution_clock::now();
	auto calculateTime = std::chrono::duration_cast<std::chrono::microseconds>(calculateEndTime - calculateStartTime).count();

	PrintTextToDebug(Stringf("Calculated tangent basis time: %.8fs\n", (double)calculateTime/1000000.0));
#endif
}

void CPUMesh::Duplicate(Mat44 const& transform)
{
	static std::vector<Vertex_PCUTBN> originalVertex = m_vertexes;
	static std::vector<unsigned int> originalIndexes = m_indexes;

	std::vector<Vertex_PCUTBN> newVerts = originalVertex;
	std::vector<unsigned int> newIndexs = originalIndexes;

	TransformVertexArray3D(newVerts, transform);

    m_vertexes.insert(m_vertexes.end(), newVerts.begin(), newVerts.end());
	m_indexes.insert(m_indexes.end(), newIndexs.begin(), newIndexs.end());
	
	//m_vertexes.emplace_back(newVerts);
	//m_indexes.emplace_back(newIndexs);
}
