#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IndecisiveTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(ObjectTest)
		{
			Assert::AreEqual(1, 1, L"test", LINE_INFO());
		}

	};
}