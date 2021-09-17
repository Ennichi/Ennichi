#pragma once
#include "stdafx.h"

class KeyInput
{
	using uchar = unsigned char;
private:
	/* �O���Q�ƕs�� */
	std::vector<uchar> Pressed;
	std::vector<uchar> prev_Pressed;
	std::vector<uchar> Target_key;
	char* __KeyState;
public:
	/* �O���Q�Ɖ� */

	//�f�t�H���g�R���X�g���N�^(�L�[�w�薳��)
	KeyInput() : Pressed{}, prev_Pressed{}, Target_key{}
	{
		__KeyState = new char[256];
	}

	//�R���X�g���N�^(�L�[��w��)
	KeyInput(uchar target) : Pressed{}, prev_Pressed{}, Target_key(1, target)
	{
		__KeyState = new char[256];
	}

	//�R���X�g���N�^(�C�j�V�����C�U�[���X�g)
	KeyInput(std::initializer_list<uchar> target
	) : Pressed{}, prev_Pressed{}, Target_key{}
	{
		__KeyState = new char[256];
		for (auto temp : target)
		{
			Target_key.push_back(temp);
		}
	}

	//�R���X�g���N�^(vector�œn��)(�قڎg��Ȃ�)
	KeyInput(const std::vector<uchar>& target) : Pressed{}, Target_key{target}
	{
		__KeyState = new char[256];
	}

	virtual ~KeyInput()
	{
		delete[] __KeyState;
	}

	//�ΏۃL�[��ǉ�
	void add(uchar KeyCode)
	{
		Target_key.push_back(KeyCode);
	}

	//�L�[��������Ă��邩
	bool GetKey(uchar KeyCode)const&
	{
		for (auto __key : Pressed)
		{
			if (__key == KeyCode)return true;
		}
		return false;
	}

	//�L�[�������ꂽ��
	bool GetKeyDown(uchar KeyCode)const&
	{
		for (auto __key : Pressed)
		{
			if (__key == KeyCode)
			{
				for (auto __prev_key : prev_Pressed)
				{
					if (__prev_key == KeyCode)return false;
				}
				return true;
			}
		}
		return false;
	}

	//�L�[�������ꂽ��
	bool GetKeyUp(uchar KeyCode)const&
	{
		for (auto __key : Pressed)
		{
			if (__key == KeyCode)
			{
				return false;
			}
		}
		for (auto __prev_key : prev_Pressed)
		{
			if (__prev_key == KeyCode)
			{
				return true;
			}
		}
		return false;
	}

	//���[�v�̍ŏ��Ɏ��s
	void operator()()
	{
		if (GetHitKeyStateAll(__KeyState) == -1)
			throw new std::runtime_error("�L�[������Ԏ擾���s");
		prev_Pressed = Pressed;
		Pressed.clear();
		for (auto __target : Target_key)
		{
			if (__KeyState[__target] == 1)
			{
				Pressed.push_back(__target);
			}
		}
	}

};
