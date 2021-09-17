#pragma once
#include "stdafx.h"


class Obj
{
protected:
	/* �h���N���X�̂݃A�N�Z�X�\ */
	unsigned int __frames = 0;

public:
	/* �����o�ϐ� */
	int x, y; // �I�u�W�F�N�g�̍��W
	int xlength=0, ylength=0; // x, y�����̒���
	int state = 0; // �`�悷��摜�́Aimage�ɂ�����Y����

	unsigned int animsp = 0;//�A�j���[�V�����̃R�}����̑��x(���t���[���Ɉ��̑��x��)(0�Ȃ��~)

	double angle = 0.0; // �摜�̉�]

	bool can_collision; // ���̃I�u�W�F�N�g�ƏՓ˂��邩�ǂ���

	std::vector<int> images; // ���[�h�����摜(�n���h��)�̃��X�g


	
	/* �����o�֐� */
	Obj( // �R���X�g���N�^
		int x,
		int	y,
		bool can_collision,
		const std::vector<int>& image_handle
	) : x(x), y(y), can_collision(can_collision),images(image_handle)
	{
		if(!images.empty())GetGraphSize(images[0], &xlength, &ylength);
	}

	Obj( // �R���X�g���N�^(�p�x�w�肠��)
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<int>& image_handle
	) : x(x), y(y), angle(angle), can_collision(can_collision),images(image_handle)
	{
		if(!images.empty())GetGraphSize(images[0], &xlength, &ylength);
	}

	//�摜�����R���X�g���N�^
	Obj(
		int x,
		int y
	) : x(x), y(y), can_collision(false), images()
	{}

	//�R�s�[�R���X�g���N�^��������
	Obj(const Obj&) = default;

	//���[�u�R���X�g���N�^����
	Obj(Obj&&) = delete;

	virtual ~Obj() {

	}

	virtual void draw()
	{
		/* �I�u�W�F�N�g����ʂɔ��f���� */
		if (DrawRotaGraph(x + xlength / 2, y + ylength / 2, 1.0, angle, images[state], 1) == -1) {
			throw new std::runtime_error("�`�掸�s");
			exit(1);
		}
		if (animsp != 0)
		{
			__frames++;
			if (__frames >= animsp)
			{
				__frames = 0;
				state = (state + 1) % static_cast<int>(images.size());
			}
		}
	}

	virtual bool isCollision(Obj& other) {
		if (!(can_collision && other.can_collision)) return false; // �ǂ��炩���Փ˕s��
		int dx = x - other.x, dy = y - other.y;//*this�̍��W�ɑ΂���other�̑��΍��W
		if ((dx < xlength && dx > -other.xlength) && (dy < ylength && dy > -other.ylength))return true;
		return false;
	}
};

