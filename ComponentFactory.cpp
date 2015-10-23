#include "ComponentFactory.h"
#include "ServiceLocator.h"
#include "Structures.h"

namespace Indecisive
{
	MeshComponent* ComponentFactory::MakeTestMesh()
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

		Geometry* pGeometry = new Geometry();
		IGraphics* pGraphics = static_cast<IGraphics*> (ServiceLocator::Instance()->Get("graphics"));
		pGeometry->vertexBuffer = pGraphics->InitVertexBuffer(vertices, 4);
		pGeometry->vertexBufferStride = sizeof(SimpleVertex);
		pGeometry->vertexBufferOffset = 0;
		pGeometry->indexBuffer = pGraphics->InitIndexBuffer(indices, 6);
		pGeometry->indexBufferSize = 6;
		pGeometry->indexBufferOffset = 0;

		return new MeshComponent(pGeometry, nullptr);
	}

	GameObject* ComponentFactory::MakeTestObject()
	{
		GameObject* pGameObject = new GameObject();
		pGameObject->AddDrawable(MakeTestMesh());
		return pGameObject;
	}
}