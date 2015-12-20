#include "LevelLoader.h"
#include "ComponentFactory.h"
#include "GraphicsDirectX.h"
#include "SceneGraph.h"
#include "ServiceLocator.h"
#include "Window.h"

namespace Indecisive
{
	const std::string LevelLoader::_PATH = ".\\Levels\\";

	void LevelLoader::_Open(const std::string& filename)
	{
		if (_stream.is_open() && _currentFilename != filename)
		{
			_stream.close();
		}
		if (!_stream.is_open())
		{
			_currentFilename = filename;
			_stream.open(_PATH + filename);
			_stream.exceptions(std::ifstream::failbit);
		}
	}

	void LevelLoader::_Close()
	{
		_stream.close();
		_currentFilename.clear();
	}

	bool LevelLoader::CanRead(const std::string& filename) const
	{
		return ExtensionCheck(filename, "lvl");
	}

	const std::string LevelLoader::GetInfo() const
	{
		return "Reads a game level.txt";
	}

	Window* LevelLoader::GetWindow(const std::string& filename)
	{
		_Open(filename);
		UINT width, height;
		std::string name;
		_stream >> name;
		if (name.compare("window") == 0)
		{
			name.clear();
			_stream >> width;
			_stream >> height;
			_stream.get(); std::getline(_stream, name);
			std::wstring w_name(name.begin(), name.end());
			_pWindow = new Window(width, height, w_name);
			return _pWindow;
		}
		return nullptr;
	}

	IGraphics* LevelLoader::GetGraphics()
	{
		return _pGraphics;
	}

	void LevelLoader::InternalRead(const std::string& filename)
	{
		if (_pWindow == nullptr)
		{
			// TODO: Error Handling.
			// Window needs to be created and initialised before hand.
			return;
		}

		_Open(filename);
		bool initialised = false;
		std::string input = "";
		TreeNode* parent = new TreeNode("root");
		TreeNode* last = parent;
		Vector3 v, v1, v2;
		ServiceLocatorInstance()->Add("root", parent);

		while (!_stream.eof())
		{
			_stream >> input;

			if (input.compare("graphics") == 0)
			{
				_stream >> input;
				if (input.compare("directx") == 0)
				{
					_pGraphics = new GraphicsDirectX();
					ServiceLocatorInstance()->Add("graphics", _pGraphics);
				}
			}
			else if (input.compare("camera") == 0)
			{
				float nearZ, farZ;
				_stream >> input;
				_stream >> v;
				_stream >> v1;
				_stream >> v2;
				_stream >> nearZ; _stream >> farZ;
				// Initialize the camera node
				last = new CameraNode(input, v, v1, v2, nearZ, farZ);
				// Camera needs to be used in graphics initialise, so add to locator
				ServiceLocatorInstance()->Add(input, last);
				parent->Append(last);

				if (!initialised)
				{
					initialised = _pGraphics->Initialise(_pWindow);
					// TODO: Error Handling
				}
			}
			else if (input.compare("children") == 0)
			{
				// Set parent to last node
				parent = last;
			}
			else if (input.compare("position") == 0)
			{
				_stream >> input;
				_stream >> v;
				// Initialise translation node
				last = new PositionNode(input, v);
				parent->Append(last);
			}
			else if (input.compare("object") == 0)
			{
				std::string obj;
				_stream >> input;
				_stream >> obj;
				// Initialise game objects (OBJLoader makes uses of graphics so it needs to be done after it)
				last = new ObjectNode(input, *ComponentFactory::MakeObjectFromObj(obj));
				parent->Append(last);
			}
			else if (input.compare("end") == 0 && parent->parent != nullptr)
			{
				parent = parent->parent;
			}
		}

		_Close();
		return;
	}
};