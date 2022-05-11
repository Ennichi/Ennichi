#pragma once

#include "../../../header/stdafx.h"
class StringObj
{
public:
	// メンバ変数
	int x = 0, y = 0; // 左上の座標
	const char* label = NULL; // 表示する文字列
	unsigned int color = 0x000000;
	int font = 0;

	// メンバ関数
	StringObj(
		int x,
		int y,
		const char* label,
		unsigned int color,
		int font
	) : x(x), y(y), label(label), color(color), font(font)
	{}

	void draw() {
		DrawStringToHandle(x, y, label, color, font);
	}
};

