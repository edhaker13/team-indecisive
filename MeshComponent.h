#pragma once
#include "Structures.h"


namespace Indecisive
{ 

	class MeshComponent
	{
	private:
		

	public:
		MeshComponent(Geometry* newGeometry, Material* newMaterial);
		~MeshComponent();

	private:
		Geometry* _Geometry;
		Material* _Material;

	};

}