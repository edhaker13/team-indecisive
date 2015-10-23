#include "OBJLoader.h"
#include <string>

namespace Indecisive
{
	
	bool OBJLoader::FindSimilarVertex(const Vertex& vertex, std::map<Vertex, unsigned short>& vertToIndexMap, unsigned short& index)
	{
		auto it = vertToIndexMap.find(vertex);
		if (it == vertToIndexMap.end())
		{
			return false;
		}
		else
		{
			index = it->second;
			return true;
		}
	}

	void OBJLoader::CreateIndices(const std::vector<Vector3>& inVertices, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::vector<unsigned short>& outIndices, std::vector<Vector3>& outVertices, std::vector<Vector2>& outTexCoords, std::vector<Vector3>& outNormals)
	{
		//Mapping from an already-existing SimpleVertex to its corresponding index
		std::map<Vertex, unsigned short> vertToIndexMap;

		std::pair<Vertex, unsigned short> pair;

		int numVertices = inVertices.size();
		for (int i = 0; i < numVertices; ++i) //For each vertex
		{
			Vertex vertex = { inVertices[i], inNormals[i], inTexCoords[i] };

			unsigned short index;
			bool found = FindSimilarVertex(vertex, vertToIndexMap, index); //See if a vertex already exists in the buffer that has the same attributes as this one
			if (found) //if found, re-use it's index for the index buffer
			{
				outIndices.push_back(index);
			}
			else //if not found, add it to the buffer
			{
				outVertices.push_back(vertex.Pos);
				outTexCoords.push_back(vertex.TexC);
				outNormals.push_back(vertex.Normal);

				unsigned short newIndex = (unsigned short)outVertices.size() - 1;
				outIndices.push_back(newIndex);

				//Add it to the map
				pair.first = vertex;
				pair.second = newIndex;
				vertToIndexMap.insert(pair);
			}
		}
	}

}