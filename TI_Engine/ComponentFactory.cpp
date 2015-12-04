#include "ComponentFactory.h"
#include "OBJLoader.h"
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
		auto pGraphics = static_cast<IGraphics*>(ServiceLocatorInstance()->Get("graphics"));
		pMesh->vertexBuffer = pGraphics->InitVertexBuffer(vertices, 4);
		pMesh->vertexBufferStride = sizeof(SimpleVertex);
		pMesh->vertexBufferOffset = 0;
		pMesh->indexBuffer = pGraphics->InitIndexBuffer(indices, 6);
		pMesh->indexBufferSize = 6;
		pMesh->indexBufferOffset = 0;

		auto pGameObject = new GameObject();
		pGameObject->AddDrawable(new MeshComponent(pMesh));
		return pGameObject;
	}

	GameObject* ComponentFactory::MakeTestObjectFromObj(std::string filepath)
	{
		auto ObjLoader = OBJLoader::OBJLoader();
		auto objMesh = ObjLoader.Load(filepath);
		if (objMesh != nullptr)
		{
			auto pGameObject = new GameObject();
			pGameObject->AddDrawable(new MeshComponent(objMesh));
			return pGameObject;
		}
		return nullptr;
	}
}