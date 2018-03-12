#include "..\Public\BeagleMemoryPage.h"
#include "..\Public\BeagleProcess.h"

BeagleMemoryPage::BeagleMemoryPage(BeagleProcess *Process, MEMORY_BASIC_INFORMATION _MemoryInformation)
{
	PageAddress = _MemoryInformation.BaseAddress;
	PageRegionSize = _MemoryInformation.RegionSize;
	PageState = _MemoryInformation.State;
	PageProtect = _MemoryInformation.Protect;
	ProcessOwner = Process;
}

void BeagleMemoryPage::HandleIsValidPtr()
{
	assert(!ProcessOwner);
}

template<>
void BeagleMemoryPage::ReadPageRegionByType<int>()
{
	DEFAULT_READ_REGION_LOOP(int, 
	[](int X)
	{

	});
}

template<>
void BeagleMemoryPage::ReadPageRegionByType<float>()
{
	
	DEFAULT_READ_REGION_LOOP(float,
	[](float X)
	{

	});
}

