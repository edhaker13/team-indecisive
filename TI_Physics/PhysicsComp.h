#pragma once
#include "TransformComponent.h"
#include <string>

#pragma once
#ifdef _WINDLL
#	define PHYSICS_API __declspec(dllexport)
#else
#	define PHYSICS_API __declspec(dllimport)
#endif

namespace Indecisive
{
	class PhysicsComp
	{
	public:
		PHYSICS_API	PhysicsComp(ITransformComponent* Itransform);
		~PhysicsComp();

	PHYSICS_API	void MoveConstVel(float t);
	PHYSICS_API	void MoveConstAcc(float t);
	PHYSICS_API	void Move(float newX, float newY, float newZ);
	
	PHYSICS_API	Vector3 GetVel() const { return velocity; }
	PHYSICS_API	void GetVelocity(Vector3 _velocity) { velocity = _velocity; }
	
	PHYSICS_API	void SetVel(Vector3 _velocity) { velocity = _velocity; }

	PHYSICS_API	float GetMass() const { return mass; };
	PHYSICS_API	void SetMass(float _mass) { mass = _mass; }
	
	PHYSICS_API	void UpdateNetForce();
	PHYSICS_API	void UpdateAccel();
	PHYSICS_API	void UpdateState();
	PHYSICS_API	virtual void Update(float t);
	
	void SlidingMotion();
	void SlidingForce(float theta, float frCoef);

	void MotionInFluid(float t);
	void DragForce();
	void dragLamFlow();
	void TurbulantFlow();
	
			//Collision
	PHYSICS_API	float GetCollisionRadius() const { return radius; }
	PHYSICS_API	void SetCollisionRadius(float _radius) { radius = _radius; }

	PHYSICS_API	bool CubeCollisionCheck(Vector3 position, float radius);
	PHYSICS_API	void FloorCollisionCheck(Vector3 position);
	PHYSICS_API	void ResolveCollision(PhysicsComp* particleModel1, PhysicsComp* particleModel2, float restitution);

	PHYSICS_API	void SetSlidingForce();
	
	PHYSICS_API	void GetDirection();

	protected:
		Vector3 velocity;
		Vector3 Acceleration;
		ITransformComponent* _transform;

	private:

		Vector3 netForce;
		Vector3 force;
		Vector3 prevPosition;
		float previous;
		float forceMag;
		float mass;

		Vector3 prevVelocity;
		float dragFactor;
		bool useConstAcc;
		bool boolsForce;
		
		bool collisionCheck;
		float radius;
		
	

		float gravity;
		Vector3 sForce;
		Vector3 drag;
		bool laminar;

	};
}