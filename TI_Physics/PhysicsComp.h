#pragma once

namespace Indecisive
{
	class PhysicsComp
	{
	public:
		PhysicsComp();
		~PhysicsComp();

	protected:


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