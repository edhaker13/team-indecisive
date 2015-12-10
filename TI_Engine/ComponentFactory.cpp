#include "ComponentFactory.h"
#include "OBJLoader.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "Structures.h"

namespace Indecisive
{
	GameObject* ComponentFactory::MakeTestObject()
	{
		SimpleVertex vertices[] =
		{
			{ Vector3(-1.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vector3(1.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3(-1.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
			{ Vector3(1.0f, -1.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		};
		WORD indices[] =
		{
			0, 1, 2,
			2, 1, 3,
		};

		auto pMesh = new Mesh();
		auto pSubObject = new SubObject();
		auto pGameObject = new GameObject();
		auto pMeshComponent = new MeshComponent(*pMesh);
		auto pGraphics = static_cast<IGraphics*>(ServiceLocatorInstance()->Get("graphics"));
		
		pMesh->vertexBuffer = pGraphics->InitVertexBuffer(vertices, 4);
		pSubObject->vertexEnd = 4;
		pMesh->vertexBufferStride = sizeof(SimpleVertex);
		pMesh->vertexBufferOffset = pSubObject->indexStart = 0;
		pMesh->indexBuffer = pGraphics->InitIndexBuffer(indices, 6);
		pMesh->indexBufferSize = pSubObject->indexSize = 6;
		pMesh->indexBufferOffset = 0;

		Texture* none = nullptr;
		if (ResourceManagerInstance()->AddTexture("WhiteTex.dds"))
			 none = ResourceManagerInstance()->GetTexture("WhiteTex.dds");
		pSubObject->ambientTexture = none;
		pSubObject->diffuseTexture = none;
		pSubObject->specularTexture= none;
		pMeshComponent->AddGroup(pSubObject);
		pGameObject->AddDrawable(pMeshComponent);
		return pGameObject;
	}

	GameObject* ComponentFactory::MakeTestObjectFromObj(std::string filename)
	{
		if (!ResourceManagerInstance()->AddTexture("WhiteTex.dds"))
			bool failed = true; //Error <No default texture>
		auto ObjLoader = OBJLoader::OBJLoader();
		ObjLoader.Load(filename);
		auto pMeshComponent = ObjLoader.ConstructFromMesh(filename);
		if (pMeshComponent != nullptr)
		{
			auto pGameObject = new GameObject();
			pGameObject->AddDrawable(pMeshComponent);
			return pGameObject;
		}
		return nullptr;
	}
}