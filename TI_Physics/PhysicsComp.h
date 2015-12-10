#pragma once
#include "ITransformComponent.h"
#include "TransformComponent.h"

namespace Indecisive
{
	class PhysicsComp
	{
	public:
		PhysicsComp(ITransformComponent* _transform);
		~PhysicsComp();

		void MoveConstVel(float t);
		void MoveConstAcc(float t);
		void Move(float newX, float newY, float newZ);

		//void SetVel(float3 _velocity) { velocity = _velocity; }

		//float GetMass() const { return mass; };
		//void SetMass(float _mass) { mass = _mass; }

		//void UpdateNetForce();
		//void UpdateAccel();
		//void UpdateState();
		//virtual void Update(float t);

		//void SlidingMotion();
		//void SlidingForce(float theta, float frCoef);

		//void MotionInFluid(float t);
		//void DragForce();
		//void dragLamFlow();
		//void TurbulantFlow();

		////Collision
		//float GetCollisionRadius() const { return radius; }
		//void SetCollisionRadius(float _radius) { radius = _radius; }

		//bool CubeCollisionCheck(float3 position, float radius);
		//void FloorCollisionCheck(float3 position);
		//void ResolveCollision(ParticleModel* particleModel1, ParticleModel* particleModel2, float restitution);

		//void SetSlidingForce();

		//void GetDirection();

	protected:
		/*float3 velocity;
		float3 Acceleration;
		ITransformComponent* _transform;*/


	private:

		// float3 netForce;
		// float3 force;
		// float3 prevPosition;
		// float previous;
		float forceMag;
		float mass;

		//float3 prevVelocity;
		//float dragFactor;
		//bool sForce;
		//bool useConstAcc;
		
		//bool collisionCheck;
		//float radius;
		
		//float gravity;
		//float3 sForce;
		//float3 dragForce;
		//bool laminarFlow;
	};
}