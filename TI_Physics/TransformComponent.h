#pragma once
#include "ITransformComponent.h"
#include "Structures.h"

namespace Indecisive
{

	class TransformComponent : public ITransformComponent
	{
	public:
		TransformComponent();
		~TransformComponent();

		//Setters and Getters for Pos/Scal/Rotat

		void SetPosition(const Vector3& position) override { _prevPos = _position; _position = position; }
		void SetPosition(float x, float y, float z) override { _position.x = x; _position.y = y; _position.z = z; }
		Vector3 GetPrevPosition() const { return _prevPos; }
		Vector3 GetPosition() const { return _position; }

		void SetScale(const Vector3& scale) override { _scale = scale; }
		void SetScale(float x, float y, float z) override {_scale.x = x; _scale.y = y; _scale.z = z;}
		Vector3 GetScale() const { return _scale; }

		void SetRotation(const Vector3& rotation) override { _rotation = rotation; }
		void SetRotation(float x, float y, float z) override {_rotation.x = x; _rotation.y = y; _rotation.z = z;}
		Vector3 GetRotation() const { return _rotation; }

		void Update() {};

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