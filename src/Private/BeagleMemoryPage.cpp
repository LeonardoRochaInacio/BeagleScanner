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

unsigned long BeagleMemoryPage::GetPageAddress()
{
	return (unsigned long)PageAddress;
}

void BeagleMemoryPage::HandleIsValidPtr()
{
	assert(ProcessOwner);
}

template<>
std::map<unsigned long, int> BeagleMemoryPage::ReadPageRegionByType<int>(ReadedPageInformation & ReadInfo)
{
	std::map<unsigned long, int> ReturnedData;
	DEFAULT_READ_REGION_LOOP(int, 
	[&](bool Success, int Value, unsigned long Count, unsigned long MemoryAdress)
	{
		if (!Success) return;
		ReturnedData[MemoryAdress] = Value;
	},
	ReadInfo.FoundedValues);
	ReadInfo.PageAdress = GetPageAddress();

	return ReturnedData;
}

template<>
std::map<unsigned long, float> BeagleMemoryPage::ReadPageRegionByType<float>(ReadedPageInformation & ReadInfo)
{
	std::map<unsigned long, float> ReturnedData;
	DEFAULT_READ_REGION_LOOP(float,
	[&](bool Success, float Value, unsigned long Count, unsigned long MemoryAdress)
	{
		if (!Success) return;
		ReturnedData[MemoryAdress] = Value;
	},
	ReadInfo.FoundedValues);
	ReadInfo.PageAdress = GetPageAddress();

	return ReturnedData;
}

