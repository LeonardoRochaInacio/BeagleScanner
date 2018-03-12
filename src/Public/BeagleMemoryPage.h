#pragma once
#include <Windows.h>
#include <assert.h>
#include "BeagleHelpers.h"

#define DEFAULT_READ_REGION_LOOP(TYPE, EXECUTION) \
HandleIsValidPtr();\
for (int i = 0; i < (int)PageRegionSize; i += sizeof(TYPE)) \
{ \
	TYPE ReadedValue; \
	MemoryChange<TYPE> Data{ \
		ProcessOwner->GetProcessHandle(), \
		(unsigned long)((int)PageAddress + i), \
		&ReadedValue \
	}; \
	bool Success = BeagleHelpers::ReadMemory(Data);\
	if(Success) EXECUTION(Success, ReadedValue, (unsigned long)((int)PageAddress + i));\
} 

enum MemoryState
{
	Commit,
	Free,
	Reserve
};

class BeagleProcess;

class BeagleMemoryPage
{

private:

	BeagleMemoryPage();

public:

	BeagleMemoryPage(BeagleProcess* Process, MEMORY_BASIC_INFORMATION _MemoryInformation);

	BeagleProcess* GetProcessOwner();

	int GetMemoryStateCode();

	void HandleIsValidPtr();

	template <typename T> 
	void ReadPageRegionByType();

private:

	BeagleProcess* ProcessOwner;

	PVOID PageAddress;

	SIZE_T PageRegionSize;

	DWORD PageState;

	DWORD PageProtect;

};

