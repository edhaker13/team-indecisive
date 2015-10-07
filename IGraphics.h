#pragma once
typedef long HRESULT;
// HINSTANCE;

class IGraphics
{
public:
	IGraphics() {};
	~IGraphics() {};

	virtual HRESULT Initialise(HINSTANCE hInstance, int nCmdShow) = 0;
};

