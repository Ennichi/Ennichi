#pragma once
#include "stdafx.h"
#include "Obj.h"

//ポイのクラスの定義(Objの継承)
class Poi : public Obj {
public:
	Poi(int x, int	y, bool can_collision, const std::vector<int>& image_handle) : Obj(x, y, can_collision, image_handle) {};
	//Objの角度なしコンストラクタ呼び出し
	//画像サイズが400×400の時のとき、左上に対する中心の座標は(240,160)
	inline std::vector<int> center()const&
	{
		std::vector<int> pos = {x + 58, y + 34};
		return pos;
	}

	void point_change() {
		int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1); //要検討(金魚用と合わせて一階のゲームループの最初で受け取ってもいいかも)
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
		else if ((PADInput & PAD_INPUT_RIGHT) != 0) { //十字右キーの入力
			x = x + 3;
		}

		else if ((PADInput & PAD_INPUT_LEFT) != 0) { //十字左キーの入力
			x = x - 3;
		}

		else if ((PADInput & PAD_INPUT_DOWN) != 0) { //十字下キーの入力
			y = y + 3;
		}

		else if ((PADInput & PAD_INPUT_UP) != 0) { //十字上キーの入力
			y = y - 3;
		}
	}
};






