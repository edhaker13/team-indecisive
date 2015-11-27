#pragma once
#include "IComponent.h"
#include "IDrawable.h"
#include "IGraphics.h"
#include "ServiceLocator.h"

namespace Indecisive
{ 
	struct Geometry;
	struct Material;

	class MeshComponent: public IComponent, public IDrawable
	{
	private:
		Geometry* _pGeometry;
		IGraphics* _pGraphics;
		Material* _pMaterial;

	public:
		MeshComponent(Geometry* geometry, Material* material) : IComponent("Mesh"), _pGeometry(geometry), _pMaterial(material) 
		{
			_pGraphics = static_cast<IGraphics*> (ServiceLocatorInstance()->Get("graphics"));
		};
		~MeshComponent()
		{
			delete _pGeometry;
			delete _pMaterial;

			_pGeometry = nullptr;
			_pMaterial = nullptr;
		};
		virtual void Draw() override
		{
			_pGraphics->DrawGeometry(_pGeometry);
		};
	};

}