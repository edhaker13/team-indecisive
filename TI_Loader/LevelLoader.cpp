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
	// Reader expects files to be at this location
	const std::string LevelLoader::_PATH = "./Levels/";

	void LevelLoader::_Open(const std::string& filename, std::ifstream& stream)
	{
		assert(!filename.empty());
		if (!stream.is_open())
		{
			// Set fail bit so exceptions will throw on read errors
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
		// A lot of win32 interfaces depend of a window handle, so it needs be initialised separately
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
		// A lot of win32 interfaces depend of a window handle, so it needs be initialised before hand.
		if (_pWindow == nullptr)
		{
			TI_LOG_E("Window was not assigned before reading a level file.");
			throw std::runtime_error("Window needs to be initialised before Read()");
		}
		std::ifstream stream;
		_Open(filename, stream);
		// Set initial variables
		bool initialised = false;
		std::string input = "";
		TreeNode* parent = new TreeNode("root");
		TreeNode* last = parent;
		auto pSoundManager = new SoundManager();
		auto pInputManager = new InputManager();
		auto edgecosts = new EdgeMap();
		auto waypoints = new WaypointList();
		pSoundManager->Initialize(_pWindow->GetHWND());
		// Allocate initialised services to the resource manager
		ResourceManagerInstance()->AddService("edgecosts", edgecosts);
		ResourceManagerInstance()->AddService("waypoints", waypoints);
		ResourceManagerInstance()->AddService("root", parent);
		ResourceManagerInstance()->AddService("sound", pSoundManager);
		ResourceManagerInstance()->AddService("input", pInputManager);

		while (!stream.eof())
		{
			assert(parent != nullptr);
			stream >> input;
			// Specifies which graphics interface to use, only directx is implemented atm
			if (input.compare("graphics") == 0)
			{
				stream >> input;
				if (input.compare("directx") == 0)
				{
					_pGraphics = new GraphicsDirectX();
					ResourceManagerInstance()->AddService("graphics", _pGraphics);
				}
			}
			// Specifies the number of waypoints, then lists them
			// Waypoints are ID(CHAR) Position(VECTOR3) Connections(comma separated ID list)
			else if (input.compare("waypoints") == 0)
			{
				WaypointList::size_type size;
				stream >> size;
				for (WaypointList::size_type i = 0; i < size; i++)
				{
					auto w = new Waypoint();
					stream >> *w; // Waypoint implements its own read function.
					waypoints->push_back(w);
				}
			}
			// Specifies the number of edges, then list them
			// Edges are ID(CHAR) ID(CHAR) Cost(FLOAT)
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
			// Camera is Name(STRING) Eye(VECTOR3) Center(VECTOR3) UP(VECTOR3) NEAR(FLOAT) FAR(FLOAT)
			// Optionally you can specify movement as a key and an amount to move in the direction
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
					else if (input.compare("actions") == 0)	cam = new CameraNode(name, eye, center, up, nearZ, farZ); // TODO: put in an start end loop
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
				// A camera needs to be used in the graphics initialisation, so add to resource manager
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
			// Sounds can be played at start and played by a key
			else if (input.compare("sound") == 0)
			{
				if (pSoundManager->IsInitialised())
				{
					std::string name;
					char key = 0;
					DWORD flag;
					bool playAtStart = false;
					while (input.compare("endsound") != 0)
					{
						stream >> input;
						if (input.compare("filename") == 0)			stream >> name;
						else if (input.compare("playstart") == 0)	playAtStart = true;
						else if (input.compare("playkey") == 0)		stream >> key;
						else if (input.compare("flag") == 0)		stream >> flag;
					}
					pSoundManager->Load(name);
					if (playAtStart)
					{
						pSoundManager->Play(name, 0, 0, flag);
					}
					if (key != 0)
					{
						pInputManager->RegisterAction((KeyCode)key, [pSoundManager, name, flag]() { pSoundManager->Play(name, 0, 0, flag); });
					}
				}
			}
			// Start a new tree level
			else if (input.compare("children") == 0)
			{
				
				parent = last;
			}
			// Set a new translation node
			else if (input.compare("position") == 0)
			{
				Vector3 pos;
				stream >> input;
				stream >> pos;
				// Initialise translation node
				last = new PositionNode(input, pos);
				parent->Append(last);
			}
			// Creates a game object from an obj file
			else if (input.compare("object") == 0)
			{
				// object creation makes use of the graphics interface so it needs to be initialised before hand
				if (!initialised)
				{
					auto ex = std::runtime_error("Graphics interface must be initialised before loading objects.");
					TI_LOG_EXCEPTION(".", ex);
					throw ex;
				}
				std::string obj;
				stream >> input;
				stream >> obj;
				last = new ObjectNode(input, *ComponentFactory::MakeObjectFromObj(obj));
				parent->Append(last);
			}
			// Creates an engine component
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

						o->GetGameObject().AddUpdatable(PhysComp);

					}

				}
			
			}
			// Ends the current tree hierarchy, if top level it will reset failbit so it can close.
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