#pragma once
#include "stdafx.h"
#include "Obj.h"

class Gun : public Obj {
public:
	Gun(int x, int	y, bool can_collision, const Sprite& sprite0) 
		: Obj(x, y, can_collision, sprite0) {};
	void gunnt_change() {
		SetDrawScreen(DX_SCREEN_BACK);  // �\����ʂ𗠂�
		ClearDrawScreen();  // ��ʑS�̂��N���A
		int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1); //�v����(�����p�ƍ��킹�Ĉ�K�̃Q�[�����[�v�̍ŏ��Ŏ󂯎���Ă���������)
		if ((PADInput & PAD_INPUT_DOWN) != 0) { //�\�����L�[�̓���
			y = y + 3;
		}

		if ((PADInput & PAD_INPUT_UP) != 0) { //�\����L�[�̓���
			y = y - 3;
		}
	}
};