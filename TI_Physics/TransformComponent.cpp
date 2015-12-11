#include "TransformComponent.h"


namespace Indecisive
{
	TransformComponent::TransformComponent()
	{
		_parent = nullptr;
		_position = Vector3();
		_rotation = Vector3();
		_scale = Vector3();

	}

	TransformComponent::~TransformComponent()
	{

	}

	void TransformComponent::Update(float t)
	{
		// Calculate world matrix
		Matrix scale = scale.CreateScale(_scale.x, _scale.y, _scale.z);
		Matrix rotation = rotation.CreateRotationX(_rotation.x) * rotation.CreateRotationY(_rotation.y) * rotation.CreateRotationZ(_rotation.z);
		Matrix translation = translation.CreateTranslation(_position.x, _position.y, _position.z);
		
		_world = scale * rotation * translation;

	}
}