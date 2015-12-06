#pragma once
#include <string>
#include "LibraryAPI.h"

namespace Indecisive
{
	struct IServiceLocator
	{
		virtual void Add(std::string, void*) = 0;
		virtual void* Get(std::string) = 0;
	};

	LIBRARY_API IServiceLocator* ServiceLocatorInstance();
}