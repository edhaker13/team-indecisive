#pragma once
#include <list>
#include "IGameObject.h"
#include "DirectXMath.h"

namespace Indecisive
{
	struct Matrix;
	class GameObject : public IGameObject
	{
	private:
		std::list<IDrawable*> _drawableComponents;
		std::list<IUpdatable*> _updatableComponents;
		Matrix _world;

	public:
		virtual void AddDrawable(IDrawable* pComponent)
		{
			if (pComponent != nullptr)
			{
				_drawableComponents.push_back(pComponent);
			}
		}

		virtual void AddUpdatable(IUpdatable* pComponent)
		{
			if (pComponent != nullptr)
			{
				_updatableComponents.push_back(pComponent);
			}
		}

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
				_world = pComponent->GetWorld() * _world;
			}
		}

		virtual Matrix GetWorld() override { return _world; }
	};
};