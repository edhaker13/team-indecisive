#include "GameObject.h"
#include "LibraryAPI.h"
#include "MeshComponent.h"

namespace Indecisive
{
	namespace ComponentFactory
	{
		GameObject* MakeTestObject();
		LIBRARY_API GameObject* MakeTestObjectFromObj(std::string);
	}
}