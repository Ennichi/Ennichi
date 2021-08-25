#pragma once
#include "stdafx.h"


class Obj
{
public:
	/* �����o�ϐ� */
	int x, y; // �I�u�W�F�N�g�̍��W
	int xlength=0, ylength=0; // x, y�����̒���
	int state = 0; // �`�悷��摜�́Aimage�ɂ�����Y����

	double angle = 0.0; // �摜�̉�]

	bool can_collision; // ���̃I�u�W�F�N�g�ƏՓ˂��邩�ǂ���

	std::vector<int> images; // ���[�h�����摜(�n���h��)�̃��X�g


	
	/* �����o�֐� */
	Obj( // �R���X�g���N�^
		int x,
		int	y,
		bool can_collision,
		const std::vector<const char*>& image_path
	) : x(x), y(y), can_collision(can_collision)
	{
		/* image_path�Ŏw�肳�ꂽ�摜��ǂݍ���, images�ɕۑ� */
		for (int i = 0; i < (int)image_path.size(); i++) {
			int t = LoadGraph(image_path[i]); // �w��摜��ǂݍ���
			if (t == -1) {
				exit(1);
			}
			images.push_back(t); // images�ɒǉ�
			GetGraphSize(images[0], &xlength, &ylength);
		}
	}

	Obj( // �R���X�g���N�^(�p�x�w�肠��)
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<const char*>& image_path
	) : x(x), y(y), angle(angle), can_collision(can_collision)
	{
		/* image_path�Ŏw�肳�ꂽ�摜��ǂݍ���, images�ɕۑ� */
		for (int i = 0; i < (int)image_path.size(); i++) {
			int t = LoadGraph(image_path[i], 1); // �w��摜��ǂݍ���
			if (t == -1) {
				exit(1);
			}
			images.push_back(t); // images�ɒǉ�
			GetGraphSize(images[0], &xlength, &ylength);
		}
	}


	virtual ~Obj() {
		for (int i = 0; i < (int)images.size(); i++) {
			DeleteGraph(images[i]); // �ǂݍ��񂾉摜������������폜
		}
	}

	virtual void draw()
	{
		/* �I�u�W�F�N�g����ʂɔ��f���� */
		if (DrawRotaGraph(x + xlength / 2, y + ylength / 2, 1.0, angle, images[state], 1) == -1) {
			throw new std::runtime_error("�`�掸�s");
			exit(1);
		}
	}

	virtual bool isCollision(Obj& other) {
		if (!(can_collision && other.can_collision)) return false; // �ǂ��炩���Փ˕s��
		int dx = x - other.x, dy = y - other.y;//*this�̍��W�ɑ΂���other�̑��΍��W
		if ((dx < xlength && dx > -other.xlength) && (dy < ylength && dy > -other.ylength))return true;
		return false;
	}
};

