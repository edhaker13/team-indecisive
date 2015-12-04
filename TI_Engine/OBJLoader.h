#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "Structures.h"

namespace Indecisive
{
	class OBJLoader
	{
	public:
		OBJLoader() {};
		~OBJLoader() {};
		Mesh* Load(const std::string filename, bool invertTexCoords = true);

	private:

		bool FindSimilarVertex(const Vertex& vertex, std::map<Vertex, WORD>& vertToIndexMap, WORD& index);
		void CreateIndices(
			const std::vector<Vector3>& inVertices, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals,
			std::vector<WORD>& outIndices, std::vector<Vector3>& outVertices, std::vector<Vector2>& outTexCoords, std::vector<Vector3>& outNormals);

	};
}