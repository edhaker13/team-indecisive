#pragma once
#include <list>
#include "IUpdatable.h"
#include "IDrawable.h"

namespace Indecisive
{
	class IGameObject : public IDrawable, IUpdatable
	{
	private:
		std::list<IDrawable*> _drawableComponents;
		std::list<IUpdatable*> _updatableComponents;
	public:
		virtual void Draw() override
		{
			for (IDrawable* pComponent : _drawableComponents)
			{
				pComponent->Draw();
			}
		}

		virtual void Update(float elapsedTime) override
		{
			for (IUpdatable* pComponent : _updatableComponents)
			{
				pComponent->Update(elapsedTime);
			}
		}
	};
};