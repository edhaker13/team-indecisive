#include "ComponentFactory.h"
#include "GameObject.h"
#include "OBJLoader.h"
#include "IResourceManager.h"
#include "Structures.h"

namespace Indecisive
{
	IGameObject* ComponentFactory::MakeTestObject()
	{
		Vertex vertices[] =
		{
			{ Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.333333f, 0.666667f, -0.666667f), Vector2(-1.0f, 1.0f) },
			{ Vector3(1.0f, 1.0f, 0.0f), Vector3(-0.816497f, 0.408248f, -0.408248f), Vector2(1.0f, 1.0f) },
			{ Vector3(-1.0f, -1.0f, 0.0f), Vector3(-0.333333f, 0.666667f, 0.666667f), Vector2(-1.0f, -1.0f) },
			{ Vector3(1.0f, -1.0f, 0.0f), Vector3(0.816497f, 0.408248f, 0.408248f), Vector2(1.0f, -1.0f) },
		};
		WORD indices[] =
		{
			0, 1, 2,
			2, 1, 3,
		};
		// Create structures to be used
		auto pMesh = new Mesh();
		auto pSubObject = new SubObject();
		auto pGameObject = new GameObject();
		auto pMeshComponent = new MeshComponent(*pMesh);
		// Load graphics from locator
		auto pGraphics = static_cast<IGraphics*>(ResourceManagerInstance()->GetService("graphics"));

		if (pGraphics == nullptr)
		{
			// TODO: Error Handling
		}

		pMesh->vertexBuffer = pGraphics->InitVertexBuffer(vertices, 4);
		pSubObject->vertexEnd = 4;
		pMesh->vertexBufferStride = sizeof(Vertex);
		pMesh->vertexBufferOffset = pSubObject->indexStart = 0;
		pMesh->indexBuffer = pGraphics->InitIndexBuffer(indices, 6);
		pMesh->indexBufferSize = pSubObject->indexSize = 6;
		pMesh->indexBufferOffset = 0;

		Texture* none = nullptr;
		// Load default `none` texture
		if (ResourceManagerInstance()->AddTexture("WhiteTex.dds"))
		{
			none = ResourceManagerInstance()->GetTexture("WhiteTex.dds");
		}
		else
		{
			// TODO: Error Handling
		}
		pSubObject->ambientTexture = none;
		pSubObject->diffuseTexture = none;
		pSubObject->specularTexture = none;
		pMeshComponent->AddGroup(pSubObject);
		pGameObject->AddDrawable(pMeshComponent);
		return pGameObject;
	}

	IGameObject* ComponentFactory::MakeObjectFromObj(const std::string& filename)
	{
		// Load default `none` texture
		if (!ResourceManagerInstance()->AddTexture("WhiteTex.dds"))
		{
			// TODO: Error Handling
		}
		auto ObjLoader = OBJLoader();
		ObjLoader.Load(filename);
		auto pMeshComponent = ObjLoader.ConstructFromMesh(filename);
		if (pMeshComponent != nullptr)
		{
			auto pGameObject = new GameObject();
			pGameObject->AddDrawable(pMeshComponent);
			return pGameObject;
		}
		// TODO: Error Handling
		return nullptr;
	}
}