#pragma once
#include <map>
#include "IServiceLocator.h"

namespace Indecisive
{
	class ServiceLocator : public IServiceLocator
	{
	private:
		std::map<std::string, void*> _services;
		ServiceLocator(ServiceLocator const&) = delete;
		void operator=(ServiceLocator const&) = delete;

	public:
		ServiceLocator() {};
		virtual void Add(std::string, void*) override;
		virtual void* Get(std::string) override;
	};
}