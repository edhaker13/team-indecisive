#pragma once
#include <string>
#include <Windows.h>

namespace Indecisive
{
	class ISoundManager
	{
	public:
		virtual bool CanRead(const std::string&) const = 0;
		virtual const std::string GetInfo() const = 0;
		virtual bool Load(const std::string&) = 0;
		virtual bool Play(const std::string& filename, DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags) = 0;
		virtual bool Initialize(HWND) = 0;
		virtual void Shutdown() = 0;
		virtual bool IsInitialised() = 0;
	};
};