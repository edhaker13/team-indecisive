#pragma once
#include "IComponent.h"
#include "IDrawable.h"
#include "IGraphics.h"
#include "IServiceLocator.h"

namespace Indecisive
{ 
	struct Geometry;

	class MeshComponent: public IComponent, public IDrawable
	{
	private:
		Geometry* _pGeometry;
		IGraphics* _pGraphics;

	public:
		MeshComponent(Geometry* geometry) : IComponent("Mesh"), _pGeometry(geometry)
		{
			_pGraphics = static_cast<IGraphics*> (ServiceLocatorInstance()->Get("graphics"));
		};
		/*~MeshComponent()
		{
			delete _pGeometry;
			delete _pMaterial;

			_pGeometry = nullptr;
			_pMaterial = nullptr;
		};/*need to find a way to delete this properly*/
		virtual void Draw() override
		{
			_pGraphics->DrawGeometry(_pGeometry);
		};
	};

}