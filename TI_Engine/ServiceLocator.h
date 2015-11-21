#pragma once
#include <map>

namespace Indecisive
{
	class ServiceLocator
	{
	private:
		std::map<std::string, void*> serviceMap;
		ServiceLocator() {};
		ServiceLocator(ServiceLocator const&) = delete;
		void operator=(ServiceLocator const&) = delete;

	public:
		static ServiceLocator* Instance(){ static ServiceLocator s; return &s; };
		void Add(std::string name, void* instance){ serviceMap.emplace(name, instance); };
		void* Get(std::string service) { if (!serviceMap.empty()) { return serviceMap[service]; } return nullptr; };
	};
}