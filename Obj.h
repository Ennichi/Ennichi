#pragma once
#include "stdafx.h"
#include "Sprite.h"

class Obj
{
public:
	/* メンバ変数 */
	int x, y; // オブジェクトの座標
	int xlength, ylength; // x, y方向の長さ
	int state = 0; // 描画する画像の、imageにおける添え字

	double angle = 0.0; // 画像の回転

	bool can_collision; // 他のオブジェクトと衝突するかどうか

	Sprite sprite;//スプライト

	
	/* メンバ関数 */
	Obj( // コンストラクタ
		int x,
		int	y,
		bool can_collision,
		const Sprite& sprite0
	) : x(x), 
		y(y), 
		can_collision(can_collision), 
		sprite(sprite0), 
		xlength(sprite0.width), 
		ylength(sprite0.height)
	{}
	

	Obj( // コンストラクタ(角度指定あり)
		int x,
		int	y,
		double angle,
		bool can_collision,
		const Sprite& sprite0
	) : x(x), 
		y(y), 
		angle(angle), 
		can_collision(can_collision),
		sprite(sprite0),
		xlength(sprite0.width),
		ylength(sprite0.height)
	{}



	virtual void draw()
	{
		/* オブジェクトを画面に反映する */
		if (DrawRotaGraph(x + xlength / 2, y + ylength / 2, 1.0, angle, sprite[state], 1) == -1) {
			throw new std::runtime_error("描画失敗");
			exit(1);
		}
	}

	virtual bool isCollision(Obj& other) {
		if (!(can_collision && other.can_collision)) return false; // どちらかが衝突不可
		int dx = x - other.x, dy = y - other.y;//*thisの座標に対するotherの相対座標
		if ((dx < xlength && dx > -other.xlength) && (dy < ylength && dy > -other.ylength))return true;
		return false;
	}
};

