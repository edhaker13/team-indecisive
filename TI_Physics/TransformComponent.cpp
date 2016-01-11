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

	void TransformComponent::Update(float dt)
	{
		
		// Calculate world matrix
		Matrix scale = Matrix::CreateScale(_scale.x, _scale.y, _scale.z);
		Matrix rotation = Matrix::CreateFromYawPitchRoll(_rotation.x, _rotation.y, _rotation.z);
		Matrix translation = Matrix::CreateTranslation(_position);
		
		_world = scale * rotation * translation;

		if (_parent != nullptr) _parent->Update(dt);
	}

}