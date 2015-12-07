#pragma once
#include <vector>
#include "IComponent.h"
#include "IDrawable.h"
#include "IGraphics.h"
#include "IServiceLocator.h"

namespace Indecisive
{ 
	struct Mesh;

	class MeshComponent: public IComponent, public IDrawable
	{
	private:
		Mesh& _mesh;
		IGraphics& _graphics;
		std::vector<SubObject*> _groups;//Since graphs arent needed within a model we'll just hold everything on the game level
		std::vector<int> _priorityGroups;

	public:
		MeshComponent(Mesh& mesh) : IComponent("Mesh"), _mesh(mesh), _graphics(* static_cast<IGraphics*> (ServiceLocatorInstance()->Get("graphics")))
		{
		};

		~MeshComponent()
		{
			_groups.clear();
			_priorityGroups.clear();
		};

		void AddPriorityGroup(int index) { _priorityGroups.push_back(index); }

		void AddGroup(SubObject* group) { _groups.push_back(group); }
		
		virtual void Draw() override
		{
			for (auto g : _groups) _graphics.DrawMesh(_mesh, *g);
		};
	};

}