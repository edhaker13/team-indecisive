#pragma once
class IUpdatable
{
public:
	IUpdatable() {}
	~IUpdatable() {}

	virtual void Update() = 0;
};

