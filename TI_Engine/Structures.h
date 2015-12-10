#pragma once
#include "DirectXMath.h"

namespace Indecisive
{
	struct Buffer : public ID3D11Buffer {};
	struct Texture : public ID3D11ShaderResourceView {};

	struct Vertex
	{
		Vector3 Pos;
		Vector3 Normal;
		Vector2 TexC;

		bool operator<(const Vertex other) const
		{
			return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
		}
	};

	struct SimpleVertex
	{
		Vector3 Pos;
		Vector3 Normal;
		Vector2 TexC;

		bool operator<(const SimpleVertex other) const
		{
			return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
		}
	};

	struct Mesh
	{
		Buffer* vertexBuffer = nullptr;
		Buffer* indexBuffer = nullptr;
		UINT vertexBufferStride = 0;
		UINT vertexBufferOffset = 0;
		UINT indexBufferSize = 0;
		UINT indexBufferOffset = 0;
	};

	struct Material
	{
		std::string name = "";
		std::string ambientTextureName = "";
		std::string diffuseTextureName = "";
		std::string specularTextureName = "";
		float specularPower = 0.0f;
		float transparency = 0.0f;
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
	};

	struct SubObject
	{
		std::string material = "";
		std::string name = "";
		INT  vertexStart = 0; //probably don't need this since indices assume a single large vertex list
		UINT vertexEnd = 0;  //probably don't need this since indices assume a single large vertex list
		UINT indexStart = 0;
		UINT indexEnd = 0;
		UINT indexSize = 0;
		ID3D11ShaderResourceView* ambientTexture = nullptr;
		ID3D11ShaderResourceView* diffuseTexture = nullptr;
		ID3D11ShaderResourceView* specularTexture = nullptr;
	};
}
