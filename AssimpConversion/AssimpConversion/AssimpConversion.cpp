#include "AssimpConversion.h"
#include "Vector.h"
#include <filesystem>
#include <time.h>
#include <iostream>

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
// MD_Vertex:頂点データの構造体
//=============================================================================
struct MD_Vertex {
	Vector3 pos;			//座標
	Vector3 normal;			//法線
	Vector3 tangent;		//接ベクトル
	Vector3 binormal;		//従ベクトル
	Vector2 uv;				//UV座標
};

//=============================================================================
// MD_UnitMesh:単位メッシュ
//=============================================================================
struct MD_UnitMesh {
	std::vector<MD_Material>	materials;	// マテリアル
	std::vector<MD_Vertex>		vertices;	// 頂点
	std::vector<uint32_t>		indecies;	// インデックス
};


std::vector<MD_UnitMesh> g_meshes; //メッシュパーツ

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
		meshHeader.numMaterial = 1; // 複数マテリアルも対応できけど基本１
		meshHeader.numVertex = mesh->mNumVertices; // 頂点数
		meshHeader.numIndex = mesh->mNumFaces * 3; // 面１つにつきインデックスは3つ(のはず)
		fwrite(&meshHeader, sizeof(mdFileFormat::MD_UnitMeshHeader), 1, outputfile);
		
		//----------------------------
		// マテリアルデータを書き出す
		//----------------------------
		//!{
		
		/*****************************************************************//**
		 * \brief テクスチャのファイル名を書き出す
		 * 
		 * \param fileName	書き出すデータ：ファイル名(ファイルパスじゃないよ)
		 * \param writeFile 書き出し先
		 *********************************************************************/
		auto fwriteTextureFileName = [&](
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
		
		// ユーザー拡張のマテリアルを使う
		// TODO：メッシュ単位でマテリアルを設定する場合に対応できないので何とかする
		if (setting.useExpandMaterial) {
			fwriteTextureFileName(setting.pExpandMaterial->AlbedMapFileName, outputfile);
			fwriteTextureFileName(setting.pExpandMaterial->MetalnessMapFileName, outputfile);
			fwriteTextureFileName(setting.pExpandMaterial->RoughnessMapFileName, outputfile);
			fwriteTextureFileName(setting.pExpandMaterial->NormalMapFileName, outputfile);
			fwriteTextureFileName(setting.pExpandMaterial->HeightMapFileName, outputfile);
		}
		// AssimpSceneからマテリアルを解析して書き出す
		else {
			// メッシュに対応するマテリアルを取り出す
			auto assimpMaterial = scene->mMaterials[mesh->mMaterialIndex];
			
			// 解析データの格納先
			MD_Material parsedMaterial;
			
			// 解析
			ParseMaterial(parsedMaterial, assimpMaterial);

			fwriteTextureFileName(parsedMaterial.AlbedMapFileName, outputfile);
			fwriteTextureFileName(parsedMaterial.MetalnessMapFileName, outputfile);
			fwriteTextureFileName(parsedMaterial.RoughnessMapFileName, outputfile);
			fwriteTextureFileName(parsedMaterial.NormalMapFileName, outputfile);
			fwriteTextureFileName(parsedMaterial.HeightMapFileName, outputfile);

		}

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
			auto& face = mesh->mFaces[j];
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
// 読み込みテスト
//=============================================================================
void AssimpConversion::LoadTest(std::string loadFilePath){
	// ファイルを開けるで
	FILE* pLoadFile = fopen(loadFilePath.c_str(), "rb");
	if (pLoadFile == nullptr) {
		MessageBoxA(nullptr, "mdファイルが開けません", "エラー", MB_OK);
		return;
	}

	//----------------------------
	// mdファイルのヘッダーを読み込み
	//----------------------------
	//!{
	mdFileFormat::MD_Header header;
	fread(&header, sizeof(header), 1, pLoadFile);
	if (header.version != mdFileFormat::VERSION) {
		//mdファイルのバージョンが違う
		MessageBoxA(nullptr, "mdファイルのバージョンが異なっています", "エラー", MB_OK);
	}
	//!}
	//----------------------------
	// メッシュ単位で読みだす
	//----------------------------
	//!{
	
	// メッシュサイズ分確保 
	g_meshes.resize(header.numMeshes);

	// メッシュ単位でループ
	for (auto i = 0; i < header.numMeshes; i++) {
		// 格納先の設定
		auto& mesh = g_meshes[i];
		
		//----------------------------
		// 単位メッシュのヘッダー読み出し
		//----------------------------
		//!{
		mdFileFormat::MD_UnitMeshHeader meshHeader;
		fread(&meshHeader, sizeof(mdFileFormat::MD_UnitMeshHeader), 1, pLoadFile);
		//!} 
		//----------------------------
		// マテリアルデータの読み出し
		//----------------------------
		//!{
		
		// マテリアルの数分領域を確保
		mesh.materials.resize(meshHeader.numMaterial);
		
		// 読み出し
		for (auto j = 0; j < meshHeader.numMaterial; j++) {
			// 書き出し先の設定
			auto& material = mesh.materials[j];

			/*****************************************************************//**
			 * \brief マテリアル読み出し関数
			 * 
			 * \param fp ファイルポインタ
			 * \param dst 格納先
			 *********************************************************************/
			auto LoadTextureFileName = [&](FILE* fp,std::string& dst){
				dst.clear();

				// 文字列の長さを取得
				size_t fileNameLength = 0;
				fread(&fileNameLength, sizeof(size_t), 1, pLoadFile);

				// １文字ずつ読みだす
				for (auto k = 0; k < fileNameLength; k++) {
					char tmp;
					fread(&tmp, sizeof(char), 1, fp);
					dst += tmp;
				}

			};

			LoadTextureFileName(pLoadFile, material.AlbedMapFileName);
			LoadTextureFileName(pLoadFile, material.MetalnessMapFileName);
			LoadTextureFileName(pLoadFile, material.RoughnessMapFileName);
			LoadTextureFileName(pLoadFile, material.NormalMapFileName);
			LoadTextureFileName(pLoadFile, material.HeightMapFileName);

		}
		//!} 
		//----------------------------
		// 頂点データの読み出し
		//----------------------------
		//!{
		
		// 領域確保
		mesh.vertices.resize(meshHeader.numVertex);

		for (auto j = 0; j < meshHeader.numVertex; j++) {
			// 1頂点ごとに読みだす
			mdFileFormat::MD_LoadVertex vertexTmp;
			fread(&vertexTmp, sizeof(vertexTmp), 1, pLoadFile);

			// 格納
			auto& vertex = mesh.vertices[j];
			vertex.pos.Set(vertexTmp.pos[0], vertexTmp.pos[1], vertexTmp.pos[2]);
			vertex.normal.Set(vertexTmp.normal[0], vertexTmp.normal[1], vertexTmp.normal[2]);
			vertex.uv.Set(vertexTmp.uv[0], vertexTmp.uv[1]);
			vertex.tangent.Set(vertexTmp.tangent[0], vertexTmp.tangent[1], vertexTmp.tangent[2]);
			vertex.binormal.Set(vertexTmp.binormal[0], vertexTmp.binormal[1], vertexTmp.binormal[2]);
		}
		
		//!}
		//----------------------------
		// インデックスの読み出し
		//----------------------------
		//!{ 
		
		// 領域確保
		mesh.indecies.resize(meshHeader.numIndex);
		
		// 読み出し
		for (auto j = 0; j < meshHeader.numIndex; j++) {
			uint32_t index;
			fread(&index, sizeof(uint32_t), 1, pLoadFile);
			mesh.indecies[j] = index;
		}
		
		//!}  
	}
	//!} 
	

	fclose(pLoadFile);

	
}

//=============================================================================
// Assimpのマテリアルを解析
//=============================================================================
void AssimpConversion::ParseMaterial(MD_Material& dst, aiMaterial* assimpMaterial){
	// アルベドマップ
	{
		aiString path;
		if (assimpMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS){
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
		if (assimpMaterial->Get(AI_MATKEY_TEXTURE_SPECULAR(0), path) == AI_SUCCESS) {
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
		if (assimpMaterial->Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS) {
			auto fileName = GetFileName(ConvertToWStr(path));
			dst.AlbedMapFileName = ToUTF8(fileName);
		}
		else {
			dst.RoughnessMapFileName.clear();
		}
	}

	// 法線マップ
	{
		aiString path;
		if (assimpMaterial->Get(AI_MATKEY_TEXTURE_NORMALS(0), path) == AI_SUCCESS) {
			auto fileName = GetFileName(ConvertToWStr(path));
			dst.NormalMapFileName = ToUTF8(fileName);
		}
		else {
			dst.NormalMapFileName.clear();
		}
	}


	// ハイトマップ
	{
		aiString path;
		if (assimpMaterial->Get(AI_MATKEY_TEXTURE_HEIGHT(0), path) == AI_SUCCESS) {
			auto fileName = GetFileName(ConvertToWStr(path));
			dst.HeightMapFileName = ToUTF8(fileName);
		}
		else {
			dst.HeightMapFileName.clear();
		}
	}
}

