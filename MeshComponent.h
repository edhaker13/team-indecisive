#pragma once
#include <Windows.h>	
#include <fstream>		
#include <vector>		
#include <map>			

namespace Indecisive
{ 

	class MeshComponent
	{
	private:
		bool StoreVertex();
		bool StoreIndices();

	public:
		MeshComponent();
		~MeshComponent();
	};

}