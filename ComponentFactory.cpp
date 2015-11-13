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
		unsigned short indices[] =
		{
			0, 1, 2,
			2, 1, 3,
		};

		auto pGeometry = new Geometry();
		auto pGraphics = static_cast<IGraphics*>(ServiceLocator::Instance()->Get("graphics"));
		pGeometry->vertexBuffer = pGraphics->InitVertexBuffer(vertices, 4);
		pGeometry->vertexBufferStride = sizeof(SimpleVertex);
		pGeometry->vertexBufferOffset = 0;
		pGeometry->indexBuffer = pGraphics->InitIndexBuffer(indices, 6);
		pGeometry->indexBufferSize = 6;
		pGeometry->indexBufferOffset = 0;

		auto pGameObject = new GameObject();
		pGameObject->AddDrawable(new MeshComponent(pGeometry, nullptr));
		return pGameObject;
	}

	GameObject* ComponentFactory::MakeTestObjectFromObj(std::string filepath)
	{
		auto pGameObject = new GameObject();
		auto ObjLoader = OBJLoader::OBJLoader();
		pGameObject->AddDrawable(new MeshComponent(ObjLoader.Load(filepath), nullptr));
		return pGameObject;
	}
}