#pragma once
#include<string>
#include "AssimpLoader.h"
#include "SharedStruct.h"




class ModelDataExpoter{
public:
	bool ExportModelData(std::vector<Mesh>&);//���f���f�[�^�̃G�N�X�|�[�g����
	void SetFilePath(std::string);//�t�@�C���p�X�ݒ�p�֐�

private:
	std::string exportFilePath = "template.md";//�o�͂���t�@�C���p�X
};

