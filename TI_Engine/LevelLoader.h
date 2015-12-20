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
		std::ifstream _stream;
		std::string _currentFilename;
		Window* _pWindow = nullptr;
		IGraphics* _pGraphics = nullptr;
		void _Open(const std::string&);
		void _Close();
		static const std::string _PATH;
	protected:
		void InternalRead(const std::string&) override;
	public:
		LIBRARY_API LevelLoader() {};
		LIBRARY_API ~LevelLoader() {};
		LIBRARY_API IGraphics* GetGraphics();
		LIBRARY_API Window* GetWindow(const std::string&);
		LIBRARY_API bool CanRead(const std::string&) const override;
		LIBRARY_API const std::string GetInfo() const override;
	};
};