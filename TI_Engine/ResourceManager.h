#pragma once
#include <map>
#include <string>
#include <vector>
#include "IResourceManager.h"
#include "Structures.h"

namespace Indecisive
{
	class ResourceManager : public IResourceManager
	{
	private:
		std::map<std::string, const Material*> _materials;
		std::map<std::string, const Mesh*> _meshes;
		std::map<std::string, const void*> _services;
		std::map<std::string, const Texture*> _textures;
		ResourceManager(ResourceManager const&) = delete;
		void operator=(ResourceManager const&) = delete;

	public:
		ResourceManager() {}; 
		~ResourceManager() {};
		virtual bool AddMaterial(const std::string& name, const Material*) override;
		virtual bool AddMesh(const std::string& name, const Mesh*) override;
		virtual bool AddService(const std::string& name, const void*) override;
		virtual bool AddTexture(const std::string&) override;
		virtual Material* GetMaterial(const std::string&) override;
		virtual Mesh* GetMesh(const std::string&) override;
		virtual void* GetService(const std::string&) override;
		virtual Texture* GetTexture(const std::string&) override;
	};

	template <class T>
	T* MapGet(const std::map<std::string, const T*>& m, const std::string& key)
	{
		auto it = m.find(key);
		if (it != m.end()) return const_cast<T*>(it->second);
		return nullptr;
	};
};