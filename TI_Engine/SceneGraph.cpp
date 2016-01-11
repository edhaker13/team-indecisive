#include "SceneGraph.h"
#include "IGameObject.h"

namespace Indecisive
{
	TreeNode::~TreeNode()
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

	void TreeNode::Append(TreeNode* node)
	{
		if (node != nullptr)
		{
			node->parent = this;
			children.emplace(node->key, node);
		}
	};

	const TreeNode* TreeNode::Find(const TreeNode* node, const std::string& key)
	{
		if (node != nullptr)
		{
			if (node->key == key)
			{
				return node;
			}
			const TreeNode* found = node->children.find(key)->second;
			if (found == node->children.end()->second)
			{
				for (const auto& p : node->children)
				{
					found = Find(p.second, key);
				}
			}
			return found;
		}
		return nullptr;
	};

	void TreeNode::Draw() const
	{
		for (const auto& pair : children)
		{
			(pair.second)->Draw();
		}
	};

	void TreeNode::Update(float elapsedTime)
	{
		for (const auto& pair : children)
		{
			(pair.second)->Update(elapsedTime);
		}
		//world *= GetParentWorld();
	};

	const Matrix TreeNode::GetWorld() const
	{
		return world;
	};

	const Matrix TreeNode::GetParentWorld() const
	{
		return parent != nullptr ? parent->GetWorld() : Matrix::Identity;
	};

	void ObjectNode::Draw() const
	{
		_object.Draw();
		TreeNode::Draw();
	};

	void ObjectNode::Update(float elapsedTime)
	{
		_object.Update(elapsedTime);
		world = _object.GetWorld();
		TreeNode::Update(elapsedTime);
	};

	void ClampedPositionNode::Update(float elapsedTime)
	{
		//if (position.z < lowerBounds.z && position.z > upperBounds.z)
		//{
		//	position.z = midPoints.z;
		//}
		world = Matrix::CreateTranslation(position);
		TreeNode::Update(elapsedTime);
	};

	void CameraNode::Update(float elapsedTime)
	{
		TreeNode::Update(elapsedTime);
	};
};