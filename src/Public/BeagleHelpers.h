#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <bitset>
/* ARM - Access Right Memory */
#define BEAGLE_ARM_ALL_ACCESS PROCESS_ALL_ACCESS
#define BEAGLE_ARM_READ_ONLY PROCESS_VM_READ
#define BEAGLE_ARM_WRITE_ONLY PROCESS_VM_WRITE | PROCESS_VM_OPERATION
#define BEAGLE_ARM_TERMINATE PROCESS_TERMINATE 
#define BEAGLE_ARM_PAGE_READWRITE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

/* Enumerations */
enum EProcessSearchType
{
	EPST_ProcessName,
	EPST_WindowTitle
};

/* Types */
typedef unsigned long BEAGLE_ARM;

/* Direct Memory Change Class */
template <typename BufferType>
class DirectMemoryChange
{

public:

	DirectMemoryChange() = delete;

	DirectMemoryChange(unsigned long _MemoryAdress, BufferType* _MemoryValue) : 
	MemoryAdress(_MemoryAdress), 
	MemoryValue(_MemoryValue) {};

	unsigned long MemoryAdress;
	BufferType *MemoryValue;
	SIZE_T size = sizeof(BufferType);
	SIZE_T *OperationWRBytes = nullptr;
};

/* Memory Change Class */
template <typename BufferType>
class MemoryChange
{

public:

	MemoryChange() = delete;

	MemoryChange(HANDLE _ProcessHandle, unsigned long _MemoryAdress, BufferType* _MemoryValue) : 
	ProcessHandle(_ProcessHandle), 
	MemoryAdress(_MemoryAdress), 
	MemoryValue(_MemoryValue) {};
	MemoryChange(HANDLE _ProcessHandle, DirectMemoryChange<BufferType> _DirectMemoryChange) : 
	ProcessHandle(_ProcessHandle)
	{
		MemoryAdress = _DirectMemoryChange.MemoryAdress;
		MemoryValue = _DirectMemoryChange.MemoryValue;
		size = _DirectMemoryChange.size;
		OperationWRBytes = _DirectMemoryChange.OperationWRBytes;
	}

	HANDLE ProcessHandle;
	unsigned long MemoryAdress;
	BufferType *MemoryValue;
	SIZE_T size = sizeof(BufferType);
	SIZE_T *OperationWRBytes = nullptr;
};

/* Helper Class */
class BeagleHelpers
{

private:

	static const char* StringErrorConvert(int Error);

public:

	static void GetProcessID_ByWindowName(const char* WindowTitle, DWORD &ProcessID);

	static void GetProcessID_ByWindowName(const char* WindowTitle, int &ProcessID);

	static bool GetProcessID_ByName(const char * ProcessName, DWORD & ProcessID);

	static std::vector<PROCESSENTRY32> MakeSnapshot();

	static void ShowProcessList();

	static HANDLE CreateProcessHandle(DWORD ProcessID, BEAGLE_ARM Permission);

	static HANDLE CreateProcessHandle(const char * Name, EProcessSearchType SearchType, BEAGLE_ARM Permission);

	static bool CloseProcessHandle(HANDLE ProcessHandle);

	static int GetWindowsPageSize();

	/*TESTE*/
	static void T(HANDLE p)
	{
		unsigned char * addr = 0x0;
		MEMORY_BASIC_INFORMATION meminfo;

		while (1)
		{
			if (VirtualQueryEx(p, addr, &meminfo, sizeof(meminfo)) == 0)
			{
				break;
			}

			

			if ((meminfo.State & MEM_COMMIT) && (meminfo.Protect & BEAGLE_ARM_PAGE_READWRITE))
			{
				for (int i = 0; i <(int) meminfo.RegionSize; i += 4)
				{
					int val;
					SIZE_T S;
					BOOL ReadResult = ReadProcessMemory(p,
						(void*)((int)meminfo.BaseAddress + i),
						&val,
						sizeof(int),
						&S);
					if (val == 782746)
					{
						int H = 1337;
						WriteProcessMemory(p, (void*)((int)meminfo.BaseAddress + i), &H, sizeof(int), &S);
						std::cout << std::hex << (unsigned int)meminfo.BaseAddress + i << std::dec << " VAL: " << val << std::endl;
					}
					
				}
			}

			
			

			addr = (unsigned char *)meminfo.BaseAddress + meminfo.RegionSize;
			
			//std::cout << std::hex << meminfo.State << std::endl;
		}
			
	}

