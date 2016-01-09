#include "ResourceManager.h"
#include "IGraphics.h"
#include "Logger.h"

namespace Indecisive
{
	IResourceManager* ResourceManagerInstance() { static ResourceManager s; return &s; }

	bool ResourceManager::AddMaterial(const std::string& name, const Material* pMaterial)
	{
		return _materials.emplace(name, pMaterial).second;
	}

	bool ResourceManager::AddMesh(const std::string& name, const Mesh* pMesh)
	{
		return _meshes.emplace(name, pMesh).second;
	}

	bool ResourceManager::AddService(const std::string& name, const void* instance)
	{
		return _services.emplace(name, instance).second;
	};

	bool ResourceManager::AddTexture(const std::string& name)
	{
		if (_textures.find(name) == _textures.end())
		{
			Texture* pTexture = nullptr;
			auto pGraphics = static_cast<IGraphics*>(GetService("graphics"));
			const std::wstring file = L".\\/Assets\\/" + std::wstring(name.cbegin(), name.cend());
			if (pGraphics == nullptr)
			{
				TI_LOG_E("Couldn't find graphics interface");
				throw std::bad_alloc();
			}
			if (!pGraphics->CreateTextureFromFile(file.c_str(), &pTexture))
			{
				return false;
			}
			return _textures.emplace(name, pTexture).second;
		}
		return false;
	}

	Material* ResourceManager::GetMaterial(const std::string& name) const
	{
		return MapGet<Material>(_materials, name);
	}

	Mesh* ResourceManager::GetMesh(const std::string& name) const
	{
		return MapGet<Mesh>(_meshes, name);
	}

	void* ResourceManager::GetService(const std::string& name) const
	{
		return MapGet<void>(_services, name);
	};

	Texture* ResourceManager::GetTexture(const std::string& name) const
	{
		return MapGet<Texture>(_textures, name);
	}
}
