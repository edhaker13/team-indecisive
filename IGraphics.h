#pragma once
class IGraphics
{
public:
	IGraphics();
	~IGraphics();

	virtual void Initialise() = 0;

	virtual void InitWindow() = 0;

	virtual void InitDevice() = 0;
};

