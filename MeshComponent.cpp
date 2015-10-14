#include "MeshComponent.h"

namespace Indecisive
{
	MeshComponent::MeshComponent(Geometry* newGeometry, Material* newMaterial)
	{
		newGeometry = _Geometry;
		newMaterial = _Material;
	}

	MeshComponent::~MeshComponent()
	{
		delete _Geometry;
		delete _Material;

		_Geometry = nullptr;
		_Material = nullptr;
	}
}