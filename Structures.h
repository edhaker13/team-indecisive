#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>
#include "DirectXMath.h"

using namespace Indecisive;

struct Geometry
{
	SimpleVertex* vertexBuffer;
	unsigned short indexBuffer;
	int numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
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

struct SimpleVertex
{
	Vector3 Pos;
	Vector3 Normal;
	Vector3 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	}
};