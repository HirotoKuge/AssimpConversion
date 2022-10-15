#pragma once
#include<string>
#include "AssimpLoader.h"
#include "SharedStruct.h"

class MDLoader{
public:
	
	bool LoadMD(std::vector<Mesh>&, std::string);


private:
	std::string loadFilepath;

};

