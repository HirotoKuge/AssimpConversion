/*****************************************************************//**
 * \file   AssimpConversion.h
 * \brief  Assimp�œǂݍ��񂾃f�[�^��.md�ɃR���o�[�g����N���X
 * 
 * \author Hiroto Kuge
 * \date   March 2023
 *********************************************************************/

#pragma once

#define NOMINMAX
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>



//=============================================================================
// MD_Material 
//=============================================================================
struct MD_Material{
	std::string AlbedMapFileName = "";		// �f�B�t���[�Y�}�b�v�t�@�C���p�X
	std::string MetalnessMapFileName = "";	// ���^���l�X�}�b�v�t�@�C���p�X
	std::string RoughnessMapFileName = "";	// ���t�l�X�}�b�v�t�@�C���p�X
	std::string NormalMapFileName = "";		// �@���}�b�v�t�@�C���p�X
	std::string HeightMapFileName = "";		// �n�C�g�}�b�v�p�X	
};

//=============================================================================
// LoadSettings�FAssimp�Ń��f���f�[�^��ǂݍ��ލۂ̐ݒ�
//=============================================================================
struct ComvertSettings {
	std::string loadFilePath = nullptr; // �t�@�C���p�X
	std::string exportFileName = nullptr; // �o�͂���t�@�C����
	bool inverseU = false; // U���W�𔽓]�����邩
    bool inverseV = false; // V���W�𔽓]�����邩
	bool useExpandMaterial = false;			// ���[�U�[��`�̃}�e���A�����g�����ǂ���
	MD_Material* pExpandMaterial = nullptr;	// ���[�U�[��`�̃}�e���A��
};

//=============================================================================
// AssimpConversion Class
//=============================================================================
class AssimpConversion {
	//-----------------------------------------------------------------------------
	// list of friends classes and methods.
	//-----------------------------------------------------------------------------	
	//!{
			/* Nothing */
	//!} 
public:
	//-----------------------------------------------------------------------------
	// public variables.
	//-----------------------------------------------------------------------------
	//!{
			/* Nothing */
	//!} 
	//-----------------------------------------------------------------------------
	// public methods.
	//-----------------------------------------------------------------------------
	//!{
    /*****************************************************************//**
     * \brief .md�t�@�C���ɃR���o�[�g����
     * 
     * \param setting �ݒ�
     *********************************************************************/
    void ComvertToMD(ComvertSettings& setting);

	//!} 
private:
	//-----------------------------------------------------------------------------
	// private variables.
	//-----------------------------------------------------------------------------
	//!{
	//!}
	//-----------------------------------------------------------------------------
	// private methods.
	//----------------------------------------------------------------------------- 
	//!{
   
	/*****************************************************************//**
	 * \brief Assimp�̃}�e���A�������
	 * 
	 * \param dst			 ��̓f�[�^�̊i�[��
	 * \param assimpMaterial Assimp�}�e���A���f�[�^
	 *********************************************************************/
	void ParseMaterial(MD_Material& dst, aiMaterial& assimpMaterial);
	
	
	//!} 
};
