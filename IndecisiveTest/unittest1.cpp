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
			TEST_METHOD(ObjectMeshTest)
			{
				/* WHAT AM I EVEN
				Window* pWindow = new Window(1280, 720, L"TEST");
				
				pWindow->Initialise(nullptr, 5);

				assert(pWindow != nullptr);
				
				auto pGraphics = new GraphicsDirectX();
				pGraphics->Initialise(pWindow);
				ResourceManagerInstance()->AddService("graphics", pGraphics);
				ResourceManagerInstance()->AddService("root", new TreeNode("root"));
				ResourceManagerInstance()->AddService("camera", new CameraNode("cam", Vector3(0.f, 5.f, 100.f), Vector3::Zero, Vector3::Up, 0.0f, 100.0f));
				IGameObject* GO = ComponentFactory::MakeObjectFromObj("fullcar.obj");*/
				auto lvlLoader = LevelLoader();
				static const std::string level = "dummy.lvl";
				lvlLoader.ReadWindow(level);
				lvlLoader.GetWindow()->Initialise(nullptr, 5);
				lvlLoader.Read(level);
				
				auto root = static_cast<TreeNode*> (ResourceManagerInstance()->GetService("root"));
				auto nodeObject = static_cast<ObjectNode*> (root->children["move"]->children["car"]);
				auto GO = &nodeObject->Object();

				//PROFIT???
				Assert::IsNotNull(GO);

				//Assert::IsNull(GO);

			}

			TEST_METHOD(TextureLoadTest)
			{
				/*Assert::IsTrue();*/
			}

			TEST_METHOD(LevelLoaderTest)
			{
				
			};

			
			
		
	};
}