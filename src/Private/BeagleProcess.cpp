#include <iostream>
#include "..\Public\BeagleProcess.h"
#include "..\Public\BeagleHelpers.h"

BeagleProcess::BeagleProcess(DWORD _ProcessID, bool SweepOnInit)
{
	ProcessID = _ProcessID;
	ProcessHandle = BeagleHelpers::CreateProcessHandle(ProcessID, BEAGLE_ARM_ALL_ACCESS);
	if (SweepOnInit) SweepProcessPages();
}

BeagleProcess::BeagleProcess(const char * ProcessName, bool SweepOnInit)
{
	DWORD _ProcessID;
	BeagleHelpers::GetProcessID_ByName(ProcessName, _ProcessID);
	ProcessID = _ProcessID;
	ProcessHandle = BeagleHelpers::CreateProcessHandle(ProcessID, BEAGLE_ARM_ALL_ACCESS);
	if (SweepOnInit) SweepProcessPages();
}

HANDLE BeagleProcess::GetProcessHandle()
{
	return ProcessHandle;
}

int BeagleProcess::GetProcessID()
{
	return ProcessID;
}

void BeagleProcess::Close()
{
	if(ProcessHandle) CloseHandle(ProcessHandle);
}

void BeagleProcess::SweepProcessPages()
{
	unsigned char * Address = 0x0;
	MEMORY_BASIC_INFORMATION MemoryInfo;

	bool bFirstLoop = true;

	while (1)
	{
		if (VirtualQueryEx(ProcessHandle, Address, &MemoryInfo, sizeof(MemoryInfo)) == 0)
		{
			break;
		}

		if (!bFirstLoop)
		{
			std::cout << MemoryInfo.BaseAddress << std::endl;
			CurrentPages.push_back(std::shared_ptr<BeagleMemoryPage>(new BeagleMemoryPage{ this, MemoryInfo }));
		}

		Address += MemoryInfo.RegionSize;

		bFirstLoop = false;
	}
}

void BeagleProcess::ClearSavedProcessPages()
{
	if(CurrentPages.size() > 0 ) CurrentPages.clear();
}

PageMemoryArray BeagleProcess::GetSavedPages()
{
	return CurrentPages;
}

