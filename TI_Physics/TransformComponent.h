#pragma once
#include "ITransformComponent.h"
#include "..\TI_Engine\Structures.h"


#ifdef _WINDLL
#	define PHYSICS_API __declspec(dllexport)
#else
#	define PHYSICS_API __declspec(dllimport)
#endif

namespace Indecisive
{

	class TransformComponent : public ITransformComponent
	{
	public:
		PHYSICS_API TransformComponent();
		~TransformComponent();

		//Setters and Getters for Pos/Scal/Rotat

		void SetPosition(const Vector3& position) override { _prevPos = _position; _position = position; }
		void SetPosition(float x, float y, float z) override { _position.x = x; _position.y = y; _position.z = z; }
		Vector3 GetPrevPosition() const override { return _prevPos; }
		Vector3 GetPosition() const override { return _position; }

		void SetScale(const Vector3& scale) override { _scale = scale; }
		void SetScale(float x, float y, float z) override {_scale.x = x; _scale.y = y; _scale.z = z;}
		Vector3 GetScale() const override { return _scale; }

		void SetRotation(const Vector3& rotation) override { _rotation = rotation; }
		void SetRotation(float x, float y, float z) override {_rotation.x = x; _rotation.y = y; _rotation.z = z;}
		Vector3 GetRotation() const override { return _rotation; }
		const Matrix& GetWorld() const override { return _world; };
		void Update(float dt) override;

	private:
		Vector3 _position;
		Vector3 _rotation;
		Vector3 _scale;
		Vector3 _prevPos;

		Matrix _world;
		float restitution;

		TransformComponent* _parent;

	};

}