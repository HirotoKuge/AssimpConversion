#include <iostream>
#include "AssimpConversion.h"



int main(void) {

	AssimpConversion Convert;
	ComvertSettings setting;

	setting.loadFilePath = "assets/Alicia/FBX/Alicia_solid_Unity.FBX";
	setting.exportFileName = "assets/Converted/Alicia.md";
	

	//Convert.ComvertToMD(setting);

	Convert.LoadTest(setting.exportFileName);

	return 0;
}
