#pragma once
#include "stdafx.h"
/* �ėp�֐��Ȃǂ܂Ƃ� */

/*�x�N�g���\����
* �g��Ȃ�����������܂�
*/
struct Vector2
{
	double x = 0.0;
	double y = 0.0;

	//���̃R���X�g���N�^
	Vector2()
	{}

	//�R���X�g���N�^
	Vector2(double x0, double y0)
	{
		x = x0;
		y = y0;
	}

	/*
	* �R���X�g���N�^(�C�j�V�����C�U�[���X�g)
	* �g����
	* Vector2 a = {2.0, 3.0};
	*/
	Vector2(std::initializer_list<double> list
	) :x{ *list.begin() }, y{ *(list.begin() + 1) }
	{}

	/*
	* ������Z�q��`(�C�j�V�����C�U�[���X�g)
	* �g����
	* Vector2 a;
	* a = {3.0, 2.5}
	*/
	Vector2& operator=(std::initializer_list<double> list)
	{
		x = *list.begin();
		y = *(list.begin() + 1);
		return *this;
	}
};


inline double distance(int x1, int y1, std::vector<int> pos2)
{
	return std::sqrt((x1 - pos2[0]) * (x1 - pos2[0]) + (y1 - pos2[1]) * (y1 - pos2[1]));
}

