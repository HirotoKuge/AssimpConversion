#include "MDLoader.h"
#include <time.h>
#include<iostream>


//.md�t�@�C���ɂ���
//.md�t�@�C���̓��f���f�[�^���o�C�i���œƎ��`���ɏo�͂�������
//���ߗp�Ƀf�[�^�̊i�[�\�����ȉ��ɋL��
//���b�V������
//�i�ȉ����b�V�����Ƃ�)
//���_��
//���_���(Vertex�\���̂Ɠ������e)
//�C���f�b�N�X��
//�C���f�b�N�X
//�f�B�t���[�Y�}�b�v�̃p�X�̕�����
//�f�B�t���[�Y�}�b�v�̃p�X(1�������i�[)


bool MDLoader::LoadMD(std::vector<Mesh>& meshes, std::string filepath){
	clock_t start = clock();
	//�t�@�C���̓z
	FILE* fp = nullptr;

	//�ǂ݂���md�t�@�C����ǂݎ���p�ŃI�[�v��
	fp = fopen(filepath.c_str(), "rb");


	//���b�V������ǂݎ��
	int meshNum = 0;
	fread(&meshNum, sizeof(int), 1, fp);

	//���b�V����������
	for (int i = 0; i < meshNum; i++) {
		Mesh toHouseMesh;//�i�[�p�̃��b�V��
		
		//���_���ǂݎ��
		int vertexNum = 0;
		fread(&vertexNum, sizeof(int), 1, fp);

		//���_�f�[�^�ǂ݂����Ċi�[
		for (int j = 0; j < vertexNum; j++) {
			Vertex tmp;
			fread(&tmp, sizeof(Vertex), 1, fp);
			toHouseMesh.Vertices.emplace_back(tmp);
		}

		//�C���f�b�N�X���ǂݎ��
		int indexNum = 0;
		fread(&indexNum, sizeof(int), 1, fp);

		//�C���f�b�N�X�ǂ݂����Ċi�[
		for (int j = 0; j < indexNum; j++) {
			uint32_t tmp;
			fread(&tmp, sizeof(int), 1, fp);
			toHouseMesh.Indices.emplace_back(tmp);
		}

		//�t�@�C���p�X�̕������ǂݎ��
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
