#pragma once
#include <vector>
#include "IComponent.h"
#include "IDrawable.h"
#include "IGraphics.h"
#include "IServiceLocator.h"

namespace Indecisive
{
	struct Mesh;

	class MeshComponent : public IComponent, public IDrawable
	{
	private:
		Mesh& _mesh;
		IGraphics& _graphics;
		std::vector<SubObject*> _groups;
		std::vector<int> _priorityGroups;

	public:
		MeshComponent(Mesh& mesh) :
			IComponent("Mesh"), _mesh(mesh), _graphics(*static_cast<IGraphics*>(ServiceLocatorInstance()->Get("graphics")))
		{};

		~MeshComponent()
		{
			_groups.clear();
			_priorityGroups.clear();
		};

		void AddPriorityGroup(int index) { _priorityGroups.push_back(index); }

		void AddGroup(SubObject* group) { _groups.push_back(group); }

		virtual void Draw() override
		{
			auto g = _groups[0];
			for (auto i : _priorityGroups)
			{
				g = _groups[i];

				/*cb->diffuseMaterial = currentGroup->mat->diffuse;
				cb->ambientMaterial = currentGroup->mat->ambient;
				cb->specularMtrl = currentGroup->mat->specular;
				cb->spec = currentGroup->mat->specularExponent;
				cb->transparency = XMFLOAT4(currentGroup->mat->transparency, 1.0f, 1.0f, 1.0f);
				_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, cb, 0, 0);*/
				_graphics.DrawMesh(_mesh, *g);
			}
		};
	};

}