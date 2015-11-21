#pragma once

namespace Indecisive
{
	class IUpdatable
	{
	public:
		virtual void Update(float elapsedTime) = 0;
	};
};