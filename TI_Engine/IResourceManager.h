#pragma once
#include <string>
#include "LibraryAPI.h"

namespace Indecisive
{
	struct Material;
	struct Mesh;
	struct Texture;

	struct IResourceManager
	{
		virtual Material* AddMaterial(const std::string&) = 0;
		virtual Mesh* AddMesh(const std::string&) = 0;
		virtual Texture* AddTexture(const std::string&) = 0;
		virtual Material* GetMaterial(const std::string&) = 0;
		virtual Mesh* GetMesh(const std::string&) = 0;
		virtual Texture*  GetTexture(const std::string&) = 0;
	};

	LIBRARY_API IResourceManager* ResourceManagerInstance();
}