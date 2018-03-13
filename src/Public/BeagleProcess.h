#pragma once
#include <memory>
#include <vector>
#include "..\Public\BeagleMemoryPage.h"

typedef std::vector <
		std::shared_ptr< BeagleMemoryPage >
		> PageMemoryArray;

class BeagleProcess
{

private:

	BeagleProcess();

public:

	BeagleProcess(DWORD _ProcessID, bool SweepOnInit = false);
	
	BeagleProcess(const char* ProcessName, bool SweepOnInit = false);

	HANDLE GetProcessHandle();

	int GetProcessID();

	void Close();

	void SweepProcessPages();

	void SweepProcessPages(unsigned long Type);

	void ClearSavedProcessPages();

	PageMemoryArray GetSavedPages();

private:

	PageMemoryArray CurrentPages;

	int ProcessID;

	HANDLE ProcessHandle;
	
};

