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

	}
}