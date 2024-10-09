#include "Engine/Render/ObjLoader.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <unordered_map>

struct VertexIndex 
{
	int m_posIndex = -1;  // Vertex Position index
	int m_texCoordIndex = -1;  // Texture coordinate index
	int m_normalIndex = -1; // vertex Normal index
};
struct VertexHash
{
	std::size_t operator() (const VertexIndex& k) const 
	{
		return std::hash<int>()(k.m_posIndex) ^
			(std::hash<int>()(k.m_texCoordIndex) << 1) ^
			(std::hash<int>()(k.m_normalIndex) << 2);
	}
};

struct VertexEqual 
{
	bool operator() (VertexIndex const& lhs, VertexIndex const& rhs) const
	{
		return lhs.m_posIndex == rhs.m_posIndex
			&& lhs.m_normalIndex == rhs.m_normalIndex
			&& lhs.m_texCoordIndex == rhs.m_texCoordIndex;
	}

};
struct Triangle
{
	Triangle(VertexIndex const& v1, VertexIndex const& v2, VertexIndex const& v3, Rgba8 const& color = Rgba8::WHITE)
		:m_color(color)
	{
		m_vertexes[0] = v1;
		m_vertexes[1] = v2;
		m_vertexes[2] = v3;
	}
	VertexIndex m_vertexes[3];
	Rgba8 m_color = Rgba8::WHITE;
};
class Face
{
public:
	Face(Strings faceTokens, Rgba8 faceColor, std::vector<Triangle> &triangles);
	Face() {}
	//std::vector<VertexIndex> m_vertexIndexes;
	Rgba8 m_faceColor = Rgba8::WHITE;
private:
	VertexIndex ParseVertexToken(std::string const& token);
};





Face::Face(Strings faceTokens, Rgba8 faceColor, std::vector<Triangle> &triangles) :m_faceColor(faceColor)
{
	for (size_t i = 2; i < faceTokens.size() - 1; ++i) {
		//m_vertexIndexes.push_back(ParseVertexToken(faceTokens[1]));
		//m_vertexIndexes.push_back(ParseVertexToken(faceTokens[i]));
		//m_vertexIndexes.push_back(ParseVertexToken(faceTokens[i + 1]));
		Triangle triangle(ParseVertexToken(faceTokens[1]), ParseVertexToken(faceTokens[i]), ParseVertexToken(faceTokens[i + 1]),m_faceColor);
		triangles.push_back(triangle);
         
	}
	
}

VertexIndex Face::ParseVertexToken(std::string const& token)
{
	Strings indices = SplitStringOnDelimiter(token, '/', false);
	VertexIndex vertex;
	if (indices.size() > 0 && !indices[0].empty()) 
	{
		vertex.m_posIndex = atoi(indices[0].c_str())-1;
	}
	if (indices.size() > 1 && !indices[1].empty()) 
	{
		vertex.m_texCoordIndex = atoi(indices[1].c_str())-1;
	}
	if (indices.size() > 2 && !indices[2].empty()) 
	{
		vertex.m_normalIndex = atoi(indices[2].c_str()) -1;
	}
	return vertex;
}

