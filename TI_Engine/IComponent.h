#pragma once
#include <string>

namespace Indecisive
{
	class IComponent
	{
	private:
		std::string _type;
	public:
		IComponent(std::string type) : _type(type) {};
		std::string GetType() { return _type; }
	};
}