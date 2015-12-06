#pragma once
namespace Indecisive
{

	class ITransformComponent
	{
	public:
		virtual void SetPosition() = 0;

		virtual void SetScale() = 0;

		virtual void SetRotation() = 0;

		virtual void Update() = 0;
	};
}