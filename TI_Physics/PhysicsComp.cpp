#include "PhysicsComp.h"

namespace Indecisive
{
	PhysicsComp::PhysicsComp(ITransformComponent* Itransform)
	{
		_transform = Itransform;
		Acceleration = Vector3(0.0f, 0.0f, -0.1f);
		mass = 100.0f;
		force = Vector3(1000.0f, 0.0f, 0.0f);
		velocity = Vector3(10.0f, 0.0f, -0.1f);
		netForce = Vector3(0.0f, 0.0f, 0.0f);
		forceMag = 10.0f;
		sForce = Vector3(0.0f, 0.0f, 0.0f);
		dragFactor = 1.0f;
		gravity = -9.81f;

		useConstAcc = true;
		laminar = true;

		radius = 20.0f;

		collisionCheck = false;
		boolsForce = true;

	
	}

	void PhysicsComp::Move(float newX, float newY, float newZ)
	{
		Vector3 position = _transform->GetPosition();
		position.x += newX;
		position.y += newY;
		position.z += newZ;
		_transform->SetPosition(position);
	}

	PhysicsComp::~PhysicsComp()
	{

	}

	void PhysicsComp::MoveConstVel(float t)
	{
		Vector3 prevPosition = _transform->GetPosition();

		Vector3 position;
		position.x = prevPosition.x + velocity.x * t;
		position.y = prevPosition.y + velocity.y * t;
		position.z = prevPosition.z + velocity.z * t;

		_transform->SetPosition(position);
		gravity = 9.81f;
	}

	void PhysicsComp::MoveConstAcc(float t)
	{
		t /= 1000;

		Vector3 position = _transform->GetPosition();

		//update world position of object by adding displacement to previously calculated position
		position.x += velocity.x * t + 0.5 * Acceleration.x * t * t;
		position.y += velocity.y * t + 0.5 * Acceleration.y * t * t;
		position.z += velocity.z * t + 0.5 * Acceleration.z * t * t;

		// Set velocity
		velocity.x += Acceleration.x * t;
		velocity.y += Acceleration.y * t;
		velocity.z += Acceleration.z * t;

		_transform->SetPosition(position);
	}

	void PhysicsComp::MotionInFluid(float t)
	{
		DragForce();
		//UpdateState();
		MoveConstAcc(t);
	}

	void PhysicsComp::dragLamFlow()
	{
		drag.x = -dragFactor * velocity.x;
		drag.y = -dragFactor * velocity.y;
		drag.z = -dragFactor * velocity.z;
	}

