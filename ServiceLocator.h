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
		ServiceLocator& Instance(){ static ServiceLocator s; return s; };
		void AddService(std::string name, void* instance){ serviceMap.emplace(name, instance); };
		void* GetInstance(std::string service) { if (!serviceMap.empty()) { return serviceMap[service]; } return nullptr; };
	};
}