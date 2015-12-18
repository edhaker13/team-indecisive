#include "GameObject.h"
#include "LibraryAPI.h"
#include "MeshComponent.h"

namespace Indecisive
{
	namespace ComponentFactory
	{
		LIBRARY_API GameObject* MakeTestObject();
		LIBRARY_API GameObject* MakeObjectFromObj(std::string);
	}
}