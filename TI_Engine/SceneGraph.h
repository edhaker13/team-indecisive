#pragma once
#include <map>
#include "DirectXMath.h"
#include "IGameObject.h"

namespace Indecisive
{
	/// <summary> Self contained n-ary tree. Expects a Matrix </summary> Polymorphism and recursion idea from http://www.andresilaghi.com/curious-projects/opengl-scene-graph-in-c/
	struct TreeNode
	{
		std::map<std::string, TreeNode*> children;
		std::string key;
		TreeNode* parent = nullptr;
		Matrix world = Matrix::Identity;

		TreeNode(): parent(nullptr) {};
		TreeNode(std::string key) : key(key), parent(nullptr) {};
		~TreeNode(void)
		{
			for (auto it = children.begin(); it != children.end();)
			{
				delete it->second;
				it = children.erase(it);
			}
			if (parent != nullptr)
			{
				delete parent;
				parent = nullptr;
			}
		};

		void Append(TreeNode* appendix)
		{
			if (appendix != nullptr)
			{
				children.emplace(appendix->key, appendix);
				appendix->parent = this;
			}
		};

		/// <summary> Searches in all siblings first </summary>
		static TreeNode* Find(TreeNode* node, const std::string& key)
		{
			if (node != nullptr)
			{
				if (node->key == key)
				{
					return node;
				}
				TreeNode* found = node->children.find(key)->second;
				if (found == node->children.end()->second)
				{
					for (const auto& p: node->children)
					{
						found = Find(p.second, key);
					}				
				}
				return found;
			}
			return nullptr;
		};

		/// <summary> Recursively draw children </summary>
		virtual void Draw()
		{
			for (const auto& pair: children)
			{
				(pair.second)->Draw();
			}
		};

		/// <summary> Recursively update children </summary>
		virtual void Update(float elapsedTime)
		{
			for(const auto& pair: children)
			{
				(pair.second)->Update(elapsedTime);
			}
			world *= GetParentWorld();
		};

		/// <summary> Recursively check collision on children </summary>
		//virtual void Selection(std::vector<ObjectNode*>& hits, const float3& rStart, const float3& rEnd);

		// <summary> Return this node's transformation matrix </summary>
		virtual const Matrix GetWorld() { return world; };

		// <summary> Return parent node's transformation matrix </summary>
		virtual const Matrix GetParentWorld()
		{
			return parent != nullptr ? parent->GetWorld() : Matrix::Identity;
		}
	};

	struct ObjectNode : public TreeNode
	{
	private:
		IGameObject& _object;
	public:
		ObjectNode(std::string key, IGameObject& object) : TreeNode(key), _object(object) {};
		virtual void Draw() override { _object.Draw(); TreeNode::Draw(); };
		virtual void Update(float elapsedTime) override
		{
			_object.Update(elapsedTime);
			world = _object.GetWorld();
			TreeNode::Update(elapsedTime); 
		};
		//virtual void Selection(std::vector<ObjectNode*>& hits, const float3& rStart, const float3& rEnd);
	};

	struct RotationNode : public TreeNode
	{
		float yaw, pitch, roll;

		RotationNode(std::string key, float yaw, float pitch, float roll) : TreeNode(key), yaw(yaw), pitch(pitch), roll(roll) {};

		virtual void Update(float elapsedTime) override
		{
			world = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
			TreeNode::Update(elapsedTime);
		};
	};

	struct PositionNode : public TreeNode
	{
		Vector3 position;
		PositionNode(std::string key, const Vector3& position) : TreeNode(key), position(position) {};

		virtual void Update(float elapsedTime) override
		{
			world = Matrix::CreateTranslation(position);
			TreeNode::Update(elapsedTime);
		};
	};
	/// <summary> Node with position that will stay within the offsets of cameraEye </summary>
	struct ClampedPositionNode : public PositionNode
	{
		const Vector3& cameraEye;
		float lowerBound, upperBound;
		ClampedPositionNode(std::string key, const Vector3& position, const Vector3& cameraEye, float lowerBound, float upperBound) :
			PositionNode(key, position), cameraEye(cameraEye), lowerBound(lowerBound), upperBound(upperBound) {};

		virtual void Update(float elapsedTime) override
		{
			if (position.z < lowerBound && position.z > upperBound)
			{
				position.z = (upperBound - lowerBound) / 2;
			}
			TreeNode::Update(elapsedTime);
		}
	};

	/// <summary> Node with camera center, eye, and up positions </summary>
	struct CameraNode : public TreeNode
	{
		Vector3 eye, up, distance;
		float nearZ, farZ;

		///<summary> Reference to position to follow as the center of the camera</summary>
		Vector3& center;

		CameraNode(std::string key, Vector3& center, const Vector3& eye, const Vector3& up, float nearZ, float farZ) :
			TreeNode(key), center(center), eye(eye), up(up), distance(center - eye), nearZ(nearZ), farZ(farZ) {};

		/// <summary> Loop around eye x and y if outside limits </summary>
		void ClampEye()
		{
			const float limit = 20.0f;
			if (eye.x > limit)
			{
				eye.x = (eye.x - limit) * -1.0f;
				distance.x = eye.x - 1.0f;
			}
			else if (eye.x < -limit)
			{
				eye.x = (eye.x + limit) * -1.0f;
				distance.x = eye.x + 1.0f;
			}

			if (eye.y > limit)
			{
				eye.y = (eye.y - limit) * -1.0f;
				distance.y = eye.y - 1.0f;
			}
			else if (eye.y < -limit)
			{
				eye.y = (eye.y + limit) * -1.0f;
				distance.y = eye.y + 1.0f;
			}
		};

		/// <summary> Update eye position if the center has moved </summary>
		virtual void Update(float elapsedTime) override
		{
			if (center + distance != eye)
			{
				eye = center - distance;
			}
			ClampEye();
			TreeNode::Update(elapsedTime);
		};
	};

};