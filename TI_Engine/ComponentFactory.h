#pragma once
#include <string>
#include "LibraryAPI.h"
#include "IGameObject.h"
#include "Logger.h"

namespace Indecisive
{
	// Methods to create Game Objects
	namespace ComponentFactory
	{ // TODO: Rename to ObjectFactory

		LIBRARY_API IGameObject* MakeTestObject();
		LIBRARY_API IGameObject* MakeObjectFromObj(const std::string&);
	}
}