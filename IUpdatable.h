#pragma once

namespace Indecisive
{
	class IUpdatable
	{
	public:
		virtual void Update(int elapsedTime) = 0;
	};
};