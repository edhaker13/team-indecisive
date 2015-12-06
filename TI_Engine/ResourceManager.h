#pragma once
#include <map>
#include <string>
#include <vector>
#include "IResourceManager.h"
#include "Structures.h"

namespace Indecisive
{
	class ResourceManager: public IResourceManager
	{
	private:
		std::map<std::string, Material*> _materials;
		std::map<std::string, Mesh*> _meshes;
		std::map<std::string, Texture*> _textures;
		ResourceManager(ResourceManager const&) = delete;
		void operator=(ResourceManager const&) = delete;

	public:
		ResourceManager() {};
		~ResourceManager() {};
		virtual bool AddMaterial(const std::string& name, const Material*) override;
		virtual bool AddMesh(const std::string& name, const Mesh*) override;
		virtual bool AddTexture(const std::string&) override;
		virtual Material* GetMaterial(const std::string&) override;
		virtual Mesh* GetMesh(const std::string&) override;
		virtual Texture*  GetTexture(const std::string&) override;
	};
};