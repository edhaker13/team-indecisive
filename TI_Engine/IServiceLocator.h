#pragma once
#include <string>
#include "LibraryAPI.h"

namespace Indecisive
{
	struct IServiceLocator
	{
		virtual void Add(const std::string&, void*) = 0;
		virtual void* Get(const std::string&) = 0;
	};

	LIBRARY_API IServiceLocator* ServiceLocatorInstance();
}