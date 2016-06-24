#pragma once
#include <fstream>
#include <string>
#include <stdexcept>
#include "DirectXMath.h" 

namespace Indecisive
{
	// Common methods to be use when reading data
	class IReadable
	{
	protected:
		static const std::string _PATH;
		virtual void _Open(const std::string&, std::ifstream&) = 0;
	public:
		virtual bool CanRead(const std::string&) const = 0;
		virtual const std::string GetInfo() const = 0;
		virtual void Read(const std::string& file) = 0;
	};
};