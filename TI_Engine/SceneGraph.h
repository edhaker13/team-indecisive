#pragma once
#include <map>
#include "DirectXMath.h"

namespace Indecisive
{
	class IGameObject;
	/// <summary> Self contained n-ary tree. Expects a Matrix </summary> Polymorphism and recursion idea from http://www.andresilaghi.com/curious-projects/opengl-scene-graph-in-c/
	struct TreeNode
	{
		std::map<std::string, TreeNode*> children;
		std::string key;
		TreeNode* parent = nullptr;
		Matrix world = Matrix::Identity;
		TreeNode(const std::string& key) : key(key), parent(nullptr) {};
		~TreeNode();
		/// <summary> Adds the node as a child </summary>
		LIBRARY_API void Append(TreeNode*);
		/// <summary> Searches in all siblings first </summary>
		static const TreeNode* Find(const TreeNode*, const std::string&);
		/// <summary> Recursively draw children </summary>
		LIBRARY_API virtual void Draw();
		/// <summary> Recursively update children </summary>
		LIBRARY_API virtual void Update(float);
		/// <summary> Return this node's transformation matrix </summary>
		LIBRARY_API virtual const Matrix GetWorld();
		/// <summary> Return parent node's transformation matrix </summary>
		LIBRARY_API virtual const Matrix GetParentWorld();
	};
	/// <summary> Node containing a game object to use </summary>
	struct ObjectNode : public TreeNode
	{
	private:
		IGameObject& _object;
	public:
		LIBRARY_API ObjectNode(const std::string& key, IGameObject& object) : TreeNode(key), _object(object) {};
		LIBRARY_API virtual void Draw() override;
		LIBRARY_API virtual void Update(float) override;
	};
	/// <summary> Node with a rotation to apply to all child objects </summary>
	struct RotationNode : public TreeNode
	{
		const float yaw, pitch, roll;
		LIBRARY_API RotationNode(const std::string& key, float yaw, float pitch, float roll) : TreeNode(key), yaw(yaw), pitch(pitch), roll(roll) {};
		LIBRARY_API virtual void Update(float) override;
	};
	/// <summary> Node with position to translate all child objects </summary>
	struct PositionNode : public TreeNode
	{
		Vector3 position;
		LIBRARY_API PositionNode(const std::string& key, const Vector3& position) : TreeNode(key), position(position) {};
		LIBRARY_API virtual void Update(float) override;
	};
	/// <summary> Node with position that will stay within the offsets of cameraEye </summary>
	struct ClampedPositionNode : public PositionNode
	{
		const Vector3& cameraEye;
		const float lowerBound, upperBound;
		LIBRARY_API ClampedPositionNode(const std::string& key, const Vector3& position, const Vector3& cameraEye, float lowerBound, float upperBound) :
			PositionNode(key, position), cameraEye(cameraEye), lowerBound(lowerBound), upperBound(upperBound) {};
		LIBRARY_API virtual void Update(float) override;
	};
	/// <summary> Node with camera eye, center, and up positions. Will follow center position </summary>
	struct CameraNode : public TreeNode
	{
		///<summary> Position to follow as the center of the camera </summary>
		const Vector3& center;
		const Vector3 up, distance;
		Vector3 eye;
		const float nearZ, farZ;
		LIBRARY_API CameraNode(std::string key, const Vector3& eye, const Vector3& center, const Vector3& up, float nearZ, float farZ) :
			TreeNode(key), eye(eye), center(center), up(up), distance(center - eye), nearZ(nearZ), farZ(farZ) {};
		/// <summary> Update eye position if the center has moved </summary>
		LIBRARY_API virtual void Update(float) override;
	};
};