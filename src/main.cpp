#include <iostream>
#include "Public\BeagleProcess.h"

int main(int argc, char * argv[])
{
	BeagleProcess NotePad("ScanTeste1.exe", true);

	for (auto x : NotePad.GetSavedPages())
	{
		//std::cout << std::hex << x->GetMemoryStateCode() << std::endl;
		x->ReadPageRegionByType<int>();
	}

	system("pause");
}