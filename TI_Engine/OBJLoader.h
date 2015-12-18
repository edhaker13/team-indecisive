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
		LIBRARY_API OBJLoader() {};
		~OBJLoader() {};
		LIBRARY_API void Load(const std::string& filename, bool invertTexCoords = true);
		LIBRARY_API MeshComponent* ConstructFromMesh(const std::string& meshName);

	private:
		std::map<std::string, std::vector<SubObject*>> _meshSubObjs;

		void CreateIndices(
			const std::vector<Vector3>& inVertices, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals,
			std::vector<WORD>& outIndices, std::vector<Vector3>& outVertices, std::vector<Vector2>& outTexCoords, std::vector<Vector3>& outNormals);
		bool FindSimilarVertex(const Vertex& vertex, std::map<Vertex, WORD>& vertToIndexMap, WORD& index);
		void LoadMaterialLibrary(const std::string& fileName);
	};
}