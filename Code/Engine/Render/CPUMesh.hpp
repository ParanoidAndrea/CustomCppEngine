#pragma once
#include <vector>
#include <string>
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Mat44.hpp"
class CPUMesh
{
public:
	CPUMesh();
	CPUMesh(std::string const& objFilename, Mat44 const& transform);
	virtual ~CPUMesh();

	void Load(std::string const& objFilename, Mat44 const& transform);

	std::vector<unsigned int> m_indexes;
	std::vector<Vertex_PCUTBN> m_vertexes;
};