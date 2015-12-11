#pragma once
#include "TransformComponent.h"
#include <string>


namespace Indecisive
{
	class PhysicsComp
	{
	public:
		PhysicsComp(ITransformComponent* Itransform);
		~PhysicsComp();

		void MoveConstVel(float t);
		void MoveConstAcc(float t);
		void Move(float newX, float newY, float newZ);

		Vector3 GetVel() const { return velocity; }
		void GetVelocity(Vector3 _velocity) { velocity = _velocity; }

		void SetVel(Vector3 _velocity) { velocity = _velocity; }

		float GetMass() const { return mass; };
		void SetMass(float _mass) { mass = _mass; }

		void UpdateNetForce();
		void UpdateAccel();
		void UpdateState();
		virtual void Update(float t);

		void SlidingMotion();
		void SlidingForce(float theta, float frCoef);

		void MotionInFluid(float t);
		void DragForce();
		void dragLamFlow();
		void TurbulantFlow();

		//Collision
		float GetCollisionRadius() const { return radius; }
		void SetCollisionRadius(float _radius) { radius = _radius; }

		bool CubeCollisionCheck(Vector3 position, float radius);
		void FloorCollisionCheck(Vector3 position);
		void ResolveCollision(PhysicsComp* particleModel1, PhysicsComp* particleModel2, float restitution);

		void SetSlidingForce();

		void GetDirection();

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