#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Indecisive
{		
	TEST_CLASS(UnitTest1)
	{
	public:

			TEST_METHOD(ObjectMeshTest)
			{
				auto go = ComponentFactory::MakeTestObjectFromObj("fullcar.obj");
	
				Assert::IsNotNull(go);
			}

			TEST_METHOD(TextureLoadTest)
			{
				auto ObjLoader = OBJLoader::OBJLoader();
				auto go = ComponentFactory::MakeTestObjectFromObj("fullcar.obj");
				
			}
		
	};
}