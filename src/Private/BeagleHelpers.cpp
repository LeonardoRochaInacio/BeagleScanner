#include <string>
#include <iostream>
#include "..\Public\BeagleHelpers.h"

const char * BeagleHelpers::StringErrorConvert(int Error)
{

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	return std::string(messageBuffer, size).c_str();
}

void BeagleHelpers::GetProcessID_ByWindowName(const char* WindowTitle, DWORD &ProcessID)
{
	HWND Window = FindWindow(0, WindowTitle);
	GetWindowThreadProcessId(Window, &ProcessID);
}

void BeagleHelpers::GetProcessID_ByWindowName(const char* WindowTitle, int &ProcessID)
{
	HWND Window = FindWindow(0, WindowTitle);
	DWORD _ProcessID;
	GetWindowThreadProcessId(Window, &_ProcessID);
	ProcessID = (int)_ProcessID;
	
}

bool BeagleHelpers::GetProcessID_ByName(const char* ProcessName, DWORD &ProcessID)
{

	std::vector<PROCESSENTRY32> Snapshot = MakeSnapshot();
	for (PROCESSENTRY32 Process : Snapshot)
	{
		if (std::strcmp(Process.szExeFile, ProcessName) == 0)
		{
			ProcessID = Process.th32ProcessID;
			return true;
		}
	}
	return false;
}

std::vector<PROCESSENTRY32> BeagleHelpers::MakeSnapshot()
{
	std::vector<PROCESSENTRY32> ProcessList;
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	try
	{
		if (SnapShot == INVALID_HANDLE_VALUE) throw "Invalid handle on MakeSnapshot() - CreateToolhelp32Snapshot()";

		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(SnapShot, &procEntry)) throw "First process not found. Possible empty process list.";

		for (int ValidProcess = 1; ValidProcess > 0; ValidProcess = Process32Next(SnapShot, &procEntry))
		{
			ProcessList.push_back(procEntry);
		}
	}
	catch(const char* Error)
	{
		std::cerr << Error << std::endl;
	}

	return ProcessList;
}

void BeagleHelpers::ShowProcessList()
{
	std::vector<PROCESSENTRY32> Snapshot = MakeSnapshot();
	for (PROCESSENTRY32 Process : Snapshot)
	{
		std::cout << Process.szExeFile << " - " << Process.th32ProcessID << std::endl;
	}
}

HANDLE BeagleHelpers::CreateProcessHandle(DWORD ProcessID, BEAGLE_ARM Permission)
{

	HANDLE ProcessHandle = OpenProcess(Permission, false, ProcessID);
	if (ProcessHandle == NULL) return nullptr;
	
	return ProcessHandle;
}

HANDLE BeagleHelpers::CreateProcessHandle(const char * Name, EProcessSearchType SearchType, BEAGLE_ARM Permission)
{
	DWORD ProcessID =
		[&]() -> DWORD
	{
		switch (SearchType)
		{
		case EPST_ProcessName:
		{
			DWORD pID;
			GetProcessID_ByName(Name, pID);
			return pID;
		}
		case EPST_WindowTitle:
		{
			DWORD pID;
			GetProcessID_ByWindowName(Name, pID);
			return pID;
		}
		}

		return 0;
	}();

	return CreateProcessHandle(ProcessID, Permission);
}

bool BeagleHelpers::CloseProcessHandle(HANDLE ProcessHandle)
{
	return ( CloseHandle(ProcessHandle) > 0 ) ? true : false ;
}

int BeagleHelpers::GetWindowsPageSize()
{
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	return SystemInfo.dwPageSize;
}
