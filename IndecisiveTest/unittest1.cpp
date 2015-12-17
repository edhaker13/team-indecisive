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

		
		//

			TEST_METHOD(ObjectMeshTest)
			{
			
				OBJLoader LoaderTest;
				std::string filename;
				std::string meshName;
				bool testCoords = false;

				Assert::IsNotNull(LoaderTest.ConstructFromMesh(meshName));

			
			}
		
	};
}