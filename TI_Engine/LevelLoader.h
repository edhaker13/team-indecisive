#pragma once
#include "IReadable.h"
#include "LibraryAPI.h"

namespace Indecisive
{
	class IGraphics;
	class Window;
	class LevelLoader : public IReadable
	{
	private:
		// Win32 window class. Allocated after constructor, so no reference.
		Window* _pWindow = nullptr;
		// Graphics interface. Allocated after constructor, so no reference.
		IGraphics* _pGraphics = nullptr;
	protected:
		LIBRARY_API void _Open(const std::string&, std::ifstream&) override;
	public:
		~LevelLoader() {};
		LIBRARY_API void Read(const std::string&) override;
		LIBRARY_API LevelLoader() {};
		LIBRARY_API IGraphics* GetGraphics();
		LIBRARY_API Window* GetWindow(const std::string&);
		LIBRARY_API bool CanRead(const std::string&) const override;
		LIBRARY_API const std::string GetInfo() const override;
	};
};