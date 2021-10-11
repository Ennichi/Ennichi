#pragma once
#include "stdafx.h"
#include "Obj.h"

//�|�C�̃N���X�̒�`(Obj�̌p��)
class Poi : public Obj {
public:
	Poi(int x, int	y, bool can_collision, const std::vector<int>& image_handle) : Obj(x, y, can_collision, image_handle) {};
	//Obj�̊p�x�Ȃ��R���X�g���N�^�Ăяo��
	//�摜�T�C�Y��400�~400�̎��̂Ƃ��A����ɑ΂��钆�S�̍��W��(240,160)
	inline std::vector<int> center()const&
	{
		std::vector<int> pos = {x + 58, y + 34};
		return pos;
	}

	void point_change() {
		int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1); //�v����(�����p�ƍ��킹�Ĉ�K�̃Q�[�����[�v�̍ŏ��Ŏ󂯎���Ă���������)
		if (x > 1080) {
			x = 1080;
		}
		else if (x < 100) {
			x = 100;
		}
		else if (y > 500) {
			y = 500;
		}
		else if (y < 100) {
			y = 100;
		}
		else if ((PADInput & PAD_INPUT_RIGHT) != 0) { //�\���E�L�[�̓���
			x = x + 3;
		}

		else if ((PADInput & PAD_INPUT_LEFT) != 0) { //�\�����L�[�̓���
			x = x - 3;
		}

		else if ((PADInput & PAD_INPUT_DOWN) != 0) { //�\�����L�[�̓���
			y = y + 3;
		}

		else if ((PADInput & PAD_INPUT_UP) != 0) { //�\����L�[�̓���
			y = y - 3;
		}
	}
};






