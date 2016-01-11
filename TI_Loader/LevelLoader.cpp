#include "LevelLoader.h"
#include "..\TI_AI\AIComponent.h"
#include "..\TI_Engine\ComponentFactory.h"
#include "..\TI_Engine\GraphicsDirectX.h"
#include "..\TI_Engine\SceneGraph.h"
#include "..\TI_Engine\IResourceManager.h"
#include "..\TI_Engine\Window.h"
#include "..\TI_Physics\TransformComponent.h"
#include "..\TI_Physics\PhysicsComp.h"
#include "..\TI_Sound\SoundManager.h"
#include "..\TI_Input\InputManager.h"

namespace Indecisive
{
	const std::string LevelLoader::_PATH = ".\\/Levels\\/";

	void LevelLoader::_Open(const std::string& filename, std::ifstream& stream)
	{
		assert(!filename.empty());
		if (!stream.is_open())
		{
			stream.exceptions(std::ifstream::failbit);
			stream.open(_PATH + filename);
		}
	}

	bool LevelLoader::CanRead(const std::string& filename) const
	{
		assert(!filename.empty());
		std::string::size_type pos = filename.find_last_of('.');

		// No file extension - can't read
		if (pos == std::string::npos)
			return false;
		std::string ext = filename.substr(pos + 1);

		return ext.compare("lvl") == 0;
	}

	void LevelLoader::ReadWindow(const std::string& filename)
	{
		std::ifstream stream;
		UINT width, height;
		std::string name;

		_Open(filename, stream);
		stream >> name;
		if (name.compare("window") == 0)
		{
			name.clear();
			stream >> width;
			stream >> height;
			stream.get(); std::getline(stream, name);
			std::wstring w_name(name.begin(), name.end());
			_pWindow = new Window(width, height, w_name);
			assert(_pWindow != nullptr);
		}
	}

	IGraphics* LevelLoader::GetGraphics() const
	{
		assert(_pGraphics != nullptr);
		return _pGraphics;
	}

	const std::string LevelLoader::GetInfo() const
	{
		return "Reads a game level .lvl";
	}

	Window* LevelLoader::GetWindow() const
	{
		assert(_pWindow != nullptr);
		return _pWindow;
	}

