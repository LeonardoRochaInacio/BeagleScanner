#include "..\Public\BeagleProcess.h"
#include "..\Public\BeagleHelpers.h"

BeagleProcess::BeagleProcess(DWORD _ProcessID)
{
	ProcessID = _ProcessID;
	ProcessHandle = BeagleHelpers::CreateProcessHandle(ProcessID, BEAGLE_ARM_ALL_ACCESS);
}

BeagleProcess::BeagleProcess(const char * ProcessName)
{
	DWORD _ProcessID;
	BeagleHelpers::GetProcessID_ByName(ProcessName, _ProcessID);
	ProcessID = _ProcessID;
	ProcessHandle = BeagleHelpers::CreateProcessHandle(ProcessID, BEAGLE_ARM_ALL_ACCESS);
}

void BeagleProcess::Close()
{
	if(ProcessHandle) CloseHandle(ProcessHandle);
}

std::vector<BeagleMemoryPage*> BeagleProcess::SweepProcessPages()
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
			CurrentPages.push_back(std::shared_ptr<BeagleMemoryPage>(new BeagleMemoryPage{ MemoryInfo }));
		}

		bFirstLoop = false;
	}
}

