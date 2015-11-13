#include "MeshComponent.h"
#include "GameObject.h"

namespace Indecisive
{
	namespace ComponentFactory
	{
		GameObject* MakeTestObject();
		GameObject* MakeTestObjectFromObj(std::string);
	}
}