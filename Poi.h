#pragma once
#include "stdafx.h"
#include "Obj.h"

//�|�C�̃N���X�̒�`(Obj�̌p��)
class Poi : public Obj {
public:
	Poi(int x, int	y, bool can_collision, const Sprite& sprite0) : Obj(x, y, can_collision, sprite0) {};
	//Obj�̊p�x�Ȃ��R���X�g���N�^�Ăяo��
	//�摜�T�C�Y��400�~400�̎��̂Ƃ��A����ɑ΂��钆�S�̍��W��(240,160)
	inline std::vector<int> center()const&
	{
		std::vector<int> pos = {x + 240, y + 160};
		return pos;
	}

	void point_change() {
		int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1); //�v����(�����p�ƍ��킹�Ĉ�K�̃Q�[�����[�v�̍ŏ��Ŏ󂯎���Ă���������)

		if ((PADInput & PAD_INPUT_RIGHT) != 0) { //�\���E�L�[�̓���
			x = x + 3;
		}

		if ((PADInput & PAD_INPUT_LEFT) != 0) { //�\�����L�[�̓���
			x = x - 3;
		}

		if ((PADInput & PAD_INPUT_DOWN) != 0) { //�\�����L�[�̓���
			y = y + 3;
		}

		if ((PADInput & PAD_INPUT_UP) != 0) { //�\����L�[�̓���
			y = y - 3;
		}

	}
};






