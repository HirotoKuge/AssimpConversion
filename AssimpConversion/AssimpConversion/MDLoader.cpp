#include "MDLoader.h"
#include <time.h>
#include<iostream>


//.mdファイルについて
//.mdファイルはモデルデータをバイナリで独自形式に出力したもの
//解釈用にデータの格納構成を以下に記載
//メッシュ総数
//（以下メッシュごとに)
//頂点数
//頂点情報(Vertex構造体と同じ内容)
//インデックス数
//インデックス
//ディフューズマップのパスの文字数
//ディフューズマップのパス(1文字ずつ格納)


bool MDLoader::LoadMD(std::vector<Mesh>& meshes, std::string filepath){
	clock_t start = clock();
	//ファイルの奴
	FILE* fp = nullptr;

	//読みたいmdファイルを読み取り専用でオープン
	fp = fopen(filepath.c_str(), "rb");


	//メッシュ数を読み取り
	int meshNum = 0;
	fread(&meshNum, sizeof(int), 1, fp);

	//メッシュ数だけ回す
	for (int i = 0; i < meshNum; i++) {
		Mesh toHouseMesh;//格納用のメッシュ
		
		//頂点数読み取り
		int vertexNum = 0;
		fread(&vertexNum, sizeof(int), 1, fp);

		//頂点データ読みだして格納
		for (int j = 0; j < vertexNum; j++) {
			Vertex tmp;
			fread(&tmp, sizeof(Vertex), 1, fp);
			toHouseMesh.Vertices.emplace_back(tmp);
		}

		//インデックス数読み取り
		int indexNum = 0;
		fread(&indexNum, sizeof(int), 1, fp);

		//インデックス読みだして格納
		for (int j = 0; j < indexNum; j++) {
			uint32_t tmp;
			fread(&tmp, sizeof(int), 1, fp);
			toHouseMesh.Indices.emplace_back(tmp);
		}

		//ファイルパスの文字数読み取り
		int pathLength = 0;
		fread(&pathLength, sizeof(int), 1, fp);

		for (int j = 0; j < pathLength; j++) {
			wchar_t tmp;
			fread(&tmp, sizeof(wchar_t), 1, fp);
			toHouseMesh.DiffuseMap += tmp;
		}

		meshes.emplace_back(toHouseMesh);
		
	}



	clock_t end = clock();

	double time = (double)(end - start) / CLOCKS_PER_SEC;

	std::cout << "required time [.md]:" << time * 1000 << "ms" << std::endl;



	return true;
}
