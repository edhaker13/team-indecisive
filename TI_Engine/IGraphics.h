#pragma once

namespace Indecisive
{
	struct Buffer;
	struct Mesh;
	struct SimpleVertex;
	struct SubObject;
	struct Texture;
	struct Vertex;
	class Window;

	class IGraphics
	{
	public:
		virtual bool CreateTextureFromFile(const wchar_t*, Texture**) = 0;
		virtual bool Initialise(Window*) = 0;
		virtual Buffer* InitVertexBuffer(SimpleVertex[], unsigned) = 0;
		virtual Buffer* InitVertexBuffer(Vertex[], unsigned) = 0;
		virtual Buffer* InitIndexBuffer(unsigned short[], unsigned) = 0;
		virtual void DrawMesh(Mesh&, SubObject&) = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
};