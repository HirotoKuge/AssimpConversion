#include "AssimpConversion.h"
#include <filesystem>
#include<time.h>
#include<iostream>

namespace fs = std::filesystem;

//=============================================================================
// md�t�@�C���t�H�[�}�b�g
//=============================================================================
namespace mdFileFormat {
	// ���݂�md�t�@�C���̃o�[�W����
	std::uint16_t VERSION = 1;
	
	// �}�e���A���̐�

	// �S�̂̃w�b�_�[
	struct MD_Header {
		std::uint16_t version;	 //�o�[�W����
		std::uint16_t numMeshes; //���b�V����
	};

	// �P�ʃ��b�V���̃w�b�_�[
	struct MD_UnitMeshHeader {
		std::uint32_t numMaterial;	//�}�e���A���̐�
		std::uint32_t numVertex;	//���_��
		std::uint32_t numIndex;		//�C���f�b�N�X�̐�
	};

	// �ǂݍ��񂾒��_
	struct MD_LoadVertex {
		float pos[3];		// ���_���W
		float normal[3];	// �@��
		float tangent[3];	// �ڃx�N�g��
		float binormal[3];	// �]�x�N�g��
		float uv[2];		// UV���W
	};
};

//=============================================================================
// �f�B���N�g���p�X���擾
//=============================================================================
std::wstring GetDirectoryPath(const std::wstring& origin){
	fs::path p = origin.c_str();
	return p.remove_filename().c_str();
}

//=============================================================================
// �t�@�C�������擾
//=============================================================================
std::wstring GetFileName(const std::wstring origin) {
	fs::path p = origin.c_str();
	return p.filename().c_str();
}

//=============================================================================
// aiString��wstring�ɕϊ�
//=============================================================================
std::wstring ConvertToWStr(const aiString& path){
	wchar_t temp[256] = {};
	size_t  size;
	mbstowcs_s(&size, temp, path.C_Str(), 256);
	return std::wstring(temp);
}

//=============================================================================
// wstring��string�ɕϊ�
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
// std::string(�}���`�o�C�g������)����std::wstring(���C�h������)�𓾂�
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
// .md�t�@�C���ɃR���o�[�g����
//=============================================================================
void AssimpConversion::ComvertToMD(ComvertSettings& setting){
	
	clock_t start = clock();

	//----------------------------
	// Assimp�Ń��f���f�[�^�����[�h
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
		// �����ǂݍ��݃G���[���ł���\������
		printf(importer.GetErrorString());
		printf("\n");
		return;
	}
	//!} 

	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	std::cout << "required time [assimp]:" << time * 1000 << "ms" << std::endl;

	//----------------------------
	// .md�ɃR���o�[�g
	//----------------------------
	//!{
	FILE* outputfile;	// �o�̓X�g���[��
	outputfile = fopen(setting.exportFileName.c_str(), "wb");  // �t�@�C�����������ݗp�ɃI�[�v��
	
	// �w�b�_�[����������
	mdFileFormat::MD_Header header;
	header.version = mdFileFormat::VERSION;
	header.numMeshes = scene->mNumMeshes;
	fwrite(&header, sizeof(mdFileFormat::MD_Header), 1, outputfile);

	// ���b�V���P�ʂŏ����o��
	for (auto i = 0; i < scene->mNumMeshes; i++) {

		// �����o�����b�V�����擾
		auto mesh = scene->mMeshes[i];

		// ���b�V���p�̃w�b�_�[�������o��
		mdFileFormat::MD_UnitMeshHeader meshHeader;
		meshHeader.numMaterial = 5; // �f�B�t���[�Y,���^���l�X,���t�l�X,�@��,�n�C�g�̂T�F�g���ύX�\ 
		meshHeader.numVertex = mesh->mNumVertices; // ���_��
		meshHeader.numIndex = mesh->mNumFaces * 3; // �ʂP�ɂ��C���f�b�N�X��3��
		fwrite(&meshHeader, sizeof(mdFileFormat::MD_UnitMeshHeader), 1, outputfile);
		
		//----------------------------
		// �}�e���A���f�[�^�������o��
		//----------------------------
		//!{
		auto fwriteMaterial = [&](
			std::string& fileName,
			FILE* writeFile
		){
			// ������̒������擾
			size_t length = fileName.length();
			fwrite(&length, sizeof(size_t), 1, writeFile);
			
			//�������1�����������o��
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
		// ���_���������o��
		//----------------------------
		//!{
		
		// ���_�P�ʂŏ����o��
		for (auto j = 0; j < meshHeader.numVertex; j++) {
			aiVector3D zero3D(0.f, 0.f, 0.f);

			// �v�f���擾
			auto pPosition = &(mesh->mVertices[i]);
			auto pNormal = &(mesh->mNormals[i]);
			auto pTexCoord = (mesh->HasTextureCoords(0)) ? &(mesh->mTextureCoords[0][i]) : &zero3D;
			auto pTangent = (mesh->HasTangentsAndBitangents()) ? &(mesh->mTangents[i]) : &zero3D;
			auto pBiTangent = (mesh->HasTangentsAndBitangents()) ? &(mesh->mBitangents[i]) : &zero3D;

			// �����o���p�̒��_
			mdFileFormat::MD_LoadVertex dstVertex;
			
			// ���W
			dstVertex.pos[0] = pPosition->x;
			dstVertex.pos[1] = pPosition->y;
			dstVertex.pos[2] = pPosition->z;

			// �@��
			dstVertex.normal[0] = pNormal->x;
			dstVertex.normal[1] = pNormal->y;
			dstVertex.normal[2] = pNormal->z;

			// UV
			dstVertex.uv[0] = pTexCoord->x;
			dstVertex.uv[1] = pTexCoord->y;

			// �ڃx�N�g��
			dstVertex.tangent[0] = pTangent->x;
			dstVertex.tangent[1] = pTangent->y;
			dstVertex.tangent[2] = pTangent->z;

			// �]�x�N�g��
			dstVertex.binormal[0] = pBiTangent->x;
			dstVertex.binormal[1] = pBiTangent->y;
			dstVertex.binormal[2] = pBiTangent->z;

			fwrite(&dstVertex, sizeof(mdFileFormat::MD_LoadVertex), 1, outputfile);

		}

		//!} 
		//----------------------------
		// �C���f�b�N�X�������o��
		//----------------------------
		//!{
		for (auto j = 0; j < mesh->mNumFaces; j++) {
			auto face = mesh->mFaces[j];
			assert(face.mNumIndices == 3); // �t�F�C�X�P�ʂ̃C���f�b�N�X����3�łȂ��Ƃ����Ȃ�

			for (auto k = 0; k < face.mNumIndices; k++) {
				uint32_t dstIndex = face.mIndices[k];
				fwrite(&dstIndex, sizeof(uint32_t), 1, outputfile);
			}
		}
		//!} 
	}

	fclose(outputfile);// ����

	//!} 

}

//=============================================================================
// Assimp�̃}�e���A�������
//=============================================================================
void AssimpConversion::ParseMaterial(MD_Material& dst, aiMaterial& assimpMaterial){
	// �A���x�h�}�b�v
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

	// ���^���l�X�}�b�v�F����
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
	
	// ���t�l�X�}�b�v�F�ʂ̑e��
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

