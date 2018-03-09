#pragma once
#include <memory>
#include <vector>
#include "..\Public\BeagleMemoryPage.h"

class BeagleProcess
{

private:

	BeagleProcess();

public:

	BeagleProcess(DWORD _ProcessID);
	
	BeagleProcess(const char* ProcessName);

	void Close();

	std::vector<BeagleMemoryPage*> SweepProcessPages();

public:

	int ProcessID;

	HANDLE ProcessHandle;

	std::vector< std::shared_ptr< BeagleMemoryPage > > CurrentPages;
};

