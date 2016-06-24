#pragma once

namespace Indecisive
{
	struct Matrix;
	// Components that need to call Update() will also need to store it's changes in a Matrix.
	class IUpdatable
	{
	public:
		 virtual void Update(float elapsedTime) = 0;
		 virtual const Matrix& GetWorld() const = 0;
	};
};