bool ObjLoader::Load(std::string const& fileName, std::vector<Vertex_PCUTBN>& outVertexes, std::vector<unsigned int>& outIndexes, bool& outHasNormals, bool& outHasUVs, Mat44 const& transform)
{
	auto loadStartTime = std::chrono::high_resolution_clock::now();
	std::vector<Vec3> vertexPositions;
	std::vector<Vec2> vertexUVs;
	std::vector<Vec3> vertexNormals;
	int faceSize = 0;
	std::map<std::string, Rgba8> materialLibrary;
	std::string currentMaterial;
	std::string objFileString;
	std::vector<Triangle> triangles;
	FileReadToString(objFileString, fileName);
	Strings objStringLines = SplitStringOnDelimiter(objFileString, "\r\n", false);
	if (objStringLines.size() == 1)
	{
		objStringLines = SplitStringOnDelimiter(objFileString, "\n", false);
	}
	for (int i = 0; i < (int)objStringLines.size(); ++i)
	{
		Strings tokens = SplitStringOnDelimiter(objStringLines[i], ' ', true);
		if (tokens.empty()) continue;
		if (tokens[0].compare("mtllib") == 0)
		{
			std::string matLibFilename = tokens[1];
			LoadMaterialLibrary(matLibFilename, materialLibrary);
		}
		else if (tokens[0].compare("usemtl") == 0)
		{
			currentMaterial = tokens[1];
		}
		else if (tokens[0].compare("v") == 0)
		{
			vertexPositions.push_back(Vec3((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str())));
		}
		else if (tokens[0].compare("vt") == 0)
		{
			vertexUVs.push_back(Vec2((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str())));
		}
		else if (tokens[0].compare("vn") == 0)
		{
			vertexNormals.push_back(Vec3((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str())));
		}
		else if (tokens[0].compare("f") == 0)
		{
			Face face(tokens, materialLibrary[currentMaterial], triangles);
			++faceSize;
		}

	}
	auto loadEndTime = std::chrono::high_resolution_clock::now();
	auto loadTime = std::chrono::duration_cast<std::chrono::microseconds>(loadEndTime - loadStartTime).count();

	outVertexes.reserve(triangles.size() * 3);
	outIndexes.reserve(triangles.size() * 3);

	if (vertexUVs.size() != 0)
	{
		outHasUVs = true;
	}


	if (vertexNormals.size() != 0)
	{
		outHasNormals = true;
	}

	//If the OBJ file has no faces/triangles, Create faces/triangles
	if (faceSize == 0)
	{
		for (int i = 2; i < vertexPositions.size(); ++i)
		{
			VertexIndex vertexIndex[3];
			
			vertexIndex[0].m_posIndex = i - 2;
			//face.m_vertexIndexes.push_back(vertexIndex);
			vertexIndex[1].m_posIndex = i - 1;
			//face.m_vertexIndexes.push_back(vertexIndex);
			vertexIndex[2].m_posIndex = i;
			//face.m_vertexIndexes.push_back(vertexIndex);
			triangles.push_back(Triangle(vertexIndex[0],vertexIndex[1],vertexIndex[2]));
		}
	}

	//for (auto const& face : faces)
	//{
	//	VertexIndex vertexes[3];
	//	vertexes[0] = face.m_vertexIndexes[0];
	//	vertexes[1] = face.m_vertexIndexes[1];
	//	vertexes[2] = face.m_vertexIndexes[2];
	//	for (size_t i = 2; i < face.m_vertexIndexes.size(); i++)
	//	{
	//		vertexes[1] = face.m_vertexIndexes[i - 1];
	//		vertexes[2] = face.m_vertexIndexes[i];
	//		Triangle triangle(vertexes[0], vertexes[1], vertexes[2], face.m_faceColor);
	//		triangles.push_back(triangle);
	//	}
	//}

	std::unordered_map<VertexIndex, int, VertexHash, VertexEqual> vertexMap;
	for (size_t i = 0; i < triangles.size(); i++)
	{
		Triangle& triangle = triangles[i];

		VertexIndex &vertexIndex0 = triangle.m_vertexes[0];
		VertexIndex &vertexIndex1 = triangle.m_vertexes[1];
		VertexIndex &vertexIndex2 = triangle.m_vertexes[2];

		int index0 = (unsigned int)outVertexes.size();
		auto it0 = vertexMap.find(vertexIndex0);
		if (it0 == vertexMap.end())
		{
			Vec3 const& p0 = vertexPositions[triangle.m_vertexes[0].m_posIndex];
			Vec2 const& t0 = triangle.m_vertexes[0].m_texCoordIndex	== -1 ? Vec2::ZERO : vertexUVs[triangle.m_vertexes[0].m_texCoordIndex];
			Vec3 const& n0 = triangle.m_vertexes[0].m_normalIndex == -1 ? Vec3::ZERO : vertexNormals[triangle.m_vertexes[0].m_normalIndex];
			Vertex_PCUTBN v0(p0, triangle.m_color, t0, Vec3::ZERO, Vec3::ZERO, n0);
			outVertexes.push_back(v0);

			vertexMap[vertexIndex0] = index0;
		}
		else 
		{
			index0 = it0->second;
		}

		int index1 = (unsigned int)outVertexes.size();
		auto it1 = vertexMap.find(vertexIndex1);
		if (it1 == vertexMap.end()) 
		{
			Vec3 const& p1 = vertexPositions[triangle.m_vertexes[1].m_posIndex];
			Vec2 const& t1 = triangle.m_vertexes[1].m_texCoordIndex	== -1 ? Vec2::ZERO : vertexUVs[triangle.m_vertexes[1].m_texCoordIndex];
			Vec3 const& n1 = triangle.m_vertexes[1].m_normalIndex == -1 ? Vec3::ZERO : vertexNormals[triangle.m_vertexes[1].m_normalIndex];
			Vertex_PCUTBN v1(p1, triangle.m_color, t1, Vec3::ZERO, Vec3::ZERO, n1);
			outVertexes.push_back(v1);
			vertexMap[vertexIndex1] = index1;
		}
		else
		{
			index1 = it1->second; 
		}

		int index2 = (unsigned int)outVertexes.size();
		auto it2 = vertexMap.find(vertexIndex2); 
		if (it2 == vertexMap.end()) 
		{
			Vec3 const& p2 = vertexPositions[triangle.m_vertexes[2].m_posIndex];
			Vec2 const& t2 = triangle.m_vertexes[2].m_texCoordIndex == -1 ? Vec2::ZERO : vertexUVs[triangle.m_vertexes[2].m_texCoordIndex];
			Vec3 const& n2 = triangle.m_vertexes[2].m_normalIndex == -1 ? Vec3::ZERO : vertexNormals[triangle.m_vertexes[2].m_normalIndex];
			Vertex_PCUTBN v2(p2, triangle.m_color, t2, Vec3::ZERO, Vec3::ZERO, n2);
			outVertexes.push_back(v2);
			vertexMap[vertexIndex2] = index2;
		}
		else 
		{
			index2 = it2->second; 
		}
		outIndexes.push_back(index0);
		outIndexes.push_back(index1);
		outIndexes.push_back(index2);
	}
	//for (auto const& face : faces)
	//{
	//	for (auto const& indexes : face.m_vertexIndexes)
	//	{
	//		Rgba8 currentColor = Rgba8::WHITE;
	//		Vertex_PCUTBN outVertex;
	//		outVertex.m_position = vertexPositions[indexes.m_posIndex];
	//		outVertex.m_color = face.m_faceColor;
	//		if (indexes.m_texCoordIndex != -1) //Has valid uv or not
	//		{
	//			outVertex.m_uvTexCoords = vertexUVs[indexes.m_texCoordIndex];
	//		}
	//		else
	//		{
	//			outVertex.m_uvTexCoords = Vec2(0.f, 0.f);
	//		}
	//
	//		if (indexes.m_normalIndex != -1) //Has valid normal or not
	//		{
	//			outVertex.m_normal = vertexNormals[indexes.m_normalIndex];
	//		}
	//		else
	//		{
	//			outVertex.m_normal = Vec3(0.f, 0.f, 1.f);
	//		}
	//		outIndexes.push_back(static_cast<unsigned int>(outIndexes.size()));
	//		outVertexes.push_back(outVertex);
	//	}
	//}
	

	TransformVertexArray3D(outVertexes, transform);

	auto createCPUMeshEndTime = std::chrono::high_resolution_clock::now();
	auto createCPUMeshTime = std::chrono::duration_cast<std::chrono::microseconds>(createCPUMeshEndTime - loadEndTime).count();
	
	PrintTextToDebug(Stringf("---------------------------------------------------------------------------------\n"));
	PrintTextToDebug(Stringf("Loaded .obj file %s\n", fileName.c_str()));
	PrintTextToDebug(Stringf("\t\t\t\t\t\t "));
	PrintTextToDebug(Stringf("positions: %d  ", (int)vertexPositions.size()) );
	PrintTextToDebug(Stringf("uvs: %d  ", (int)vertexUVs.size()));
	PrintTextToDebug(Stringf("normals: %d  ", (int)vertexNormals.size()));
	PrintTextToDebug(Stringf("faces: %d\n", faceSize));
	PrintTextToDebug(Stringf("\t\t\t\t\t\t "));
	PrintTextToDebug(Stringf("vertexes: %d  ", (int)outVertexes.size()));
	PrintTextToDebug(Stringf("indexes: %d  ", (int)outIndexes.size()));
	PrintTextToDebug(Stringf("triangles: %d  ", (int)triangles.size()));
	PrintTextToDebug(Stringf("time:%.8fs\n", (double)loadTime / 1000000.0));
	PrintTextToDebug(Stringf("Created CPU Mesh         time: %.8fs\n", (double)createCPUMeshTime / 1000000.0));

	return true;
}


void LoadMaterialLibrary(std::string const& materialLibFilename, std::map<std::string, Rgba8>& materialLibrary)
{
	std::string materialLibString;
	FileReadToString(materialLibString,"Data/Models/"+materialLibFilename);
	Strings materialLibStringLines = SplitStringOnDelimiter(materialLibString, "\n", false);
	std::string currentMaterialName;
	for (size_t i = 0; i <materialLibStringLines.size(); ++i)
	{
		if (materialLibStringLines[i].substr(0, 6).compare("newmtl") == 0)
		{
			Strings tokens = SplitStringOnDelimiter(materialLibStringLines[i], ' ', true);
			currentMaterialName = tokens[1];
		}
		else if (materialLibStringLines[i].substr(0, 2).compare("Kd") == 0)
		{
			Strings tokens = SplitStringOnDelimiter(materialLibStringLines[i], ' ', true);
			{
				Rgba8 color = Rgba8::SetFromFloats((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str()),1.f);
				materialLibrary[currentMaterialName] = color;
			}
		}
	}

}
