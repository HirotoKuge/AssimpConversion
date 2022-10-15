#include <iostream>
#include "AssimpLoader.h"
#include "ModelDataExpoter.h"
#include"SharedStruct.h"
#include"MDLoader.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

const wchar_t* modelFile = L"Assets/arrowModel.fbx";
std::vector<Mesh> meshes; // メッシュの配列
std::vector<Mesh> setMeshes; // メッシュの配列



int main(void) {

	ImportSettings settings={
		modelFile,
		meshes,
		false,
		true
	};

	AssimpLoader assimpLoader;
	ModelDataExpoter exporter;
	MDLoader loader;
	
	assimpLoader.Load(settings);

	//exporter.SetFilePath("assets/Converted/Alicia.md");

	//exporter.ExportModelData(meshes);


	loader.LoadMD(setMeshes, "assets/Converted/Alicia.md");


	int a = 0;

	return 0;
}
