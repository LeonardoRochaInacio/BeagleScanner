#include "..\Public\BeagleMemoryPage.h"

BeagleMemoryPage::BeagleMemoryPage(MEMORY_BASIC_INFORMATION _MemoryInformation)
{
	PageAddress = _MemoryInformation.BaseAddress;
	PageRegionSize = _MemoryInformation.RegionSize;
	PageState = _MemoryInformation.State;
	PageProtect = _MemoryInformation.Protect;
}
