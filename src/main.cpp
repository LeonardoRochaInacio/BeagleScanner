#include <iostream>
#include "Public\BeagleProcess.h"

int main(int argc, char * argv[])
{
	BeagleProcess NotePad("ScanTeste1.exe", true);
	int i = 0;
	for (auto x : NotePad.GetSavedPages())
	{
		i++;
		//std::cout << i << std::endl;
		ReadedPageInformation PageInfo;
		std::map<unsigned long, int> aaa = x->ReadPageRegionByType<int>(PageInfo);
		if (aaa.count(0x005BF9CC))
		{
			int asd = aaa.find(0x005BF9CC)->second;
			std::cout << asd << std::endl;
		}
		
	}

	system("pause");
}