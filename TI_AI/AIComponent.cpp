#include <algorithm>
#include <iostream>
#include <sstream>
#include "..\TI_Engine\SceneGraph.h"
#include "AIComponent.h"

namespace Indecisive
{
	//--------------------------------------------------------------------------------------------------
	// Stream operators for quick, clean and reusable text output
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
		PositionList path;
		auto nearestToStart = GetNearest(start, waypoints);
		auto nearestToEnd = GetNearest(end, waypoints);

		if (nearestToStart == nearestToEnd || nearestToStart == nullptr || nearestToEnd == nullptr)
		{
			return path; // Early exit, no path can be found.
		}

		Node* current = nullptr;
		NodeList openNodes, closedNodes;
		openNodes.push_back(new Node(0, nullptr, nearestToStart));
		
		while (!openNodes.empty())
		{
			// Set current node to the next cheap node
			for (auto n : openNodes)
			{
				if (current == nullptr || n->cost < current->cost)
				{
					current = n;
				}
			}
			// Last waypoint to target, we have finished our search
			if (current->waypoint == nearestToEnd)
			{
				return ConstructPath(current, end);
			}
			// For each connected node calculate effective cost from current node
			for (auto id : current->waypoint->connectedIDs)
			{

				if (IsInList(openNodes, id) || IsInList(closedNodes, id))
				{
					// check new cost < current cost???
					continue;
				}
				float cost = current->cost;
				// TODO: Handle the possibilty of an invalid ID.
				auto connected = *(std::find_if(waypoints.cbegin(), waypoints.cend(),
					[id](Waypoint* n){ return n->id == id; })); // Short lambda to get the node from it's ID.
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
		// Root node be the one with out a parent, move up til found.
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
		// Appropiate edge cost should have the same IDs for the edges as the supplied waypoints
		auto it = std::find_if(edges.begin(), edges.end(), [from, to](const EdgeCost& e){
			return e.first.first == from->id && e.first.second == to->id
				|| e.first.first == to->id && e.second == from->id; });
		// Only return if the cost was found, otherwise return numeric maximum
		if (it != edges.cend())
		{
			return it->second;
		}
		return std::numeric_limits<float>::max(); //TODO: use cost type for max?
	}
	//--------------------------------------------------------------------------------------------------
	float PathFinder::GetEuclidianCost(const Vector3& positionOne, const Vector3& positionTwo)
	{
		// Distance can be squared as its only being used for comparison
		return Vector3::DistanceSquared(positionOne, positionTwo);
	}
	//--------------------------------------------------------------------------------------------------
	Waypoint* PathFinder::GetNearest(const Vector3& position, const WaypointList& waypoints)
	{
		Waypoint* nearest = nullptr;
		auto minDist = std::numeric_limits<float>::max(); //TODO: use waypoint type for max?
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

	//--------------------------------------------------------------------------------------------------
	Vector3 Steering::Arrive(const Vector3& position, const Vector3& target, const Vector3& velocity
		, float deceleration, float maxSpeed)
	{
		Vector3 vectorToTarget = target - position;
		// Calculate the distance to the target
		auto distance = vectorToTarget.Length();

		if (distance > 0)
		{
			float speed = distance / deceleration;
			speed = std::min(speed, maxSpeed);

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
		// Only flee if the target is within 'panic distance'. Work in distance squared space.
		static const float PanicDistanceSq = 100.0f * 100.0f;
		if (Vector3::DistanceSquared(position, target) > PanicDistanceSq)
		{
			return Vector3::Zero;
		}

		auto resultingVelocity = Vector3::Normalize(position - target) * maxSpeed;

		return (resultingVelocity - velocity);
	}
	//--------------------------------------------------------------------------------------------------
	Vector3 Steering::PathFollow(const Vector3& position, const Vector3& endTarget, bool& hasTarget
		, bool& newTarget, bool& endOnTarget, const Vector3& velocity, float decel, float maxSpeed
		, const PositionList& path, PositionList::size_type& currentIndex, const TreeNode& node)
	{
		Vector3 currentTarget;

		// No target set, don't do anything
		if (!hasTarget && !newTarget)
		{
			return Vector3::Zero;
		}

		assert(!path.empty());
		// Get current target in local space
		currentTarget = Vector3ToLocalSpace(path[currentIndex], node.GetParentWorld());

		// New target has been clicked
		if (newTarget)
		{
			if (currentIndex != 0)
			{
				currentIndex = 0;
				currentTarget = Vector3ToLocalSpace(path[currentIndex], node.GetParentWorld());
			}
			// Once a target has been set, apply the appropiate flags.
			hasTarget = true;
			newTarget = false;
			endOnTarget = false;
		}
		// Last destination is the target position, use arrive to not overshoot.
		else if (endOnTarget)
		{
			return Steering::Arrive(position, endTarget, velocity, decel, maxSpeed);
		}
		// Current destination is part of a path, move along the path if applicable.
		else if (hasTarget)
		{
			auto distToCurrent = Vector3::DistanceSquared(position, currentTarget);
			auto distToEnd = Vector3::DistanceSquared(position, endTarget);
			// Next waypoint if close enough to currentTarget and there's a next
			if (currentIndex + 1 < path.size())
			{
				if (distToCurrent <= GetMinDistanceInPath(path))
				{
					currentTarget = Vector3ToLocalSpace(path[++currentIndex], node.GetParentWorld());
				}
			}
			// End on target if closer to the target or if the destination is the target
			else if (distToEnd < distToCurrent || currentTarget == endTarget)
			{
				endOnTarget = true;
				hasTarget = true; // TODO:? using the word target too much
				newTarget = false;
				return Vector3::Zero; //Steering::Arrive(position, endTarget, velocity, decel, maxSpeed);
			}
		}

		// Move to target
		return Steering::Seek(position, currentTarget, velocity, maxSpeed);
	}
	//--------------------------------------------------------------------------------------------------
	void Steering::MoveInHeadingDirection(float dt, Vector3& steering, Vector3& velocity
		, Vector3& position, float mass, float maxSpeed)
	{
		// Force is usually steering + obstacle avoidance. The latter is not implemented here.
		Vector3 force = steering;

		// Acceleration = Force/Mass
		Vector3 acceleration = force / mass;

		// Update velocity.
		velocity += acceleration * dt;

		// Finally, update the position.
		position += velocity * dt;
	}
	//------------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------------
	float GetMinDistanceInPath(const PositionList& path)
	{
		// Using a static variable to reuse the result in subsequent calls
		static float result = 0;
		if (result == 0)
		{
			float minDist = std::numeric_limits<float>::max();
			Vector3 lastP = path[0]; //TODO: More descriptive names
			std::for_each(path.cbegin() + 1, path.cend(),
				[&minDist, &lastP](Vector3 v){
				auto d = Vector3::DistanceSquared(lastP, v);
				lastP = v;
				if (d < minDist) minDist = d / 4;
			});
			result = minDist;
		}
		return result;
	}
	//--------------------------------------------------------------------------------------------------
	Vector3 Vector3ToLocalSpace(const Vector3& v, const Matrix& parentWorld)
	{
		auto world = Matrix::CreateTranslation(v) * parentWorld.Invert();
		return world.Translation();
	}
	//------------------------------------------------------------------------------------------------//

	//--------------------------------------------------------------------------------------------------
	void AIComponent::SetTarget(const Vector3& t)
	{
		// Need to find a path from our position to the target
		mPath = PathFinder::Find(mPosition, t, mWaypoints, mEdges);
		// Set the target in local space
		mTarget = Vector3ToLocalSpace(t, mNode.GetParentWorld());
		// Set the appropiate flags
		mNewTarget = true;
		mHasTarget = true;
	}
	//--------------------------------------------------------------------------------------------------
	void AIComponent::Update(float dt)
	{
		// Generate steering force based on selected behaviour
		switch (mBehaviour)
		{
		case Indecisive::AIComponent::NoBehaviour:
			break;
		case Indecisive::AIComponent::FollowBehaviour:
			if (mPath.empty()) return; // Can't follow a path where there is none
			mSteering = Steering::PathFollow(mPosition, mTarget, mHasTarget, mNewTarget, mEndOnTarget
				, mVelocity, mDeceleration, mMaxSpeed, mPath, mCurrentIndex, mNode);
			break;
		case Indecisive::AIComponent::SeekBehaviour:
			mSteering = Steering::Seek(mPosition, mTarget, mVelocity, mMaxSpeed);
			break;
		default:
			break;
		}
		// Apply forces
		Steering::MoveInHeadingDirection(dt, mSteering, mVelocity, mPosition, mMass, mMaxSpeed);
		// Apply translation to use for the tree node
		if (mHasTarget) mWorld = Matrix::CreateTranslation(mPosition);
	}
	//------------------------------------------------------------------------------------------------//
}