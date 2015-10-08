#pragma once
#include <list>
#include "DirectXMath.h"
namespace Indecisive
{
	/// <summary> Self contained n-ary tree. Expects a Matrix </summary> Polymorphism and recursion idea from http://www.andresilaghi.com/curious-projects/opengl-scene-graph-in-c/
	struct BaseNode
	{
		// TODO: Change to map
		std::list<BaseNode*> children;
		std::string key;
		BaseNode* parent = nullptr;
		Matrix world = Matrix::Identity;

		BaseNode() {};
		BaseNode(std::string key) : key(key), parent(nullptr), world(M::Identity) {};
		~BaseNode(void)
		{
			std::list<BaseNode*>::iterator it;
			for (it = children.begin(); it != children.end();)
			{
				delete *it;
				it = children.erase(it);
			}
			if (parent != nullptr)
			{
				parent->children.remove(this);
			}
		};

		void Append(BaseNode* appendix)
		{
			if (appendix != nullptr)
			{
				children.push_back(appendix);
				appendix->parent = this;
			}
		};

		/// <summary> Searches in deepest children first </summary>
		static BaseNode* Find(BaseNode* node, const std::string& value)
		{
			if (node != nullptr)
			{
				if (node->key == value)
				{
					return node;
				}
				BaseNode* found = nullptr;
				std::list<BaseNode*>::iterator it;
				for (it = node->children.begin(); it != node->children.end(); it++)
				{
					if (*it != nullptr && found == nullptr)
					{
						found = Find(*it, value);
					}
				}
				return found;
			}
			return nullptr;
		};

		static BaseNode* Insert(BaseNode* beforeNode, BaseNode* node)
		{
			if (node != nullptr)
			{
				bool insert = true;
				std::list<BaseNode*>::iterator it;
				for (it = node->children.begin(); it != node->children.end(); it++)
				{
					if (*it == node)
					{
						insert = false;
					}
				}

				if (insert)
				{
					node->children.push_back(beforeNode);
					beforeNode->parent = node;
				}

				return node;
			}
			return nullptr;
		};

		/// <summary> Recursively draw on children </summary>
		virtual void Draw()
		{
			std::list<BaseNode*>::iterator it;
			for (it = children.begin(); it != children.end(); it++)
			{
				if ((*it) != nullptr)
				{
					(*it)->Draw();
				}
			}
		};

		/// <summary> Recursively update children </summary>
		virtual void Update(int elapsedTime)
		{
			std::list<BaseNode*>::iterator it;
			for (it = children.begin(); it != children.end(); it++)
			{
				if ((*it) != nullptr)
				{
					(*it)->Update(elapsedTime);
				}
			}
		};

		/// <summary> Recursively check collision on children </summary>
		//virtual void Selection(std::vector<ObjectNode*>& hits, const float3& rStart, const float3& rEnd);

		// <summary> Return this node's transformation matrix </summary>
		virtual Matrix GetWorld() { return world; };

		// <summary> Return parent node's transformation matrix </summary>
		virtual Matrix GetParentWorld()
		{
			if (parent != nullptr)
			{
				return parent->GetWorld();
			}
			return Matrix::Identity;
		}
	};

	//struct ObjectNode : public BaseNode
	//{
	//	GameObject* object;
	//
	//	ObjectNode() : object(nullptr) {};
	//	ObjectNode(std::string key, GameObject* object) : BaseNode(key), object(object) {};
	//	~ObjectNode() {	delete object; object = nullptr; };
	//	//virtual void Draw() { object->Draw(); BaseNode::Draw(); }; // Needs adding arguments
	//	virtual void Update(int elapsedTime) { object->Update(elapsedTime);	BaseNode::Update(elapsedTime); };
	//	//virtual void Selection(std::vector<ObjectNode*>& hits, const float3& rStart, const float3& rEnd);
	//};

	struct RotationNode : public BaseNode
	{
		Vector3 yawPitchRoll;

		RotationNode(std::string key, Vector3 yawPitchRoll) : BaseNode(key), yawPitchRoll(yawPitchRoll) {};

		// Needs implementation after VectorMath
		virtual void Draw()
		{
			Matrix Transform = Matrix::CreateFromYawPitchRoll(yawPitchRoll.x, yawPitchRoll.y, yawPitchRoll.z);
			world = Transform * GetParentWorld();
			BaseNode::Draw();
		};
	};

	struct PositionNode : public BaseNode
	{
		Vector3 position;
		PositionNode(std::string key, Vector3 position) : BaseNode(key), position(position) {};

		// Needs implementation after VectorMath
		virtual void Draw()
		{
			//World = XMMatrixTranslation(position.x, position.y, position.z) * GetParentWorld();
			BaseNode::Draw();
		};
	};
	/// <summary> Node with position data that will stay with it the offsets of cameraEye </summary>
	struct ClampedPositionNode : public PositionNode
	{
		const Vector3& cameraEye;
		float lowerBound, upperBound;
		ClampedPositionNode(std::string key, Vector3 position, const Vector3& cameraEye, float lowerBound, float upperBound) :
			PositionNode(key, position), cameraEye(cameraEye), lowerBound(lowerBound), upperBound(upperBound) {};

		virtual void Update(int elapsedTime)
		{
			if (position.z < lowerBound && position.z > upperBound)
			{
				position.z = (upperBound - lowerBound) / 2;
			}
			PositionNode::Update(elapsedTime);
		}
	};

	/// <summary> Node with camera center, eye, and up positions </summary>
	struct CameraNode : public BaseNode
	{
		Vector3 eye, up, distance;

		///<summary> Reference to position to have in the center of the camera</summary>
		Vector3& center;

		CameraNode(std::string key, Vector3& center, Vector3 eye, Vector3 up) :
			BaseNode(key), center(center), eye(eye), up(up)
		{
			distance = center - eye;
		};

		/// <summary> Update eye position if the center has moved </summary>
		void UpdateEye()
		{
			if (center + distance != eye)
			{
				eye = center - distance;
			}
		};

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
	};

};