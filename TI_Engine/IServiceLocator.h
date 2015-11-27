#pragma once
#include <string>
#include "LibraryAPI.h"

namespace Indecisive
{
	struct IServiceLocator
	{
		virtual void Add(std::string name, void* instance) = 0;
		virtual void* Get(std::string service) = 0;
	};

	LIBRARY_API IServiceLocator* ServiceLocatorInstance();
}