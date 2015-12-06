#pragma once
#include "IComponent.h"
#include "IDrawable.h"
#include "IGraphics.h"
#include "IServiceLocator.h"

namespace Indecisive
{ 
	struct Mesh;

	class MeshComponent: public IComponent, public IDrawable
	{
	private:
		Mesh* _pMesh;
		IGraphics* _pGraphics;

	public:
		MeshComponent(Mesh* mesh) : IComponent("Mesh"), _pMesh(mesh)
		{
			_pGraphics = static_cast<IGraphics*> (ServiceLocatorInstance()->Get("graphics"));
		};
		/*~MeshComponent()
		{
			delete _pMesh;
			delete _pMaterial;

			_pMesh = nullptr;
			_pMaterial = nullptr;
		};/*need to find a way to delete this properly*/
		virtual void Draw() override
		{
			_pGraphics->DrawMesh(_pMesh);
		};
	};

}