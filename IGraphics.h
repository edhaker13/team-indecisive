#pragma once
typedef long HRESULT;

namespace Indecisive
{
	class IGraphics
	{
	public:
		virtual HRESULT Initialise(HINSTANCE hInstance, int nCmdShow) = 0;
	};
};