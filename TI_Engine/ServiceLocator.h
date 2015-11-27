#pragma once
#include <map>
#include "IServiceLocator.h"

namespace Indecisive
{
	class ServiceLocator: public IServiceLocator
	{
	private:
		std::map<std::string, void*> serviceMap;
		ServiceLocator(ServiceLocator const&) = delete;
		void operator=(ServiceLocator const&) = delete;

	public:
		ServiceLocator() {};
		virtual void Add(std::string name, void* instance);
		virtual void* Get(std::string service);
	};
}