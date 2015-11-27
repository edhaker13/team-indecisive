#include "ServiceLocator.h"

namespace Indecisive
{
	IServiceLocator* ServiceLocatorInstance(){ static ServiceLocator s; return &s; };

	void ServiceLocator::Add(std::string name, void* instance)
	{
		serviceMap.emplace(name, instance);
	};

	void* ServiceLocator::Get(std::string service)
	{
		if (!serviceMap.empty())
		{
			return serviceMap[service];
		}
		return nullptr;
	};
}