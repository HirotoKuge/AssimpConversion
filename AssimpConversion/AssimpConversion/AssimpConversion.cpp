#include "AssimpConversion.h"
#include <filesystem>
#include<time.h>
#include<iostream>

namespace fs = std::filesystem;

//=============================================================================
// mdファイルフォーマット
//=============================================================================
namespace mdFileFormat {
	// 現在のmdファイルのバージョン
	std::uint16_t VERSION = 1;
	
	// マテリアルの数

	// 全体のヘッダー
	struct MD_Header {
		std::uint16_t version;	 //バージョン
		std::uint16_t numMeshes; //メッシュ数
	};

	// 単位メッシュのヘッダー
	struct MD_UnitMeshHeader {
		std::uint32_t numMaterial;	//マテリアルの数
		std::uint32_t numVertex;	//頂点数
		std::uint32_t numIndex;		//インデックスの数
	};

	// 読み込んだ頂点
	struct MD_LoadVertex {
		float pos[3];		// 頂点座標
		float normal[3];	// 法線
		float tangent[3];	// 接ベクトル
		float binormal[3];	// 従ベクトル
		float uv[2];		// UV座標
	};
};

//=============================================================================
// ディレクトリパスを取得
//=============================================================================
std::wstring GetDirectoryPath(const std::wstring& origin){
	fs::path p = origin.c_str();
	return p.remove_filename().c_str();
}

//=============================================================================
// ファイル名を取得
//=============================================================================
std::wstring GetFileName(const std::wstring origin) {
	fs::path p = origin.c_str();
	return p.filename().c_str();
}

//=============================================================================
// aiStringをwstringに変換
//=============================================================================
std::wstring ConvertToWStr(const aiString& path){
	wchar_t temp[256] = {};
	size_t  size;
	mbstowcs_s(&size, temp, path.C_Str(), 256);
	return std::wstring(temp);
}

//=============================================================================
// wstringをstringに変換
//=============================================================================
std::string ToUTF8(const std::wstring& value){
	auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
	auto buffer = new char[length];

	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

	std::string result(buffer);
	delete[] buffer;
	buffer = nullptr;

	return result;
}

//=============================================================================
// std::string(マルチバイト文字列)からstd::wstring(ワイド文字列)を得る
//=============================================================================
std::wstring ToWideString(const std::string& str){
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}