	void PhysicsComp::TurbulantFlow()
	{
		float velMag;
		// calculate magnitude of velocity
		velMag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.y * velocity.y));

		// calculate unit vector of velocity
		Vector3 unitVel;
		unitVel.x = (velocity.x / velMag);
		unitVel.y = (velocity.y / velMag);
		unitVel.z = (velocity.z / velMag);

		// calculate magnitude of drag force
		float dragMag;
		dragMag = dragFactor * velMag * velMag;

		// calculate of x- and y-components of drag force
		drag.x = -dragMag * unitVel.x;
		drag.y = -dragMag * unitVel.y;

	}

	void PhysicsComp::DragForce()
	{
		if (laminar)
		{
			// calculate drag force for laminar flow
			dragLamFlow();
		}
		else
		{
			// calculate drag force for turbulent flow
			TurbulantFlow();
		}

	}

	void PhysicsComp::SlidingForce(float theta, float frCoef)
	{
		forceMag = mass * gravity * (sin(theta) - frCoef * cos(theta));

		sForce.x = forceMag * cos(theta);
		sForce.y = forceMag * sin(theta);
		sForce.z = forceMag * cos(theta);

		if (sForce.y > 0.0f)
		{
			/*sForce.x = 0.0f;
			sForce.y = 0.0f;*/
		}
	}

	void PhysicsComp::UpdateNetForce()
	{
		netForce.x = drag.x + sForce.x;
		netForce.y = drag.y + sForce.y;
		netForce.z = drag.z + sForce.z;
	}

	bool PhysicsComp::CubeCollisionCheck(Vector3 position2, float radius2)
	{
		Vector3 position = _transform->GetPosition();

		Vector3 difference;
		difference.x = position2.x - position.x;
		difference.y = position2.y - position.y;
		difference.z = position2.z - position.z;

		float differenceMag = sqrt((difference.x* difference.x) + (difference.y * difference.y) + (difference.z * difference.z));

		float RadDistance = radius + radius2;

		if (differenceMag <= RadDistance)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void PhysicsComp::FloorCollisionCheck(Vector3 position)
	{
		Vector3 position2 = _transform->GetPosition();

		Vector3 difference;

		difference.y = position2.y - position.y;

		float differenceMag = sqrt(difference.y * difference.y);

		if (differenceMag <= radius)
		{
			velocity.y = -velocity.y;
			boolsForce = true;
		}
		else
		{
			boolsForce = false;
		}
	}

	void PhysicsComp::UpdateAccel()
	{
		Acceleration.x = netForce.x / mass;
		Acceleration.y = netForce.y / mass;
		Acceleration.z = netForce.z / mass;
	}

	void PhysicsComp::UpdateState()
	{
		UpdateNetForce();
		UpdateAccel();
	}

	void PhysicsComp::ResolveCollision(PhysicsComp* particleModel1, PhysicsComp* particleModel2, float restitution)
	{
		Vector3 Object1prev = particleModel1->_transform->GetPrevPosition();
		Vector3 Object2prev = particleModel2->_transform->GetPrevPosition();

		float mass1 = particleModel1->GetMass();
		float mass2 = particleModel2->GetMass();

		Vector3 vel1 = particleModel1->GetVel();
		Vector3 vel2 = particleModel2->GetVel();

		restitution = 1.0f;

		particleModel1->_transform->SetPosition(Object1prev);
		particleModel2->_transform->SetPosition(Object2prev);
		//Finalvelocity = Object1mass * velocity + Object2mass * velocity2 + Object2mass

		velocity.x = ((mass1 *(vel1.x)) + (mass2 * (vel2.x)) + (mass2 * restitution * (vel2.x - vel1.x))) / (mass1 + mass2);
		velocity.y = ((mass1 *(vel1.y)) + (mass2 * (vel2.y)) + (mass2 * restitution * (vel2.y - vel1.y))) / (mass1 + mass2);
		velocity.z = ((mass1 *(vel1.z)) + (mass2 * (vel2.z)) + (mass2 * restitution * (vel2.z - vel1.z))) / (mass1 + mass2);

		vel2.x = ((mass1 *(vel1.x)) + (mass2 * (vel2.x)) + (mass1 * restitution * (vel1.x - vel2.x))) / (mass1 + mass2);
		vel2.y = ((mass1 *(vel1.y)) + (mass2 * (vel2.y)) + (mass1 * restitution * (vel1.x - vel2.x))) / (mass1 + mass2);
		vel2.z = ((mass1 *(vel1.z)) + (mass2 * (vel2.z)) + (mass1 * restitution * (vel1.x - vel2.x))) / (mass1 + mass2);

		particleModel2->SetVel(vel2);
	}

	void PhysicsComp::Update(float t)
	{
		_transform->Update(t);
		if (useConstAcc)
		{
			if (boolsForce)
			{
				SlidingForce(XMConvertToRadians(90.0f), 0.2f);
			}
			else
			{
				sForce.x = 0.0f;
				sForce.y = mass * gravity;
				sForce.z = 0.0f;
			}

			DragForce();
			MoveConstAcc(t);
			UpdateState();

		}
		else
		{
			MoveConstVel(t);
			UpdateAccel();
		
		}
	
	}



}

