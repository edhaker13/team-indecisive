#pragma once
#include <map>
#include <vector>
#include "..\TI_Engine\DirectXMath.h"
#include "..\TI_Engine\IComponent.h"
#include "..\TI_Engine\IUpdatable.h"
#include "..\TI_Engine\LibraryAPI.h"

#pragma once
#ifdef _WINDLL
#	define AI_API __declspec(dllexport)
#else
#	define AI_API __declspec(dllimport)
#endif

namespace Indecisive
{
	//--------------------------------------------------------------------------------------------------
	// Path related structures
	//--------------------------------------------------------------------------------------------------
	struct Waypoint
	{
		char id;
		Vector3 position;
		std::vector<char> connectedIDs;
		Waypoint() {};
		Waypoint(char id, Vector3 position) : id(id), position(position) {};
		~Waypoint() { connectedIDs.clear(); };

		AI_API friend std::ostream& operator <<(std::ostream&, const Waypoint&);
		AI_API friend std::istream& operator >>(std::istream&, Waypoint&);
	};
	struct Node // Holds the cost of moving to a waypoint
	{
		float cost;
		Node* parent;
		Waypoint* waypoint;
		Node(float cost, Node* parent, Waypoint* waypoint)
			: cost(cost), parent(parent), waypoint(waypoint) {};
		~Node() { parent = nullptr; }

		friend std::ostream& operator <<(std::ostream&, const Node&);
	};
	typedef std::pair<char, char> Edge;
	typedef std::pair<Edge, float> EdgeCost;
	typedef std::map<Edge, float> EdgeMap; // Map of the cost of moving between nodes
	typedef std::vector<Node*> NodeList; // List of nodes, binds waypoints and costs
	typedef std::vector<Waypoint*> WaypointList; // List of possible waypoints to find a path
	typedef std::vector<Vector3> PositionList; // List of positions to follow as a path

	//--------------------------------------------------------------------------------------------------
	// Path Finding class, returns a node list with the calculated path
	//--------------------------------------------------------------------------------------------------
	class PathFinder // Calculates the shortest path from the supplied
	{
	public:
		// Using A* algorithm
		AI_API static PositionList Find(const Vector3&, const Vector3&, const WaypointList&, const EdgeMap&);
	
	private:
		static PositionList ConstructPath(Node*, const Vector3&);
		static float GetEdgeCost(const Waypoint*, const Waypoint*, const EdgeMap&);
		static float GetEuclidianCost(const Vector3&, const Vector3&);
		static Waypoint* GetNearest(const Vector3&, const WaypointList&);
		static bool IsInList(const NodeList&, const char&);
	};
	//--------------------------------------------------------------------------------------------------
	// Steering behaviours, likely to be reused so separate
	//--------------------------------------------------------------------------------------------------
	namespace Steering
	{
		Vector3 Arrive(const Vector3&, const Vector3&, const Vector3&, float, float);
		Vector3 Seek(const Vector3&, const Vector3&, const Vector3&, float);
		Vector3 Flee(const Vector3&, const Vector3&, const Vector3&, float);
		Vector3 PathFollow(const Vector3&, const Vector3& , bool&, bool&, bool&, const Vector3&, float, float
			, const PositionList&, PositionList::size_type&, Vector3&);
		void MoveInHeadingDirection(float, Vector3&, Vector3&, Vector3&, float, float);
	}

	//--------------------------------------------------------------------------------------------------
	// Binding class, path finding and steering to our engine methods
	//--------------------------------------------------------------------------------------------------
	class AIComponent : public IComponent, public IUpdatable
	{
	public:
		AIComponent(const WaypointList& ws, const EdgeMap& es, float decel, float mass, float maxSpeed)
			: IComponent("AI"), mWaypoints(ws), mEdges(es), mDeceleration(decel), mMass(mass)
			, mBehaviour(Behaviour::FollowBehaviour), mMaxSpeed(maxSpeed) {};
		enum Behaviour { NoBehaviour, FollowBehaviour, SeekBehaviour };
		const Behaviour& GetBehaviour() const { return mBehaviour; };
		const Vector3& GetTarget() const { return mTarget; };
		const Matrix& GetWorld() const override { return mWorld; };
		void SetBehaviour(const Behaviour& b) { mBehaviour = b; };
		void SetTarget(const Vector3& t)
		{ 
			mTarget = t;
			mPath = PathFinder::Find(mPosition, mTarget, mWaypoints, mEdges);
			mNewTarget = true;
			mHasTarget = true;
		}

		AI_API void Update(float) override;
	
	protected:
		bool mEndOnTarget = false;
		bool mNewTarget = false;
		bool mHasTarget = false;
		Behaviour mBehaviour;
		const EdgeMap& mEdges;
		float mDeceleration, mMass, mMaxSpeed;
		Matrix mWorld;
		PositionList mPath;
		PositionList::size_type mCurrentIndex = 0;
		Vector3 mCurrentTarget, mPosition, mSteering, mTarget, mVelocity;
		const WaypointList& mWaypoints;
	};
	//------------------------------------------------------------------------------------------------//
}