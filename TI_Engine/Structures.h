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
		Buffer* vertexBuffer;
		Buffer* indexBuffer;

		UINT vertexBufferStride;
		UINT vertexBufferOffset;
		UINT indexBufferSize;
		UINT indexBufferOffset;
	};

	struct Material
	{
		std::string name;
		std::string ambientTexture;
		std::string diffuseTexture;
		std::string specularTexture;
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		float specularPower;
		float transparency;
	};

	struct SubObject
	{
		std::string id;
		std::string useMtl;
		INT _vertexStartLocation;//probably dont need this since indices assume a single large vertex list
		UINT _vertexEndLocation;//same goes for this
		UINT _indexStartLocation;
		UINT _indexEndLocation;
	};
}
