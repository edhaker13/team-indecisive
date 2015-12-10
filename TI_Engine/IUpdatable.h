#pragma once

namespace Indecisive
{
	struct Matrix;
	class IUpdatable
	{
	public:
		virtual void Update(float elapsedTime) = 0;
		virtual Matrix GetWorld() = 0;
	};
};