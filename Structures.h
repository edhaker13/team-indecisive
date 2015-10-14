#include "DirectXMath.h"

namespace Indecisive
{ 
	struct Geometry
	{
		Vertex* vertexBuffer;
		unsigned short indexBuffer;
		int numberOfIndices;

		unsigned int vertexBufferStride;
		unsigned int vertexBufferOffset;

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

	struct Vertex
	{
		Vector3 Pos;
		Vector3 Normal;
		Vector3 TexC;

		bool operator<(const Vertex other) const
		{
			return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
		}
	};

	struct SimpleVertex
	{
		Vector3 Pos;
		Vector4 Colour;
	};

		

}
