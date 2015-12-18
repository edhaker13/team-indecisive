#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "IGraphics.h"

namespace Indecisive
{
	IResourceManager* ResourceManagerInstance() { static ResourceManager s; return &s; }

	bool ResourceManager::AddMaterial(const std::string& name, const Material* pMaterial)
	{
		if (_materials.find(name) == _materials.end())
		{
			_materials[name] = const_cast<Material*>(pMaterial);
			return true;
		}
		return false;
	}

	bool ResourceManager::AddMesh(const std::string& name, const Mesh* pMesh)
	{
		if (_meshes.find(name) == _meshes.end())
		{
			_meshes[name] = const_cast<Mesh*>(pMesh);
			return true;
		}
		return false;
	}

	bool ResourceManager::AddTexture(const std::string& name)
	{
		if (_textures.find(name) == _textures.end())
		{
			Texture* pTexture = nullptr;
			const std::wstring file = L".\\Assets\\" + std::wstring(name.cbegin(), name.cend());
			auto pGraphics = static_cast<IGraphics*>(ServiceLocatorInstance()->Get("graphics"));
			if (pGraphics == nullptr || !pGraphics->CreateTextureFromFile(file.c_str(), &pTexture))
			{
				return false;
			}
			_textures[name] = pTexture;
			return true;
		}
		return false;
	}

	Material* ResourceManager::GetMaterial(const std::string& name)
	{
		if (_materials.find(name) != _materials.end())
		{
			return _materials[name];
		}
		return nullptr;
	}

	Mesh* ResourceManager::GetMesh(const std::string& name)
	{
		if (_meshes.find(name) != _meshes.end())
		{
			return _meshes[name];
		}
		return nullptr;
	}

	Texture*  ResourceManager::GetTexture(const std::string& name)
	{
		if (_textures.find(name) != _textures.end())
		{
			return _textures[name];
		}
		return nullptr;
	}
}
