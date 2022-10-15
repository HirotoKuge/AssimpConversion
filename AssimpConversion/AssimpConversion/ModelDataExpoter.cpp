#include <stdio.h> 
#include <stdlib.h>
#include<iostream>
#include "ModelDataExpoter.h"

bool ModelDataExpoter::ExportModelData(std::vector<Mesh>& meshes) {
	FILE* outputfile;	// �o�̓X�g���[��
	outputfile = fopen(exportFilePath.c_str(), "wb");  // �t�@�C�����������ݗp�ɃI�[�v��(�J��)

	if (outputfile == NULL) {
		std::cout << "file open is failed!" << std::endl;
		return false;
	}

	std::cout << "start export modeldata" << std::endl;

	//�ǂݍ��݂ɕK�v�ȏ����\���̂ɂ܂Ƃ߂ď����o��
	int meshNum = meshes.size();
	
	//���b�V���̐�
	fwrite(&meshNum, sizeof(int), 1, outputfile);
	
	std::cout << "export number of meshes complete" << std::endl;


	//���b�V���̐������o�͏�������
	for (int i = 0; i < meshNum; i++) {


		std::cout << "meshNo " << i << " export start" << std::endl;

		//(�������b�V������Ȃ郂�f���ɂ��Ή��ł���p��)�����o���Ώۂ̃��b�V�������o��
		auto exportMesh = meshes.at(i);

		//���_���擾
		int vertexNum = exportMesh.Vertices.size();

		//�C���f�b�N�X���擾
		int indexNum = exportMesh.Indices.size();

		//���b�V���Ή��̃J���[�}�b�v�p�X�擾
		auto diffusemap = exportMesh.DiffuseMap;


		//���_����������
		fwrite(&vertexNum, sizeof(int), 1, outputfile);

		std::cout << "export number of vertices complete" << std::endl;


		//���_���������o��
		for (int j = 0; j < vertexNum; j++) {
			auto writeVertex = meshes[i].Vertices[j];
			auto n = fwrite(&writeVertex, sizeof(Vertex), 1, outputfile);
		}

		std::cout << "export vertex data complete" << std::endl;


		//�C���f�b�N�X����������
		fwrite(&indexNum, sizeof(int), 1, outputfile);

		std::cout << "export number of indices complete" << std::endl;


		//�C���f�b�N�X��������
		for (int j = 0; j < indexNum; j++) {
			int writeIndex = meshes[i].Indices[j];
			auto n = fwrite(&writeIndex, sizeof(int), 1, outputfile);
		}

		std::cout << "export index dara complete" << std::endl;


		//diffusemap�̃t�@�C���p�X�̕��������擾���ď����o��
		int wordLength = diffusemap.length();
		fwrite(&wordLength, sizeof(int), 1, outputfile);

		std::cout << "export diffusemap file path length complete" << std::endl;


		//diffusemap�̃t�@�C���p�X�����o��
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
