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
		std::list<const IDrawable*> _drawableComponents;
		std::list<IUpdatable*> _updatableComponents;
		Matrix _world;

	public:
		virtual void AddDrawable(const IDrawable* pComponent) override
		{
			if (pComponent != nullptr)
			{
				_drawableComponents.push_back(pComponent);
			}
		}

		virtual void AddUpdatable(IUpdatable* pComponent) override
		{
			if (pComponent != nullptr)
			{
				_updatableComponents.push_back(pComponent);
			}
		}

		virtual void Draw() const override
		{
			for (const auto* p : _drawableComponents)
			{
				p->Draw();
			}
		}

		virtual void Update(float elapsedTime) override
		{
			for (auto* p : _updatableComponents)
			{
				p->Update(elapsedTime);
				_world = p->GetWorld() * _world;
			}
		}

		virtual const Matrix& GetWorld() const override { return _world; }
	};
};