#pragma once
#include <Windows.h>
#include <assert.h>
#include <map>
#include <memory>
#include "BeagleHelpers.h"

#define DEFAULT_READ_REGION_LOOP(TYPE, EXECUTION, FINALCOUNT) \
HandleIsValidPtr();\
unsigned long MCount = 0;\
for (int i = 0; i < (int)PageRegionSize; i += sizeof(TYPE)) \
{ \
	TYPE ReadedValue; \
	MemoryChange<TYPE> Data{ \
		ProcessOwner->GetProcessHandle(), \
		(unsigned long)((int)PageAddress + i), \
		&ReadedValue \
	}; \
	bool Success = BeagleHelpers::ReadMemory(Data);\
	if(Success)\
	{\
			EXECUTION(Success, ReadedValue, MCount, (unsigned long)((int)PageAddress + i)); \
			MCount++;\
	}\
}\
FINALCOUNT = MCount;

struct ReadedPageInformation
{
	unsigned long PageAdress = 0x0;
	unsigned long FoundedValues = 0;
};

typedef struct ReadedPageInformation ReadedPageInformation;

class BeagleProcess;

class BeagleMemoryPage
{

private:

	BeagleMemoryPage();

public:

	BeagleMemoryPage(BeagleProcess* Process, MEMORY_BASIC_INFORMATION _MemoryInformation);

	BeagleProcess* GetProcessOwner();

	int GetMemoryStateCode();

	unsigned long GetPageAddress();

	void HandleIsValidPtr();

	template <typename T> 
	std::map<unsigned long, T> ReadPageRegionByType(ReadedPageInformation & ReadInfo);

private:

	BeagleProcess* ProcessOwner;

	PVOID PageAddress;

	SIZE_T PageRegionSize;

	DWORD PageState;

	DWORD PageProtect;

};

