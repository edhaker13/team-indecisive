#pragma once
typedef long HRESULT;

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
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
};