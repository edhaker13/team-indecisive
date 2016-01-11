#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Indecisive
{		
	TEST_CLASS(UnitTest1)
	{
	private:
		IGraphics* _pGraphics;
	public:	
		TEST_CLASS_INITIALIZE(setup)
		{
			//TO DO
		}
		
		TEST_METHOD(LevelLoaderTest)
			{
				auto lvlLoader = LevelLoader();
				auto LV = LevelLoader();
				
				static const std::string level = "dummy.lvl";
				bool CanRead = lvlLoader.CanRead(level);

				auto root = static_cast<TreeNode*> (ResourceManagerInstance()->GetService("root"));
				
				Assert::IsTrue(CanRead);
			};

				TEST_METHOD(TextureLoadTest)
			{

				Window* pWindow = new Window(1280, 720, L"TEST"); 
				pWindow->Initialise(nullptr, 5);

				auto pGraphics = new GraphicsDirectX();
				
				ResourceManagerInstance()->AddService("graphics", pGraphics);
				ResourceManagerInstance()->AddService("root", new TreeNode("root"));
				ResourceManagerInstance()->AddService("camera", new CameraNode("cam", Vector3(0.f, 5.f, 100.f), Vector3::Zero, Vector3::Up, 0.0f, 100.0f));
				pGraphics->Initialise(pWindow);
				bool test = ResourceManagerInstance()->AddTexture("WhiteTex.dds");
				

				Assert::IsTrue(test);
				
			}

			TEST_METHOD(ObjectMeshTest)
			{

				auto lvlLoader = LevelLoader();
				static const std::string level = "dummy.lvl";
				lvlLoader.ReadWindow(level);
				lvlLoader.GetWindow()->Initialise(nullptr, 5);
				lvlLoader.Read(level);
				
				auto root = static_cast<TreeNode*> (ResourceManagerInstance()->GetService("root"));
				auto nodeObject = static_cast<ObjectNode*> (root->children["move"]->children["car"]);
				auto GO = &nodeObject->GetGameObject(); 

				Assert::IsNotNull(GO);

			}

		
		
	};
}