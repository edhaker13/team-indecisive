#include "ResourceManager.h"

namespace Indecisive
{
	IResourceManager* ResourceManagerInstance() { static ResourceManager s; return &s; }
	Material* ResourceManager::AddMaterial(const std::string& name)
	{
		return nullptr;
	}

	Mesh*     ResourceManager::AddMesh(const std::string& name)
	{
		return nullptr;
	}
	
	Texture*  ResourceManager::AddTexture(const std::string& name)
	{
		return nullptr;
	}
	
	Material* ResourceManager::GetMaterial(const std::string& name)
	{
		return nullptr;
	}
	
	Mesh*     ResourceManager::GetMesh(const std::string& name)
	{
		return nullptr;
	}
	
	Texture*  ResourceManager::GetTexture(const std::string& name)
	{
		return nullptr;
	}
}
