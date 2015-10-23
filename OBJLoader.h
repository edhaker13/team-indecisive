#pragma once
#include "Structures.h"
#include <map>
#include <vector>
#include "MeshComponent.h"

namespace Indecisive
{
	class OBJLoader
	{
	public:
		OBJLoader();
		~OBJLoader(); 

	private:
		Geometry Load();

		bool FindSimilarVertex(const Vertex& vertex, std::map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index);

		void CreateIndices(const std::vector<Vector3>& inVertices, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::vector<unsigned short>& outIndices, std::vector<Vector3>& outVertices, std::vector<Vector2>& outTexCoords, std::vector<Vector3>& outNormals);

	};
}