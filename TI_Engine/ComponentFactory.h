#include <string>
#include "LibraryAPI.h"
#include "IGameObject.h"

namespace Indecisive
{
	namespace ComponentFactory
	{
		LIBRARY_API IGameObject* MakeTestObject();
		LIBRARY_API IGameObject* MakeObjectFromObj(std::string);
	}
}