	void LevelLoader::Read(const std::string& filename)
	{
		if (_pWindow == nullptr)
		{
			TI_LOG_E("Window was not assigned before reading a level file.");
			throw std::runtime_error("Window needs to be initialised before Read()");
		}
		std::ifstream stream;
		_Open(filename, stream);
		bool initialised = false;
		std::string input = "";
		TreeNode* parent = new TreeNode("root");
		TreeNode* last = parent;
		auto pSoundManager = new SoundManager();
		auto pInputManager = new InputManager();
		auto edgecosts = new EdgeMap();
		auto waypoints = new WaypointList();
		pSoundManager->Initialize(_pWindow->GetHWND());
		ResourceManagerInstance()->AddService("edgecosts", edgecosts);
		ResourceManagerInstance()->AddService("waypoints", waypoints);
		ResourceManagerInstance()->AddService("root", parent);
		ResourceManagerInstance()->AddService("sound", pSoundManager);
		ResourceManagerInstance()->AddService("input", pInputManager);

		while (!stream.eof())
		{
			assert(parent != nullptr);
			stream >> input;

			if (input.compare("graphics") == 0)
			{
				stream >> input;
				if (input.compare("directx") == 0)
				{
					_pGraphics = new GraphicsDirectX();
					ResourceManagerInstance()->AddService("graphics", _pGraphics);
				}
			}
			else if (input.compare("waypoints") == 0)
			{
				WaypointList::size_type size;
				stream >> size;
				for (WaypointList::size_type i = 0; i < size; i++)
				{
					auto w = new Waypoint();
					stream >> *w;
					waypoints->push_back(w);
				}
			}
			else if (input.compare("edgecosts") == 0)
			{
				EdgeMap::size_type size;
				stream >> size;
				for (EdgeMap::size_type i = 0; i < size; i++)
				{
					Edge edge; float cost;
					stream >> edge.first;
					stream >> edge.second;
					stream >> cost;
					edgecosts->emplace(edge, cost);
				}
			}
			else if (input.compare("camera") == 0)
			{
				std::string name; Vector3 eye, center, up; float nearZ, farZ; char key; float amount;
				CameraNode* cam = nullptr;
				while (input.compare("endcamera") != 0)
				{
					stream >> input;
					if (input.compare("name") == 0)			stream >> name;
					else if (input.compare("eye") == 0)		stream >> eye;
					else if (input.compare("center") == 0)	stream >> center;
					else if (input.compare("up") == 0)		stream >> up;
					else if (input.compare("near") == 0)	stream >> nearZ;
					else if (input.compare("far") == 0)		stream >> farZ;
					else if (input.compare("actions") == 0)	cam = new CameraNode(name, eye, center, up, nearZ, farZ);
					else if (input.compare("moveright") == 0)
					{
						stream >> key;
						stream >> amount;
						pInputManager->RegisterAction((KeyCode)key, [cam, amount](){ cam->eye.x += amount; });
					}
					else if (input.compare("moveleft") == 0)
					{
						stream >> key;
						stream >> amount;
						pInputManager->RegisterAction((KeyCode)key, [cam, amount](){ cam->eye.x -= amount; });
					}
					else if (input.compare("moveforwards") == 0)
					{
						stream >> key;
						stream >> amount;
						pInputManager->RegisterAction((KeyCode)key, [cam, amount](){ cam->eye.z += amount; });
					}
					else if (input.compare("movebackwards") == 0)
					{
						stream >> key;
						stream >> amount;
						pInputManager->RegisterAction((KeyCode)key, [cam, amount](){ cam->eye.z -= amount; });
					}
				}
				if (cam == nullptr) TI_LOG_E("Couldn't make a camera node");
				// A camera needs to be used in graphics initialise, so add to locator
				ResourceManagerInstance()->AddService(name, cam);
				// Add node to tree, set last node as this
				parent->Append(cam);
				last = cam;
				// Now that we have a camera initialise graphics if not already done
				if (!initialised)
				{
					initialised = _pGraphics->Initialise(_pWindow);
				}
			}
			else if (input.compare("sound") == 0)
			{
				if (pSoundManager->IsInitialised())
				{
					stream >> input;
					pSoundManager->Load(input);
					DWORD flag;
					stream >> flag;
					pSoundManager->Play(input, 0, 0, flag);
				}
			}
			else if (input.compare("children") == 0)
			{
				// Set parent to last node
				parent = last;
			}
			else if (input.compare("position") == 0)
			{
				Vector3 pos;
				stream >> input;
				stream >> pos;
				// Initialise translation node
				last = new PositionNode(input, pos);
				parent->Append(last);
			}
			else if (input.compare("object") == 0)
			{
				std::string obj;
				stream >> input;
				stream >> obj;
				// Initialise game objects (OBJLoader makes uses of graphics so it needs to be done after it)
				last = new ObjectNode(input, *ComponentFactory::MakeObjectFromObj(obj));
				parent->Append(last);
			}
			else if (input.compare("comp") == 0)
			{
				if (ObjectNode* o = dynamic_cast<ObjectNode*>(last))
				{
					stream >> input;
					if (input.compare("AI") == 0)
					{
						assert(!waypoints->empty());
						assert(!edgecosts->empty());
						float decel, mass, speed;
						stream >> decel; stream >> mass; stream >> speed;
						auto aiComp = new AIComponent(*last, *waypoints, *edgecosts, decel, mass, speed);
						aiComp->SetTarget(waypoints->back()->position);
						o->GetGameObject().AddUpdatable(aiComp);
					}
					else if (input.compare("Physics") == 0)
					{
						TransformComponent* Basetransform = new TransformComponent();
						Basetransform->SetScale(10000.0f, 10000.0f, 10000.0f);
						Basetransform->SetPosition(0.0f, 0.0f, 0.0f);
						PhysicsComp* FloorModel = new PhysicsComp(Basetransform);

						Vector3 scale, position, rotation, velocity;
						float mass, radius, theta, frCoef;
						stream >> scale; stream >> position; stream >> rotation; stream >> radius; stream >> velocity; stream >> mass;
						stream >> theta; stream >> frCoef;

						TransformComponent* ObjectTransform = new TransformComponent();
						ObjectTransform->SetScale(scale);
						ObjectTransform->SetPosition(position);
						ObjectTransform->SetRotation(rotation);

						PhysicsComp* PhysComp = new PhysicsComp(ObjectTransform);

						PhysComp->SetCollisionRadius(radius);
						PhysComp->SetVel(velocity);
						PhysComp->SetMass(mass);
						PhysComp->SlidingForce(theta, frCoef);

						PhysComp->FloorCollisionCheck(Basetransform->GetPosition()); 

						o->GetGameObject().AddUpdatable(PhysComp);

					}

				}
			
			}

			else if (input.compare("end") == 0)
			{
				if (parent->parent != nullptr)
				{
					parent = parent->parent;
				}
				else
				{
					stream.exceptions(std::ifstream::badbit);
				}
			}
		}

		stream.close();
	}
};