	template <typename BufferType>
	static bool ReadMemory(MemoryChange<BufferType> MemoryInfo);

	template<typename BufferType>
	static bool WriteMemory(MemoryChange<BufferType> MemoryInfo);

	template <typename BufferType>
	static bool DirectReadProcessMemory(DWORD ProcessID, DirectMemoryChange<BufferType> _MemoryChange);

	template <typename BufferType>
	static bool DirectReadProcessMemory(const char* Name, EProcessSearchType SearchType, DirectMemoryChange<BufferType> _MemoryChange);

	template <typename BufferType>
	static bool DirectWriteProcessMemory(DWORD ProcessID, DirectMemoryChange<BufferType> _MemoryChange);

	template <typename BufferType>
	static bool DirectWriteProcessMemory(const char* Name, EProcessSearchType SearchType, DirectMemoryChange<BufferType> _MemoryChange);
};


/* Template Method Implementaion*/

template <typename BufferType>
bool BeagleHelpers::ReadMemory(MemoryChange<BufferType> MemoryInfo)
{
	BOOL ReadResult = ReadProcessMemory(MemoryInfo.ProcessHandle,
										(LPCVOID)MemoryInfo.MemoryAdress,
										MemoryInfo.MemoryValue,
										MemoryInfo.size,
										MemoryInfo.OperationWRBytes);

	return (ReadResult > 0) ? true : false;
}

template <typename BufferType>
bool BeagleHelpers::WriteMemory(MemoryChange<BufferType> MemoryInfo)
{
	BOOL WriteResult = WriteProcessMemory(MemoryInfo.ProcessHandle,
										(LPVOID)MemoryInfo.MemoryAdress,
										MemoryInfo.MemoryValue,
										MemoryInfo.size,
										MemoryInfo.OperationWRBytes);

	return (WriteResult > 0) ? true : false;
}

template <typename BufferType>
bool BeagleHelpers::DirectReadProcessMemory(DWORD ProcessID, DirectMemoryChange<BufferType> _MemoryChange)
{
	HANDLE ProcessHandle = CreateProcessHandle(ProcessID, BEAGLE_ARM_READ_ONLY);
	MemoryChange<BufferType> ToRead(ProcessHandle, _MemoryChange);
	BOOL ReadResult = ReadMemory(ToRead);
	CloseProcessHandle(ProcessHandle);
	return (ReadResult > 0) ? true : false;
}

template <typename BufferType>
bool BeagleHelpers::DirectReadProcessMemory(const char* Name, EProcessSearchType SearchType, DirectMemoryChange<BufferType> _MemoryChange)
{
	HANDLE ProcessHandle = CreateProcessHandle(Name, SearchType, BEAGLE_ARM_READ_ONLY);
	MemoryChange<BufferType> ToRead(ProcessHandle, _MemoryChange);
	BOOL ReadResult = ReadMemory(ToRead);
	CloseProcessHandle(ProcessHandle);
	return (ReadResult > 0) ? true : false;
}

template <typename BufferType>
bool BeagleHelpers::DirectWriteProcessMemory(DWORD ProcessID, DirectMemoryChange<BufferType> _MemoryChange)
{
	HANDLE ProcessHandle = CreateProcessHandle(ProcessID, BEAGLE_ARM_WRITE_ONLY);
	MemoryChange<BufferType> ToWrite(ProcessHandle, _MemoryChange);
	BOOL WriteResult = WriteMemory(ToWrite);
	CloseProcessHandle(ProcessHandle);
	return (WriteResult > 0) ? true : false;
}

template <typename BufferType>
bool BeagleHelpers::DirectWriteProcessMemory(const char* Name, EProcessSearchType SearchType, DirectMemoryChange<BufferType> _MemoryChange)
{
	HANDLE ProcessHandle = CreateProcessHandle(Name, SearchType, BEAGLE_ARM_WRITE_ONLY);
	MemoryChange<BufferType> ToWrite(ProcessHandle, _MemoryChange);
	BOOL WriteResult = WriteMemory(ToWrite);
	CloseProcessHandle(ProcessHandle);
	return (WriteResult > 0) ? true : false;
}