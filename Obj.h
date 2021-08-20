#pragma once
#include "stdafx.h"
#include "Sprite.h"

class Obj
{
public:
	/* �����o�ϐ� */
	int x, y; // �I�u�W�F�N�g�̍��W
	int xlength, ylength; // x, y�����̒���
	int state = 0; // �`�悷��摜�́Aimage�ɂ�����Y����

	double angle = 0.0; // �摜�̉�]

	bool can_collision; // ���̃I�u�W�F�N�g�ƏՓ˂��邩�ǂ���

	Sprite sprite;//�X�v���C�g

	
	/* �����o�֐� */
	Obj( // �R���X�g���N�^
		int x,
		int	y,
		bool can_collision,
		const Sprite& sprite0
	) : x(x), 
		y(y), 
		can_collision(can_collision), 
		sprite(sprite0), 
		xlength(sprite0.width), 
		ylength(sprite0.height)
	{}
	

	Obj( // �R���X�g���N�^(�p�x�w�肠��)
		int x,
		int	y,
		double angle,
		bool can_collision,
		const Sprite& sprite0
	) : x(x), 
		y(y), 
		angle(angle), 
		can_collision(can_collision),
		sprite(sprite0),
		xlength(sprite0.width),
		ylength(sprite0.height)
	{}



	virtual void draw()
	{
		/* �I�u�W�F�N�g����ʂɔ��f���� */
		if (DrawRotaGraph(x + xlength / 2, y + ylength / 2, 1.0, angle, sprite[state], 1) == -1) {
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

