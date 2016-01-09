#pragma once
#include "IDrawable.h"
#include "IUpdatable.h"

namespace Indecisive
{
	class IGameObject : public IDrawable, public IUpdatable
	{
	public:
		virtual void AddDrawable(const IDrawable*) = 0;
		virtual void AddUpdatable(IUpdatable*) = 0;
	};
}