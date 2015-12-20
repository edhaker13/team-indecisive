#include "OBJLoader.h"
#include "IResourceManager.h"

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

	MeshComponent* OBJLoader::ConstructFromMesh(const std::string& meshName) const
	{
		assert(!meshName.empty());
		auto mesh = ResourceManagerInstance()->GetMesh(meshName);
		if (mesh == nullptr)
		{	
			// TODO: Error Handling
			return nullptr;
		}
		auto m = new MeshComponent(*mesh);
		auto it = _meshSubObjs.find(meshName);
		assert(it != _meshSubObjs.end());
		auto subObjs = it->second;
		std::vector<int> _transparentGroups;  // Indices of groups, PRIORITY 1
		std::vector<int> _opaqueGroups; // Indices of groups, PRIORITY 2
		for (unsigned i = 0; i < subObjs.size(); i++)
		{
			Material* mat = ResourceManagerInstance()->GetMaterial(subObjs[i]->material);
			Texture* none = ResourceManagerInstance()->GetTexture("WhiteTex.dds");
			Texture* ambi = nullptr;
			Texture* diff = nullptr;
			Texture* spec = nullptr;
			bool transparent = false;
			if (mat != nullptr)
			{
				transparent = mat->transparency < 1.0f ? true : false;
				ambi = ResourceManagerInstance()->GetTexture(mat->ambientTextureName);
				diff = ResourceManagerInstance()->GetTexture(mat->diffuseTextureName);
				spec = ResourceManagerInstance()->GetTexture(mat->specularTextureName);
				if (ambi == nullptr) ambi = none;
				if (diff == nullptr) diff = none;
				if (spec == nullptr) spec = none;
			}
			subObjs[i]->ambientTexture = ambi;
			subObjs[i]->diffuseTexture = diff;
			subObjs[i]->specularTexture = spec;
			subObjs[i]->indexSize = subObjs[i]->indexEnd - subObjs[i]->indexStart;
			m->AddGroup(subObjs[i]);
			if (transparent)
			{
				_transparentGroups.push_back(i);
			}
			else
			{
				_opaqueGroups.push_back(i);
			}
		}
		for (unsigned i = 0; i < _opaqueGroups.size(); i++)
		{
			m->AddPriorityGroup(_opaqueGroups[i]);
		}
		for (unsigned i = 0; i < _transparentGroups.size(); i++)
		{
			m->AddPriorityGroup(_transparentGroups[i]);
		}
		return m;
	}

	void OBJLoader::Load(const std::string& filename, bool invertTexCoords)
	{
		assert(!filename.empty());
		std::string binaryFilename = ".\\/Assets\\/" + filename + ".bin";
		std::ifstream binaryInFile;
		binaryInFile.open(binaryFilename, std::ios::in | std::ios::binary);

		auto pGraphics = static_cast<IGraphics*>(ResourceManagerInstance()->GetService("graphics"));
		assert(pGraphics != nullptr);

		// TODO: Disabled binary load until it can r/w subobjects and material
		if (true /*!binaryInFile.good()*/)
		{
			std::ifstream inFile;
			inFile.open(".\\/Assets\\/" + filename);

			if (!inFile.good())
			{
				// TODO: Error Handling
				return;
			}
			else
			{
				//Start new subobject list for this file
				_meshSubObjs[filename];
				//Temp subobject
				SubObject* pSubObject = nullptr;
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
				UINT indexCounter = 0;

				Vector3 vert;
				Vector2 texCoord;
				Vector3 normal;
				unsigned short vInd[3]; //indices for the vertex position
				unsigned short tInd[3]; //indices for the texture coordinate
				unsigned short nInd[3]; //indices for the normal
				std::string beforeFirstSlash;
				std::string afterFirstSlash;
				std::string afterSecondSlash;
				std::string matLib;

				while (!inFile.eof()) //While we have yet to reach the end of the file...
				{
					inFile >> input; //Get the next input from the file

					//Check what type of input it was, we are only interested in vertex positions, texture coordinates, normals, indices, groups, and materials.
					if (input.compare("mtllib") == 0)//Material library, calls readMaterial()
					{
						inFile >> matLib;
						LoadMaterialLibrary(matLib);
					}
					else if (input.compare("g") == 0)//Group(SubObject) name
					{
						if (pSubObject != nullptr) _meshSubObjs[filename].push_back(pSubObject);
						pSubObject = new SubObject();
						inFile >> pSubObject->name;
						pSubObject->vertexEnd = 0;
						pSubObject->vertexStart = UINT_MAX;
						pSubObject->indexEnd = 0;
						pSubObject->indexStart = UINT_MAX;
					}
					else if (input.compare("v") == 0) //Vertex position
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
						//Sets start to an iterator, iterator position is 1 past the final element of vertindices (could use normals or textures, no matter).
						if (vertIndices.size() != 0)
						{
							pSubObject->vertexStart = vertIndices.back() < pSubObject->vertexStart ? vertIndices.back() : pSubObject->vertexStart;
						}
						else
						{
							pSubObject->vertexStart = 0;
						}
						pSubObject->indexStart = indexCounter < pSubObject->indexStart ? indexCounter : pSubObject->indexStart;
						//Place into vectors
						for (int i = 0; i < 3; ++i)
						{
							vertIndices.push_back(vInd[i] - 1);		//Minus 1 from each as these as OBJ indexes start from 1 whereas C++ arrays start from 0
							textureIndices.push_back(tInd[i] - 1);	//which is really annoying. Apart from Lua and SQL, there's not much else that has indexing 
							normalIndices.push_back(nInd[i] - 1);	//starting at 1. So many more languages index from 0, the .OBJ people screwed up there.
							indexCounter++;  //To track indexes in groups.
						}
						//sets end to an iterator, iterator position is 1 past the final element of vertices (could use normals or textures, no matter).
						pSubObject->indexEnd = indexCounter > pSubObject->indexEnd ? indexCounter : pSubObject->indexEnd;
						pSubObject->vertexEnd = vertIndices.back() > pSubObject->vertexEnd ? vertIndices.back() : pSubObject->vertexEnd;
					}
					else if (input.compare("usemtl") == 0)//Use material, changes current set material for faces
					{
						inFile >> pSubObject->material;
					}
				}
				if (pSubObject != nullptr) _meshSubObjs[filename].push_back(pSubObject);
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
				//TODO: write subobjects and material to binary
				outbin.close();

				pMesh->indexBuffer = pGraphics->InitIndexBuffer(indicesArray, numMeshIndices);
				pMesh->indexBufferOffset = 0;
				pMesh->indexBufferSize = numMeshIndices;

				//This data has now been sent over to the GPU so we can delete this CPU-side stuff
				delete[] indicesArray;
				delete[] finalVerts;

				ResourceManagerInstance()->AddMesh(filename, pMesh);
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

			//Put data into vertex and index buffers, then pass the relevant data to the Mesh object.
			pMesh->vertexBuffer = pGraphics->InitVertexBuffer(finalVerts, numVertices);
			pMesh->vertexBufferOffset = 0;
			pMesh->vertexBufferStride = sizeof(Vertex);
			pMesh->indexBuffer = pGraphics->InitIndexBuffer(indices, numIndices);
			pMesh->indexBufferOffset = 0;
			pMesh->indexBufferSize = numIndices;

			//This data has now been sent over to the GPU so we can delete this CPU-side stuff
			delete[] indices;
			delete[] finalVerts;

			ResourceManagerInstance()->AddMesh(filename, pMesh);
		}
	}

	void OBJLoader::LoadMaterialLibrary(const std::string& filename)
	{
		if (ResourceManagerInstance()->GetMaterial(filename) != nullptr)
		{
			return;
		}

		std::ifstream inFile;
		inFile.open(".\\/Assets\\/" + filename);

		if (!inFile.good())
		{
			// TODO: Error Handling
			return;
		}
		else
		{
			// TODO: Error Handling

			std::string input = "";
			std::string currentMatName = "";
			Material* currentMat = nullptr;
			while (!inFile.eof())
			{
				inFile >> input; //Get the next input from the file

				//Check what type of input it was, we are only interested in vertex positions, texture coordinates, normals and indices
				if (input.compare("newmtl") == 0)
				{
					if (currentMat != nullptr && currentMatName != "")
					{
						if (ResourceManagerInstance()->AddMaterial(currentMatName, currentMat))
						{
							currentMat = nullptr;
							currentMatName.clear();
						}
						else
						{
							// TODO: Error Handling
						}
					}
					inFile >> currentMatName;
					if (currentMat == nullptr)
					{
						currentMat = new Material();
						currentMat->name = currentMatName;
					}
				}
				else if (input.compare("Ns") == 0)
				{
					inFile >> currentMat->specularPower;
				}
				else if (input.compare("Ni") == 0)
				{//Reflective index, unused
					char dump[128];
					inFile.getline(dump, 128);
				}
				else if (input.compare("d") == 0)
				{//Alpha, unused
					inFile >> currentMat->transparency;
				}
				else if (input.compare("Tr") == 0)
				{//Transparency, unused
					char dump[128];
					inFile.getline(dump, 128);
				}
				else if (input.compare("Tf") == 0)
				{//Transmission filter, unused
					char dump[128];
					inFile.getline(dump, 128);
				}
				else if (input.compare("illum") == 0)
				{//Illumination, unused
					char dump[128];
					inFile.getline(dump, 128);
				}
				else if (input.compare("Ka") == 0)
				{
					inFile >> currentMat->ambient.x;
					inFile >> currentMat->ambient.y;
					inFile >> currentMat->ambient.z;
				}
				else if (input.compare("Kd") == 0)
				{
					inFile >> currentMat->diffuse.x;
					inFile >> currentMat->diffuse.y;
					inFile >> currentMat->diffuse.z;
				}
				else if (input.compare("Ks") == 0)
				{
					inFile >> currentMat->specular.x;
					inFile >> currentMat->specular.y;
					inFile >> currentMat->specular.z;
				}
				else if (input.compare("Ke") == 0)
				{
					char dump[128];
					inFile.getline(dump, 128);
				}
				else if (input.compare("map_Ka") == 0)
				{
					std::string filename;
					inFile >> filename;
					if (ResourceManagerInstance()->AddTexture(filename))
					{
						currentMat->ambientTextureName = filename;
					}
					else
					{
						// TODO: Error Handling
					}
				}
				else if (input.compare("map_Kd") == 0)
				{
					std::string filename;
					inFile >> filename;
					if (ResourceManagerInstance()->AddTexture(filename))
					{
						currentMat->diffuseTextureName = filename;
					}
					else
					{
						// TODO: Error Handling
					}
				}
				else if (input.compare("map_Ks") == 0)
				{
					std::string filename;
					inFile >> filename;
					if (ResourceManagerInstance()->AddTexture(filename))
					{
						currentMat->specularTextureName = filename;
					}
					else
					{
						// TODO: Error Handling
					}
				}
			}
		}
	}
}