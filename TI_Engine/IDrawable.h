#pragma once

namespace Indecisive
{
	// Components and Objects that need to call a Draw function will use this
	class IDrawable
	{
	public:
		virtual void Draw() const = 0;
	};
};