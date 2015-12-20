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
		virtual bool CreateTextureFromFile(const wchar_t*, Texture**) const = 0;
		virtual bool Initialise(const Window*) = 0;
		virtual Buffer* InitVertexBuffer(const Vertex[], unsigned) const = 0;
		virtual Buffer* InitIndexBuffer(const unsigned short[], unsigned) const = 0;
		virtual void DrawMesh(const Mesh&, const SubObject&) const = 0;
		virtual void Update() = 0;
		virtual void Draw() const = 0;
	};
};