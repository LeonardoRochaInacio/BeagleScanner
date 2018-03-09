#pragma once
#include <Windows.h>

class BeagleMemoryPage
{

private:

	BeagleMemoryPage();

public:

	BeagleMemoryPage(MEMORY_BASIC_INFORMATION _MemoryInformation);

	void ReadPageRegion()
	{
		for (int i = 0; i < (int)PageRegionSize; i += 4)
		{
			/*int Value;
			SIZE_T S;
			BOOL ReadResult = ReadProcessMemory(p,
				(void*)((int)meminfo.BaseAddress + i),
				&val,
				sizeof(int),
				&S);*/
		}
	}

public:

	PVOID PageAddress;
	SIZE_T PageRegionSize;
	DWORD PageState;
	DWORD PageProtect;

};

