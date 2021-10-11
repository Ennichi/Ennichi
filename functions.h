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
	Vector2(double x0, double y0) : x{x0}, y{y0}
	{}

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


inline double distance(double dx, double dy)
{
	return std::sqrt(dx * dx + dy * dy);
}

double dist_elipse(int x1, int y1, int x2, int y2, double magx, double magy);

bool file_exists(const std::string&);

template<typename... strArgs>
void makeImageHandle(std::vector<int>& vHandle, const strArgs... pathArgs)
{
	std::vector<const char*> path_v = { pathArgs... };
	unsigned int vsize = static_cast<unsigned int>(vHandle.size());
	if (vsize > path_v.size())
	{
		vHandle.erase(vHandle.begin() + path_v.size() - 1ull, vHandle.end());
	}
	std::vector<int>::iterator itr = vHandle.begin(), itrend = vHandle.end();
	for (auto tmp : path_v)
	{
		if (itr != itrend)
		{
			*itr = LoadGraph(tmp);
			if (*itr == -1)
			{
				throw new std::runtime_error("�摜�t�@�C��" + static_cast<std::string>(tmp) + "���ǂݍ��߂܂���ł���");
			}
			++itr;
		}
		else
		{
			vHandle.push_back(LoadGraph(tmp));
			if (*(vHandle.end() - 1) == -1)
			{
				throw new std::runtime_error("�摜�t�@�C��" + static_cast<std::string>(tmp) + "���ǂݍ��߂܂���ł���");
			}
		}
	}

}