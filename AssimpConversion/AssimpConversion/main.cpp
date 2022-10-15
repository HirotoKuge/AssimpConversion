#include <iostream>
#include "AssimpLoader.h"
#include "ModelDataExpoter.h"
#include"SharedStruct.h"
#include"MDLoader.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

const wchar_t* modelFile = L"Assets/arrowModel.fbx";
std::vector<Mesh> meshes; // ���b�V���̔z��
std::vector<Mesh> setMeshes; // ���b�V���̔z��



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
