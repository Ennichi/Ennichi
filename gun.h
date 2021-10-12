#pragma once
#include "stdafx.h"
#include "Obj.h"

class Gun : public Obj {
public:
	Gun(int x, int	y, bool can_collision, const std::vector<int>& image_handle) 
		: Obj(x, y, can_collision, image_handle) {};
	void gunnt_change() {
		if (x > 1080) {
			x = 1080;
		}
		else if (x < 100) {
			x = 100;
		}
		else if (y > 540) {
			y = 540;
		}
		else if (y < 100) {
			y = 100;
		}
		int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1); 
		if ((PADInput & PAD_INPUT_DOWN) != 0) { //十字下キーの入力
			y = y + 3;
		}

		if ((PADInput & PAD_INPUT_UP) != 0) { //十字上キーの入力
			y = y - 3;
		}
	}
};