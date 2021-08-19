#pragma once
#include "stdafx.h"
#include "Obj.h"

class Gun : public Obj {
public:
	Gun(int x, int	y, bool can_collision, const std::vector<const char*>& image_path) 
		: Obj(x, y, can_collision, image_path) {};
	void gunnt_change() {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア
		int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1); //要検討(金魚用と合わせて一階のゲームループの最初で受け取ってもいいかも)
		if ((PADInput & PAD_INPUT_DOWN) != 0) { //十字下キーの入力
			y = y + 3;
		}

		if ((PADInput & PAD_INPUT_UP) != 0) { //十字上キーの入力
			y = y - 3;
		}
	}
};