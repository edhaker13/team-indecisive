#include "ServiceLocator.h"

namespace Indecisive
{
	IServiceLocator* ServiceLocatorInstance() { static ServiceLocator s; return &s; };

	void ServiceLocator::Add(const std::string& name, void* instance)
	{
		_services.emplace(name, instance);
	};

	void* ServiceLocator::Get(const std::string& service)
	{
		if (!_services.empty())
		{
			return _services[service];
		}
		return nullptr;
	};
}