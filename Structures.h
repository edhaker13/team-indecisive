#pragma once
#include "DirectXMath.h"

namespace Indecisive
{
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
		Vector4 Colour;

		//------- LIGHTING TO BE DONE

		//Vector3 Normal;

		//-------
	};

	struct Geometry
	{
		Buffer* vertexBuffer;
		Buffer* indexBuffer;

		unsigned vertexBufferStride;
		unsigned vertexBufferOffset;
		unsigned indexBufferSize;
		unsigned indexBufferOffset;
	};

	struct Material
	{
		Vector4 diffuse;
		Vector4 ambient;
		//------- LIGHTING NOT YET IMPLEMENTED
		//Vector4 specular;
		//float specularPower;
		//-------
	};
}
