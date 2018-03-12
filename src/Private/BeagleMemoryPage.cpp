#include "..\Public\BeagleMemoryPage.h"
#include "..\Public\BeagleProcess.h"
#include <iostream>

BeagleMemoryPage::BeagleMemoryPage(BeagleProcess *Process, MEMORY_BASIC_INFORMATION _MemoryInformation)
{
	PageAddress = _MemoryInformation.BaseAddress;
	PageRegionSize = _MemoryInformation.RegionSize;
	PageState = _MemoryInformation.State;
	PageProtect = _MemoryInformation.Protect;
	ProcessOwner = Process;
}

BeagleProcess * BeagleMemoryPage::GetProcessOwner()
{
	return ProcessOwner;
}

int BeagleMemoryPage::GetMemoryStateCode()
{
	return PageState;
}

void BeagleMemoryPage::HandleIsValidPtr()
{
	assert(ProcessOwner);
}

template<>
void BeagleMemoryPage::ReadPageRegionByType<int>()
{
	DEFAULT_READ_REGION_LOOP(int, 
	[](bool Success, int Value, int MemoryAdress)
	{
		if (!Success) return;
		//SOMENTE PARA TESTES, VERIFICAR ESTADO DA PAGINA
		if (Value == 782746)
		{
			std::cout << "Data: " << std::hex << MemoryAdress << std::endl;
		}
			
	});
}

template<>
void BeagleMemoryPage::ReadPageRegionByType<float>()
{
	
	DEFAULT_READ_REGION_LOOP(float,
	[](bool Success, float Value, int MemoryAdress)
	{
		
	});
}

