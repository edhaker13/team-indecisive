#pragma once
#include <windows.h>

namespace Indecisive
{
	struct Buffer;
	struct Geometry;
	struct SimpleVertex;

	class IGraphics
	{
	public:
		virtual HRESULT Initialise(HINSTANCE, int) = 0;
		virtual Buffer* InitVertexBuffer(SimpleVertex[], unsigned) = 0;
		virtual Buffer* InitIndexBuffer(unsigned short[], unsigned) = 0;
		virtual void DrawGeometry(Geometry*) = 0;
	};
};