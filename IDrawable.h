#pragma once
class IDrawable
{
public:
	IDrawable() {}
	~IDrawable() {}

	virtual void Draw() = 0;
};

