#pragma once
#include "..\TI_Engine\IReadable.h"
#pragma once
#ifdef TI_LOADER_EXPORTS
#	define LOADER_API __declspec(dllexport)
#else
#	define LOADER_API __declspec(dllimport)
#endif

namespace Indecisive
{
	class IGraphics;
	class Window;
	class PhysicsComp;
	class LevelLoader : public IReadable
	{
	private:
		// Win32 window class. Allocated after constructor, so no reference.
		Window* _pWindow = nullptr;
		// Graphics interface. Allocated after constructor, so no reference.
		IGraphics* _pGraphics = nullptr;
		//PhysicsComponent
		PhysicsComp* _pPhysics = nullptr;

	protected:
		LOADER_API void _Open(const std::string&, std::ifstream&) override;
	public:
		~LevelLoader() {};
		LevelLoader() {};
		LOADER_API void Read(const std::string&) override;
		LOADER_API bool CanRead(const std::string&) const override;
		LOADER_API IGraphics* GetGraphics() const;
		LOADER_API const std::string GetInfo() const override;
		LOADER_API Window* GetWindow() const;
		LOADER_API PhysicsComp* GetPhysics() const;
		LOADER_API void ReadWindow(const std::string&);
	};
};