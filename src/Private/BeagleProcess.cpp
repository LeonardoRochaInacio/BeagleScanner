#include "..\Public\BeagleProcess.h"
#include "..\Public\BeagleHelpers.h"

BeagleProcess::BeagleProcess(DWORD _ProcessID, bool SweepOnInit)
{
	ProcessID = _ProcessID;
	ProcessHandle = BeagleHelpers::CreateProcessHandle(ProcessID, BEAGLE_ARM_ALL_ACCESS);
	if (SweepOnInit) SweepProcessPages(MEM_COMMIT);
}

BeagleProcess::BeagleProcess(const char * ProcessName, bool SweepOnInit)
{
	DWORD _ProcessID;
	BeagleHelpers::GetProcessID_ByName(ProcessName, _ProcessID);
	ProcessID = _ProcessID;
	ProcessHandle = BeagleHelpers::CreateProcessHandle(ProcessID, BEAGLE_ARM_ALL_ACCESS);
	if (SweepOnInit) SweepProcessPages(MEM_COMMIT);
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
	SweepProcessPages(MEM_COMMIT | MEM_FREE | MEM_RESERVE);
}

void BeagleProcess::SweepProcessPages(unsigned long Type)
{
	ClearSavedProcessPages();

	unsigned char * Address = 0x0;
	MEMORY_BASIC_INFORMATION MemoryInfo;

	bool bFirstLoop = true;

	while (1)
	{
		if (VirtualQueryEx(ProcessHandle, Address, &MemoryInfo, sizeof(MemoryInfo)) == 0)
		{
			break;
		}

		if (!bFirstLoop && (MemoryInfo.State & Type) && (MemoryInfo.Protect & BEAGLE_ARM_PAGE_READWRITE))
		{
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

