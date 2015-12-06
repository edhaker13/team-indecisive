#include "OBJLoader.h"
#include "MeshComponent.h"
#include "ServiceLocator.h"

namespace Indecisive
{
	
	bool OBJLoader::FindSimilarVertex(const Vertex& vertex, std::map<Vertex, WORD>& vertToIndexMap, WORD& index)
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

	Mesh* OBJLoader::Load(const std::string filename, bool invertTexCoords)
	{
		std::string binaryFilename = ".\\Assets\\" + filename + ".bin";
		std::ifstream binaryInFile;
		binaryInFile.open(binaryFilename, std::ios::in | std::ios::binary);

		if (!binaryInFile.good())
		{
			std::ifstream inFile;
			inFile.open(".\\Assets\\" + filename);

			if (!inFile.good())
			{
				return nullptr;
			}
			else
			{
				//Vectors to store the vertex positions, normals and texture coordinates. Need to use vectors since they're resizeable and we have
				//no way of knowing ahead of time how large these meshes will be
				std::vector<Vector3> verts;
				std::vector<Vector3> normals;
				std::vector<Vector2> texCoords;

				//DirectX uses 1 index buffer, OBJ is optimized for storage and not rendering and so uses 3 smaller index buffers.....great...
				//We'll have to merge this into 1 index buffer which we'll do after loading in all of the required data.
				std::vector<unsigned short> vertIndices;
				std::vector<unsigned short> normalIndices;
				std::vector<unsigned short> textureIndices;

				std::string input;

				Vector3 vert;
				Vector2 texCoord;
				Vector3 normal;
				unsigned short vInd[3]; //indices for the vertex position
				unsigned short tInd[3]; //indices for the texture coordinate
				unsigned short nInd[3]; //indices for the normal
				std::string beforeFirstSlash;
				std::string afterFirstSlash;
				std::string afterSecondSlash;

				while (!inFile.eof()) //While we have yet to reach the end of the file...
				{
					inFile >> input; //Get the next input from the file

					//Check what type of input it was, we are only interested in vertex positions, texture coordinates, normals and indices, nothing else
					if (input.compare("v") == 0) //Vertex position
					{
						inFile >> vert.x;
						inFile >> vert.y;
						inFile >> vert.z;

						verts.push_back(vert);
					}
					else if (input.compare("vt") == 0) //Texture coordinate
					{
						inFile >> texCoord.x;
						inFile >> texCoord.y;

						if (invertTexCoords) texCoord.y = 1.0f - texCoord.y;

						texCoords.push_back(texCoord);
					}
					else if (input.compare("vn") == 0) //Normal
					{
						inFile >> normal.x;
						inFile >> normal.y;
						inFile >> normal.z;

						normals.push_back(normal);
					}
					else if (input.compare("f") == 0) //Face
					{
						for (int i = 0; i < 3; ++i)
						{
							inFile >> input;
							int slash = input.find("/"); //Find first forward slash
							int secondSlash = input.find("/", slash + 1); //Find second forward slash

							//Extract from string
							beforeFirstSlash = input.substr(0, slash); //The vertex position index
							afterFirstSlash = input.substr(slash + 1, secondSlash - slash - 1); //The texture coordinate index
							afterSecondSlash = input.substr(secondSlash + 1); //The normal index

							//Parse into int
							vInd[i] = (unsigned short)atoi(beforeFirstSlash.c_str()); //atoi = "ASCII to int"
							tInd[i] = (unsigned short)atoi(afterFirstSlash.c_str());
							nInd[i] = (unsigned short)atoi(afterSecondSlash.c_str());
						}

						//Place into vectors
						for (int i = 0; i < 3; ++i)
						{
							vertIndices.push_back(vInd[i] - 1);		//Minus 1 from each as these as OBJ indexes start from 1 whereas C++ arrays start from 0
							textureIndices.push_back(tInd[i] - 1);	//which is really annoying. Apart from Lua and SQL, there's not much else that has indexing 
							normalIndices.push_back(nInd[i] - 1);	//starting at 1. So many more languages index from 0, the .OBJ people screwed up there.
						}
					}
				}
				inFile.close(); //Finished with input file now, all the data we need has now been loaded in

				//Get vectors to be of same size, ready for singular indexing
				std::vector<Vector3> expandedVertices;
				std::vector<Vector3> expandedNormals;
				std::vector<Vector2> expandedTexCoords;
				unsigned int numIndices = vertIndices.size();
				for (unsigned int i = 0; i < numIndices; i++)
				{
					expandedVertices.push_back(verts[vertIndices[i]]);
					expandedTexCoords.push_back(texCoords[textureIndices[i]]);
					expandedNormals.push_back(normals[normalIndices[i]]);
				}

				//Now to (finally) form the final vertex, texture coord, normal list and single index buffer using the above expanded vectors
				std::vector<unsigned short> meshIndices;
				meshIndices.reserve(numIndices);
				std::vector<Vector3> meshVertices;
				meshVertices.reserve(expandedVertices.size());
				std::vector<Vector3> meshNormals;
				meshNormals.reserve(expandedNormals.size());
				std::vector<Vector2> meshTexCoords;
				meshTexCoords.reserve(expandedTexCoords.size());

				CreateIndices(expandedVertices, expandedTexCoords, expandedNormals, meshIndices, meshVertices, meshTexCoords, meshNormals);


				IGraphics* pGraphics = static_cast<IGraphics*> (ServiceLocatorInstance()->Get("graphics"));

				//Turn data from vector form to arrays
				Vertex* finalVerts = new Vertex[meshVertices.size()];
				unsigned int numMeshVertices = meshVertices.size();
				for (unsigned int i = 0; i < numMeshVertices; ++i)
				{
					finalVerts[i].Pos = meshVertices[i];
					finalVerts[i].Normal = meshNormals[i];
					finalVerts[i].TexC = meshTexCoords[i];
				}

				auto pMesh = new Mesh();
				pMesh->vertexBuffer = pGraphics->InitVertexBuffer(finalVerts, numMeshVertices);
				pMesh->vertexBufferOffset = 0;
				pMesh->vertexBufferStride = sizeof(Vertex);

				unsigned short* indicesArray = new unsigned short[meshIndices.size()];
				unsigned int numMeshIndices = meshIndices.size();
				for (unsigned int i = 0; i < numMeshIndices; ++i)
				{
					indicesArray[i] = meshIndices[i];
				}

				//Output data into binary file, the next time you run this function, the binary file will exist and will load that instead which is much quicker than parsing into vectors
				std::ofstream outbin(binaryFilename.c_str(), std::ios::out | std::ios::binary);
				outbin.write((char*)&numMeshVertices, sizeof(unsigned int));
				outbin.write((char*)&numMeshIndices, sizeof(unsigned int));
				outbin.write((char*)finalVerts, sizeof(Vertex) * numMeshVertices);
				outbin.write((char*)indicesArray, sizeof(unsigned short) * numMeshIndices);
				outbin.close();

				pMesh->indexBuffer = pGraphics->InitIndexBuffer(indicesArray, numMeshIndices);
				pMesh->indexBufferOffset = 0;
				pMesh->indexBufferSize = numMeshIndices;

				//This data has now been sent over to the GPU so we can delete this CPU-side stuff
				delete[] indicesArray;
				delete[] finalVerts;

				return pMesh;
			}

			//-----------------------------------------------------------------------------
			// *
			//-----------------------------------------------------------------------------
		}
		else
		{
			auto pMesh = new Mesh();
			unsigned int numVertices;
			unsigned int numIndices;

			//Read in array sizes
			binaryInFile.read((char*)&numVertices, sizeof(unsigned int));
			binaryInFile.read((char*)&numIndices, sizeof(unsigned int));

			//Read in data from binary file
			Vertex* finalVerts = new Vertex[numVertices];
			unsigned short* indices = new unsigned short[numIndices];
			binaryInFile.read((char*)finalVerts, sizeof(Vertex) * numVertices);
			binaryInFile.read((char*)indices, sizeof(unsigned short) * numIndices);

			//Put data into vertex and index buffers, then pass the relevant data to the MeshData object.
			//The rest of the code will hopefully look familiar to you, as it's similar to whats in your InitVertexBuffer and InitIndexBuffer methods

			IGraphics* pGraphics = static_cast<IGraphics*> (ServiceLocatorInstance()->Get("graphics"));

			pMesh->vertexBuffer = pGraphics->InitVertexBuffer(finalVerts, numVertices);
			pMesh->vertexBufferOffset = 0;
			pMesh->vertexBufferStride = sizeof(Vertex);
			pMesh->indexBuffer = pGraphics->InitIndexBuffer(indices, numIndices);
			pMesh->indexBufferOffset = 0;
			pMesh->indexBufferSize = numIndices;

			//This data has now been sent over to the GPU so we can delete this CPU-side stuff
			delete[] indices;
			delete[] finalVerts;

			return pMesh;
		}
	}


}