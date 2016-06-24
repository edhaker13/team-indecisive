#pragma once
#include <string>

namespace Indecisive
{
	// Basic component interface, type will be it's identifier
	class IComponent
	{
	private:
		std::string _type;
	public:
		IComponent(std::string type) : _type(type) {};
		const std::string& GetType() const { return _type; };
	};
}