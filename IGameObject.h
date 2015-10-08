#pragma once
#include <list>
#include "IComponent.h"
#include "IDrawable.h"

namespace Indecisive
{
	class IGameObject: public IDrawable, IUpdatable
	{
	public:
		std::list<IComponent> components;
		virtual void Update(int elapsedTime) override {
			for each (IComponent component in components)
			{
				component.Update(elapsedTime);
			}
		}
	};
};