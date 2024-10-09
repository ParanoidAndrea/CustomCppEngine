#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Mat44.hpp"
#include <vector>
#include "Engine/Core/StringUtils.hpp"
#include <map>

class ObjLoader
{
public:
	static bool Load(std::string const& fileName, std::vector<Vertex_PCUTBN>& outVertexes, std::vector<unsigned int>& outIndexes, bool& outHasNormals, bool& outHasUVs, Mat44 const& transform = Mat44());
};
void LoadMaterialLibrary(std::string const& materialLibFilename, std::map<std::string, Rgba8>& materialLibrary);
