#include <algorithm>
#include <iostream>
#include <sstream>
#include "..\TI_Engine\SceneGraph.h"
#include "AIComponent.h"

namespace Indecisive
{
	//--------------------------------------------------------------------------------------------------
	std::ostream& operator <<(std::ostream& s, const Waypoint& w)
	{
		return s << "ID " << w.id << "\t" << w.position;
	}
	std::istream& operator >>(std::istream& s, Waypoint& w)
	{
		std::string token;
		s >> w.id >> w.position.x >> w.position.y >> w.position.z >> token;
		std::stringstream ss(token);
		while (std::getline(ss, token, ',')) { w.connectedIDs.push_back(token[0]); }
		return s;
	}
	std::ostream& operator <<(std::ostream& s, const Node& n)
	{
		if (n.parent != nullptr)
			return s << *n.waypoint << "\t$" << n.cost << "\tParent(" << *n.parent << ")";
		else
			return s << *n.waypoint << "\t$" << n.cost;
	}
	//--------------------------------------------------------------------------------------------------
	PositionList PathFinder::Find(const Vector3& start, const Vector3& end, const WaypointList& waypoints
		, const EdgeMap& edges)
	{
		NodeList openNodes, closedNodes;
		PositionList path;
		Node* current = nullptr;

		auto nearestToStart = GetNearest(start, waypoints);
		auto nearestToEnd = GetNearest(end, waypoints);

		if (nearestToStart == nearestToEnd || nearestToStart == nullptr || nearestToEnd == nullptr)
		{
			return path;
		}
		openNodes.push_back(new Node(0, nullptr, nearestToStart));
		while (!openNodes.empty())
		{
			for (auto n : openNodes)
			{
				if (current == nullptr || n->cost < current->cost)
				{
					current = n;
				}
			}
			if (current->waypoint == nearestToEnd)
			{
				return ConstructPath(current, end);
			}
			for (auto id : current->waypoint->connectedIDs)
			{

				if (IsInList(openNodes, id) || IsInList(closedNodes, id))
				{
					// check new cost < current cost???
					continue;
				}
				float cost = current->cost;
				auto connected = *(std::find_if(waypoints.cbegin(), waypoints.cend(),
					[id](Waypoint* n){ return n->id == id; }));
				cost += GetEdgeCost(current->waypoint, connected, edges);
				cost += GetEuclidianCost(connected->position, end);
				openNodes.push_back(new Node(cost, current, connected));
			}
			// To advance the list, current needs to be added in closed and removed from open
			closedNodes.push_back(current);
			openNodes.erase(std::remove_if(openNodes.begin(), openNodes.end(),
				[current](Node* p){ return p == current; }));
			current = nullptr;
		}
		return path;
	}
	//--------------------------------------------------------------------------------------------------
	PositionList PathFinder::ConstructPath(Node* targetNode, const Vector3& target)
	{
		PositionList path;
		auto current = targetNode;
		while (current != nullptr)
		{
			path.push_back(current->waypoint->position);
			current = current->parent;
		}
		// Path starts at end, needs to be reversed
		std::reverse(path.begin(), path.end());
		return path;
	}
	//--------------------------------------------------------------------------------------------------
	float PathFinder::GetEdgeCost(Waypoint const* from, Waypoint const* to, const EdgeMap& edges)
	{
		auto it = std::find_if(edges.begin(), edges.end(), [from, to](const EdgeCost& e){
			return e.first.first == from->id && e.first.second == to->id
				|| e.first.first == to->id && e.second == from->id; });
			if (it != edges.cend())
			{
				return it->second;
			}
			return std::numeric_limits<float>::max();
	}
	//--------------------------------------------------------------------------------------------------
	float PathFinder::GetEuclidianCost(const Vector3& positionOne, const Vector3& positionTwo)
	{
		return Vector3::DistanceSquared(positionOne, positionTwo);
	}
	//--------------------------------------------------------------------------------------------------
	Waypoint* PathFinder::GetNearest(const Vector3& position, const WaypointList& waypoints)
	{
		Waypoint* nearest = nullptr;
		auto minDist = std::numeric_limits<float>::max();
		auto dist = 0.f;

		for (auto w : waypoints)
		{
			dist = Vector3::DistanceSquared(position, w->position);

			if (dist < minDist)
			{
				minDist = dist;
				nearest = w;
			}
		}

		return nearest;
	}
	//--------------------------------------------------------------------------------------------------
	bool PathFinder::IsInList(const NodeList& nodes, char const& id)
	{
		auto it = std::find_if(nodes.cbegin(), nodes.cend(),
			[id](Node* n){ return n->waypoint->id == id; });
		return it != nodes.end();
	}
	//------------------------------------------------------------------------------------------------//

