#pragma once
#include "../../../header/stdafx.h"

//ポイのクラスの定義(Objの継承)
class Poi : public Obj {
public:
	Poi(int x, int	y, bool can_collision, const std::vector<int>& image_handle) : Obj(x, y, can_collision, image_handle) {};
	//Objの角度なしコンストラクタ呼び出し
	//画像サイズが128x128の時のとき、左上に対する中心の座標は(62, 48)
	virtual std::vector<int> center()const&
	{
		std::vector<int> pos = {x + 64, y + 43};
		return pos;
	}

	void point_change(const KeyInput& input)
	{
		signed char xdirection = 0, ydirection = 0;
		if (input.GetKey(KEY_INPUT_RIGHT) && x < 1080)xdirection++;
		if (input.GetKey(KEY_INPUT_LEFT) && x > 100)xdirection--;
		if (input.GetKey(KEY_INPUT_DOWN) && y < 500)ydirection++;
		if (input.GetKey(KEY_INPUT_UP) && y > 100)ydirection--;
		x += xdirection * 3;
		y += ydirection * 3;
	}
};






