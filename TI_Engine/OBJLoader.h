#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "MeshComponent.h"
#include "Structures.h"

namespace Indecisive
{
	class OBJLoader
	{
	public:
		OBJLoader() {};
		~OBJLoader() {};
		Mesh* Load(const std::string& filename, bool invertTexCoords = true);

	private:
		std::map<std::string, std::vector<SubObject>> _meshSubObjs;

		void CreateIndices(
			const std::vector<Vector3>& inVertices, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals,
			std::vector<WORD>& outIndices, std::vector<Vector3>& outVertices, std::vector<Vector2>& outTexCoords, std::vector<Vector3>& outNormals);
		MeshComponent* ConstructFromMesh(const std::string& meshName);
		bool FindSimilarVertex(const Vertex& vertex, std::map<Vertex, WORD>& vertToIndexMap, WORD& index);
		void LoadMaterialLibrary(const std::string& fileName);

	};
}