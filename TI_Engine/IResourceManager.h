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
		virtual bool AddMaterial(const std::string& name, const Material*) = 0;
		virtual bool AddMesh(const std::string& name, const Mesh*) = 0;
		virtual bool AddService(const std::string& name, const void*) = 0;
		virtual bool AddTexture(const std::string&) = 0;
		virtual Material* GetMaterial(const std::string&) const = 0;
		virtual Mesh* GetMesh(const std::string&) const = 0;
		virtual void* GetService(const std::string&) const = 0;
		virtual Texture* GetTexture(const std::string&) const = 0;
	};

	LIBRARY_API IResourceManager* ResourceManagerInstance();
}