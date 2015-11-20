#pragma once
#include <windows.h>

namespace Indecisive
{
	struct Buffer;
	struct Geometry;
	struct SimpleVertex;
	struct Vertex;
	class Window;

	class IGraphics
	{
	public:
		virtual HRESULT Initialise(Window*) = 0;
		virtual Buffer* InitVertexBuffer(SimpleVertex[], unsigned) = 0;
		virtual Buffer* InitVertexBuffer(Vertex[], unsigned) = 0;
		virtual Buffer* InitIndexBuffer(unsigned short[], unsigned) = 0;
		virtual void DrawGeometry(Geometry*) = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
};