#pragma once
#include "ITransformComponent.h"

namespace Indecisive
{

	class TransformComponent : public ITransformComponent
	{
	public:
		TransformComponent();
		~TransformComponent();

		//Setters and Getters for Pos/Scal/Rotat

		//void SetPosition(float3 position) { _prevPos = _position; _position = position; }
		//void SetPosition(float x, float y, float z) { _position.x = x; _position.y = y; _position.z = z; }
		//float3 GetPrevPosition() const { return _prevPos; }
		//float3 GetPosition() const { return _position; }

		//void SetScale(float3 scale) { _scale = scale; }
		//void SetScale(float x, float y, float z) {_scale.x = x; _scale.y = y; _scale.z = z;}
		//float3 GetScale() const { return _scale }

		//void SetRotation(float3 rotation) {_rotation = rotation;)
		//void SetRotation(float x, float y, float z) {_rotation.x = x; _rotation.y = y; _rotation.z = z;}
		//float3 GetRotation() const { return _rotation }

		void Update(float t);

	private:
		/*float3 _position;
		float3 _rotation;
		float3 _scale;
		float3 _prevPos;

		XMFLOAT4X4 _world;
		float restitution;

		Transform* _parent;*/

	};

}