#include "LevelLoader.h"
#include "ComponentFactory.h"
#include "GraphicsDirectX.h"
#include "SceneGraph.h"
#include "ServiceLocator.h"
#include "Window.h"

namespace Indecisive
{
	const std::string LevelLoader::_PATH = ".\\/Levels\\/";

	void LevelLoader::_Open(const std::string& filename, std::ifstream& stream)
	{
		assert(!filename.empty());
		if (!stream.is_open())
		{
			stream.open(_PATH + filename);
			stream.exceptions(std::ifstream::failbit);
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

	const std::string LevelLoader::GetInfo() const
	{
		return "Reads a game level .lvl";
	}

	Window* LevelLoader::GetWindow(const std::string& filename)
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
			return _pWindow;
		}
		return nullptr;
	}

	IGraphics* LevelLoader::GetGraphics()
	{
		assert(_pGraphics != nullptr);
		return _pGraphics;
	}

	void LevelLoader::Read(const std::string& filename)
	{
		if (_pWindow == nullptr)
		{
			// TODO: Error Handling.
			throw std::runtime_error("Window needs to be initialised before Read()");
		}
		std::ifstream stream;
		_Open(filename, stream);
		bool initialised = false;
		std::string input = "";
		TreeNode* parent = new TreeNode("root");
		TreeNode* last = parent;
		Vector3 v, v1, v2;
		ServiceLocatorInstance()->Add("root", parent);

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
					ServiceLocatorInstance()->Add("graphics", _pGraphics);
				}
			}
			else if (input.compare("camera") == 0)
			{
				float nearZ, farZ;
				stream >> input;
				stream >> v;
				stream >> v1;
				stream >> v2;
				stream >> nearZ;
				stream >> farZ;
				// Initialize the camera node
				last = new CameraNode(input, v, v1, v2, nearZ, farZ);
				// Camera needs to be used in graphics initialise, so add to locator
				ServiceLocatorInstance()->Add(input, last);
				parent->Append(last);

				if (!initialised)
				{
					initialised = _pGraphics->Initialise(_pWindow);
				}
			}
			else if (input.compare("children") == 0)
			{
				// Set parent to last node
				parent = last;
			}
			else if (input.compare("position") == 0)
			{
				stream >> input;
				stream >> v;
				// Initialise translation node
				last = new PositionNode(input, v);
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