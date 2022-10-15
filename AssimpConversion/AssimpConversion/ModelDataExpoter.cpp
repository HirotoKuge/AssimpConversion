#include <stdio.h> 
#include <stdlib.h>
#include<iostream>
#include "ModelDataExpoter.h"

bool ModelDataExpoter::ExportModelData(std::vector<Mesh>& meshes) {
	FILE* outputfile;	// 出力ストリーム
	outputfile = fopen(exportFilePath.c_str(), "wb");  // ファイルを書き込み用にオープン(開く)

	if (outputfile == NULL) {
		std::cout << "file open is failed!" << std::endl;
		return false;
	}

	std::cout << "start export modeldata" << std::endl;

	//読み込みに必要な情報を構造体にまとめて書き出し
	int meshNum = meshes.size();
	
	//メッシュの数
	fwrite(&meshNum, sizeof(int), 1, outputfile);
	
	std::cout << "export number of meshes complete" << std::endl;


	//メッシュの数だけ出力処理を回す
	for (int i = 0; i < meshNum; i++) {


		std::cout << "meshNo " << i << " export start" << std::endl;

		//(複数メッシュからなるモデルにも対応できる用に)書き出す対象のメッシュを取り出す
		auto exportMesh = meshes.at(i);

		//頂点数取得
		int vertexNum = exportMesh.Vertices.size();

		//インデックス数取得
		int indexNum = exportMesh.Indices.size();

		//メッシュ対応のカラーマップパス取得
		auto diffusemap = exportMesh.DiffuseMap;


		//頂点数書き込み
		fwrite(&vertexNum, sizeof(int), 1, outputfile);

		std::cout << "export number of vertices complete" << std::endl;


		//頂点情報を書き出す
		for (int j = 0; j < vertexNum; j++) {
			auto writeVertex = meshes[i].Vertices[j];
			auto n = fwrite(&writeVertex, sizeof(Vertex), 1, outputfile);
		}

		std::cout << "export vertex data complete" << std::endl;


		//インデックス数書き込み
		fwrite(&indexNum, sizeof(int), 1, outputfile);

		std::cout << "export number of indices complete" << std::endl;


		//インデックス書き込み
		for (int j = 0; j < indexNum; j++) {
			int writeIndex = meshes[i].Indices[j];
			auto n = fwrite(&writeIndex, sizeof(int), 1, outputfile);
		}

		std::cout << "export index dara complete" << std::endl;


		//diffusemapのファイルパスの文字数を取得して書き出し
		int wordLength = diffusemap.length();
		fwrite(&wordLength, sizeof(int), 1, outputfile);

		std::cout << "export diffusemap file path length complete" << std::endl;


		//diffusemapのファイルパス書き出し
		for (int j = 0; j < wordLength; j++) {
			auto writeWord = diffusemap[j];
			fwrite(&writeWord, sizeof(wchar_t), 1, outputfile);
		}

		std::cout << "export diffusemap file path complete" << std::endl;

		std::cout << "meshNo " << i << " export complete" << std::endl;


	}
	
	fclose(outputfile);


	std::cout<<"export modeldata to .md complete" << std::endl;

	return true;
	
}

void ModelDataExpoter::SetFilePath(std::string filePath) { exportFilePath = filePath; }
