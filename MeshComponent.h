#pragma once
#include "IComponent.h"
#include "IDrawable.h"
#include "GraphicsDirectX.h"

namespace Indecisive
{ 
	struct Geometry;
	struct Material;

	class MeshComponent: public IComponent, public IDrawable
	{
	private:
		Geometry* _geometry;
		Material* _material;

	public:
		MeshComponent(Geometry* geometry, Material* material) : IComponent("Mesh"), _geometry(geometry), _material(material) {};
		~MeshComponent()
		{
			delete _geometry;
			delete _material;

			_geometry = nullptr;
			_material = nullptr;
		};
		virtual void Draw() override
		{
			/// TODO:: REMOVE GRAPHICS DEPENDCY !!!!!!
			GraphicsDirectX::Instance().DrawGeometry(_geometry);
		};
	};

}