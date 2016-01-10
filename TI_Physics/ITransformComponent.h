#pragma once
namespace Indecisive
{
	struct Vector3;
	class ITransformComponent
	{
	public:
		virtual void SetPosition(const Vector3&) = 0;
		virtual void SetPosition(float x, float y, float z) = 0;
		virtual void SetScale(const Vector3&) = 0;
		virtual void SetScale(float x, float y, float z) = 0;
		virtual void SetRotation(const Vector3&) = 0;
		virtual void SetRotation(float x, float y, float z) = 0;

		virtual Vector3 GetPosition() const = 0;
		virtual Vector3 GetPrevPosition() const = 0;
		virtual Vector3 GetScale() const = 0;
		virtual Vector3 GetRotation() const = 0;

		virtual void Update(float dt) = 0;
	};
}