	//-----------------------------------------------------------------------------------------------
	float Steering::GetMinDistanceInPath(const PositionList& path)
	{
		static float result = 0;
		if (result == 0)
		{
			float minDist = std::numeric_limits<float>::max();
			Vector3 lastV = path[0];
			std::for_each(path.cbegin() + 1, path.cend(),
				[&minDist, &lastV](Vector3 v){
				auto d = Vector3::DistanceSquared(lastV, v);
				lastV = v;
				if (d < minDist) minDist = d / 2;
			});
			result = minDist;
		}
		return result;
	}
	//--------------------------------------------------------------------------------------------------
	Vector3 Steering::Arrive(const Vector3& position, const Vector3& target, const Vector3& velocity
		, float deceleration, float maxSpeed)
	{
		Vector3 vectorToTarget = target - position;
		// calculate the distance to the target
		auto distance = vectorToTarget.Length();

		if (distance > 0)
		{
			float speed = distance / deceleration;
			//speed = std::min(speed, maxSpeed);

			Vector3 resultingVelocity = Vector3::Normalize(vectorToTarget) * speed;

			return (resultingVelocity - velocity);
		}

		return Vector3::Zero;
	}
	//--------------------------------------------------------------------------------------------------
	Vector3 Steering::Seek(const Vector3& position, const Vector3& target, const Vector3& velocity
		, float maxSpeed)
	{
		auto resultingVelocity = Vector3::Normalize(target - position) * maxSpeed;

		return (resultingVelocity - velocity);
	}
	//--------------------------------------------------------------------------------------------------
	Vector3 Steering::Flee(const Vector3& position, const Vector3& target, const Vector3& velocity
		, float maxSpeed)
	{
		//only flee if the target is within 'panic distance'. Work in distance squared space.
		static const float PanicDistanceSq = 100.0f * 100.0f;
		if (Vector3::DistanceSquared(position, target) > PanicDistanceSq)
		{
			return Vector3::Zero;
		}

		auto resultingVelocity = Vector3::Normalize(position - target) * maxSpeed;

		return (resultingVelocity - velocity);
	}
	//--------------------------------------------------------------------------------------------------
	Vector3 Steering::PathFollow(const Vector3& position, const Vector3& endTarget, bool& hasTarget, bool& newTarget
		, bool& endOnTarget, const Vector3& velocity, float decel, float maxSpeed
		, const PositionList& path, PositionList::size_type& currentIndex, const TreeNode& node)
	{
		Vector3 currentTarget;

		// No target set, don't do anything
		if (!hasTarget && !newTarget)
		{
			return Vector3::Zero;
		}

		assert(!path.empty());
		// Get current target in model space
		auto world = Matrix::CreateTranslation(path[currentIndex]) * node.GetParentWorld().Invert();
		currentTarget = world.Translation();

		// New target has been clicked
		if (newTarget)
		{
			currentIndex = 0;
			auto world = Matrix::CreateTranslation(path[currentIndex]) * node.GetParentWorld().Invert();
			currentTarget = world.Translation();
			hasTarget = true;
			newTarget = false;
			endOnTarget = false;
		}
		else if (endOnTarget)
		{
			return Steering::Arrive(position, endTarget, velocity, decel, maxSpeed);
		}
		else if (hasTarget)
		{
			auto distToCurrent = Vector3::DistanceSquared(position, currentTarget);
			auto distToEnd = Vector3::DistanceSquared(position, endTarget);
			// Next waypoint if close enough to currentTarget and there's a next
			if (currentIndex + 1 < path.size())
			{
				if (distToCurrent <= GetMinDistanceInPath(path))
				{
					auto world = Matrix::CreateTranslation(path[++currentIndex]) * node.GetParentWorld().Invert();
					currentTarget = world.Translation();
				}
			}
			else if (distToEnd < distToCurrent || currentTarget == endTarget)
			{
				endOnTarget = true;
				hasTarget = true;
				newTarget = false;
				return Vector3::Zero; //Steering::Arrive(position, endTarget, velocity, decel, maxSpeed);
			}
		}

		// Move to target
		return Steering::Seek(position, currentTarget, velocity, maxSpeed);
	}
	//-----------------------------------------------------------------------------------------------
	void Steering::MoveInHeadingDirection(float dt, Vector3& steering, Vector3& velocity
		, Vector3& position, float mass, float maxSpeed)
	{
		Vector3 force = steering;

		//Acceleration = Force/Mass
		Vector3 acceleration = force / mass;

		//Update velocity.
		velocity += acceleration * dt;

		//Finally, update the position.
		position += velocity * dt;
	}
	//------------------------------------------------------------------------------------------------//

	//--------------------------------------------------------------------------------------------------
	void AIComponent::SetTarget(const Vector3& t)
	{
		mPath = PathFinder::Find(mPosition, t, mWaypoints, mEdges);
		auto world = Matrix::CreateTranslation(t) * mNode.GetParentWorld().Invert();
		mTarget = world.Translation();
		mNewTarget = true;
		mHasTarget = true;
	}
	//--------------------------------------------------------------------------------------------------
	void AIComponent::Update(float dt)
	{
		dt = 0.002f;
		if (mPath.empty())
			return;

		switch (mBehaviour)
		{
		case Indecisive::AIComponent::NoBehaviour:
			break;
		case Indecisive::AIComponent::FollowBehaviour:
			mSteering = Steering::PathFollow(mPosition, mTarget, mHasTarget, mNewTarget, mEndOnTarget
				, mVelocity, mDeceleration, mMaxSpeed, mPath, mCurrentIndex, mNode);
			break;
		case Indecisive::AIComponent::SeekBehaviour:
			mSteering = Steering::Seek(mPosition, mTarget, mVelocity, mMaxSpeed);
			break;
		default:
			break;
		}
		Steering::MoveInHeadingDirection(dt, mSteering, mVelocity, mPosition, mMass, mMaxSpeed);
		if (mHasTarget) mWorld = Matrix::CreateTranslation(mPosition);
	}
	//------------------------------------------------------------------------------------------------//
}