//=============================================================================
// .mdファイルにコンバートする
//=============================================================================
void AssimpConversion::ComvertToMD(ComvertSettings& setting){
	
	clock_t start = clock();

	//----------------------------
	// Assimpでモデルデータをロード
	//----------------------------
	//!{
	if (setting.loadFilePath.size() == 0) {
		return;
	}

	auto inverseU = setting.inverseU;
	auto inverseV = setting.inverseV;

	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	auto scene = importer.ReadFile(setting.loadFilePath, flag);

	if (scene == nullptr) {
		// もし読み込みエラーがでたら表示する
		printf(importer.GetErrorString());
		printf("\n");
		return;
	}
	//!} 

	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	std::cout << "required time [assimp]:" << time * 1000 << "ms" << std::endl;

	//----------------------------
	// .mdにコンバート
	//----------------------------
	//!{
	FILE* outputfile;	// 出力ストリーム
	outputfile = fopen(setting.exportFileName.c_str(), "wb");  // ファイルを書き込み用にオープン
	
	// ヘッダーを書き込む
	mdFileFormat::MD_Header header;
	header.version = mdFileFormat::VERSION;
	header.numMeshes = scene->mNumMeshes;
	fwrite(&header, sizeof(mdFileFormat::MD_Header), 1, outputfile);

	// メッシュ単位で書き出し
	for (auto i = 0; i < scene->mNumMeshes; i++) {

		// 書き出すメッシュを取得
		auto mesh = scene->mMeshes[i];

		// メッシュ用のヘッダーを書き出し
		mdFileFormat::MD_UnitMeshHeader meshHeader;
		meshHeader.numMaterial = 5; // ディフューズ,メタルネス,ラフネス,法線,ハイトの５つ：拡張変更可能 
		meshHeader.numVertex = mesh->mNumVertices; // 頂点数
		meshHeader.numIndex = mesh->mNumFaces * 3; // 面１つにつきインデックスは3つ
		fwrite(&meshHeader, sizeof(mdFileFormat::MD_UnitMeshHeader), 1, outputfile);
		
		//----------------------------
		// マテリアルデータを書き出す
		//----------------------------
		//!{
		auto fwriteMaterial = [&](
			std::string& fileName,
			FILE* writeFile
		){
			// 文字列の長さを取得
			size_t length = fileName.length();
			fwrite(&length, sizeof(size_t), 1, writeFile);
			
			//文字列を1文字ずつ書き出し
			for (size_t i = 0; i < length; i++) {
				char tmp = fileName.at(i);
				fwrite(&tmp, sizeof(char), 1, outputfile);
			}

		};
		
		fwriteMaterial(setting.pExpandMaterial->AlbedMapFileName, outputfile);
		fwriteMaterial(setting.pExpandMaterial->MetalnessMapFileName, outputfile);
		fwriteMaterial(setting.pExpandMaterial->RoughnessMapFileName, outputfile);
		fwriteMaterial(setting.pExpandMaterial->NormalMapFileName, outputfile);
		fwriteMaterial(setting.pExpandMaterial->HeightMapFileName, outputfile);

		//!}
		//----------------------------
		// 頂点情報を書き出す
		//----------------------------
		//!{
		
		// 頂点単位で書き出す
		for (auto j = 0; j < meshHeader.numVertex; j++) {
			aiVector3D zero3D(0.f, 0.f, 0.f);

			// 要素を取得
			auto pPosition = &(mesh->mVertices[i]);
			auto pNormal = &(mesh->mNormals[i]);
			auto pTexCoord = (mesh->HasTextureCoords(0)) ? &(mesh->mTextureCoords[0][i]) : &zero3D;
			auto pTangent = (mesh->HasTangentsAndBitangents()) ? &(mesh->mTangents[i]) : &zero3D;
			auto pBiTangent = (mesh->HasTangentsAndBitangents()) ? &(mesh->mBitangents[i]) : &zero3D;

			// 書き出す用の頂点
			mdFileFormat::MD_LoadVertex dstVertex;
			
			// 座標
			dstVertex.pos[0] = pPosition->x;
			dstVertex.pos[1] = pPosition->y;
			dstVertex.pos[2] = pPosition->z;

			// 法線
			dstVertex.normal[0] = pNormal->x;
			dstVertex.normal[1] = pNormal->y;
			dstVertex.normal[2] = pNormal->z;

			// UV
			dstVertex.uv[0] = pTexCoord->x;
			dstVertex.uv[1] = pTexCoord->y;

			// 接ベクトル
			dstVertex.tangent[0] = pTangent->x;
			dstVertex.tangent[1] = pTangent->y;
			dstVertex.tangent[2] = pTangent->z;

			// 従ベクトル
			dstVertex.binormal[0] = pBiTangent->x;
			dstVertex.binormal[1] = pBiTangent->y;
			dstVertex.binormal[2] = pBiTangent->z;

			fwrite(&dstVertex, sizeof(mdFileFormat::MD_LoadVertex), 1, outputfile);

		}

		//!} 
		//----------------------------
		// インデックスを書き出す
		//----------------------------
		//!{
		for (auto j = 0; j < mesh->mNumFaces; j++) {
			auto face = mesh->mFaces[j];
			assert(face.mNumIndices == 3); // フェイス単位のインデックス数は3つでないといけない

			for (auto k = 0; k < face.mNumIndices; k++) {
				uint32_t dstIndex = face.mIndices[k];
				fwrite(&dstIndex, sizeof(uint32_t), 1, outputfile);
			}
		}
		//!} 
	}

	fclose(outputfile);// 閉じる

	//!} 

}

//=============================================================================
// Assimpのマテリアルを解析
//=============================================================================
void AssimpConversion::ParseMaterial(MD_Material& dst, aiMaterial& assimpMaterial){
	// アルベドマップ
	{
		aiString path;
		if (assimpMaterial.Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS){
			auto fileName = GetFileName(ConvertToWStr(path));
			dst.AlbedMapFileName = ToUTF8(fileName);
		}
		else{
			dst.AlbedMapFileName.clear();
		}
	}

	// メタルネスマップ：光沢
	{
		aiString path;
		if (assimpMaterial.Get(AI_MATKEY_TEXTURE_SPECULAR(0), path) == AI_SUCCESS) {
			auto fileName = GetFileName(ConvertToWStr(path));
			dst.AlbedMapFileName = ToUTF8(fileName);
		}
		else {
			dst.MetalnessMapFileName.clear();
		}
	}
	
	// ラフネスマップ：面の粗さ
	{
		aiString path;
		if (assimpMaterial.Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS) {
			auto fileName = GetFileName(ConvertToWStr(path));
			dst.AlbedMapFileName = ToUTF8(fileName);
		}
		else {
			dst.RoughnessMapFileName.clear();
		}
	}

}

