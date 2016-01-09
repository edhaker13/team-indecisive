#pragma once
#include <string>
#include "LibraryAPI.h"
#include "IGameObject.h"
#include "Logger.h"

namespace Indecisive
{
	namespace ComponentFactory
	{
		LIBRARY_API IGameObject* MakeTestObject();
		LIBRARY_API IGameObject* MakeObjectFromObj(const std::string&);
	}
}