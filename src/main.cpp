#include <iostream>
#include "BeagleHelpers.h"

#define CURRENT_LINE __LINE__


int main(int argc, char * argv[])
{
	//std::cout << BeagleHelpers::GetWindowsPageSize() << std::endl;
	HANDLE X =  BeagleHelpers::CreateProcessHandle("LoseHealth.exe", EProcessSearchType::EPST_ProcessName, BEAGLE_ARM_ALL_ACCESS);
	BeagleHelpers::T(X);


	system("pause");
}