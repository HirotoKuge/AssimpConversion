#pragma once
#include<string>
#include "AssimpLoader.h"
#include "SharedStruct.h"




class ModelDataExpoter{
public:
	bool ExportModelData(std::vector<Mesh>&);//モデルデータのエクスポート処理
	void SetFilePath(std::string);//ファイルパス設定用関数

private:
	std::string exportFilePath = "template.md";//出力するファイルパス
};

