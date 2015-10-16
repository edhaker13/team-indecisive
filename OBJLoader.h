#pragma once
#include "Structures.h"
#include "MeshComponent.h"

namespace Indecisive
{
	class OBJLoader
	{
	public:
		OBJLoader();
		~OBJLoader();

	private:
		Geometry Load();

		bool FindSimilarVertex();

		void CreateIndices();


	};
}