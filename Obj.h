#pragma once
#include "stdafx.h"


class Obj
{
protected:
	/* 派生クラスのみアクセス可能 */
	unsigned int __frames = 0;

public:
	/* メンバ変数 */
	int x, y; // オブジェクトの座標
	int xlength=0, ylength=0; // x, y方向の長さ
	int state = 0; // 描画する画像の、imageにおける添え字
	int xCenter = 0, yCenter = 0;// 描画時の回転の中心
	double xScale = 1.0, yScale = 1.0;// x,y方向の拡大率
	bool reverse = false;// 左右反転するかどうか

	unsigned int animsp = 0;//アニメーションのコマ送りの速度(何フレームに一回の速度か)(0なら停止)

	double angle = 0.0; // 画像の回転

	bool can_collision; // 他のオブジェクトと衝突するかどうか

	std::vector<int> images; // ロードした画像(ハンドル)のリスト


	
	/* メンバ関数 */
	Obj( // コンストラクタ
		int x,
		int	y,
		bool can_collision,
		const std::vector<int>& image_handle
	) : x(x), y(y), can_collision(can_collision),images(image_handle)
	{
		if(!images.empty())GetGraphSize(images[0], &xlength, &ylength);
		xCenter = xlength / 2;
		yCenter = ylength / 2;
	}

	Obj( // コンストラクタ(角度指定あり)
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<int>& image_handle
	) : x(x), y(y), angle(angle), can_collision(can_collision),images(image_handle)
	{
		if(!images.empty())GetGraphSize(images[0], &xlength, &ylength);
		xCenter = xlength / 2;
		yCenter = ylength / 2;
	}

	//画像無しコンストラクタ
	Obj(
		int x,
		int y
	) : x(x), y(y), can_collision(false), images()
	{}

	//コピーコンストラクタ自動生成
	Obj(const Obj&) = default;

	//ムーブコンストラクタ無し
	Obj(Obj&&) = delete;

	virtual ~Obj() {
		images.~vector();
	}

	virtual void draw()
	{
		/* オブジェクトを画面に反映する */
		if (DrawRotaGraph3(x + xlength / 2, y + ylength / 2, xCenter, yCenter, xScale, yScale, angle, images[state], TRUE, reverse) == -1) {
			throw new std::runtime_error("描画失敗");
			exit(1);
		}
		if (animsp != 0)
		{
			__frames++;
			if (__frames >= animsp)
			{
				__frames = 0;
				state = (state + 1) % static_cast<int>(images.size());
			}
		}
	}

	virtual bool isCollision(Obj& other) {
		if (!(can_collision && other.can_collision)) return false; // どちらかが衝突不可
		int dx = x - other.x, dy = y - other.y;//*thisの座標に対するotherの相対座標
		if ((dx < xlength && dx > -other.xlength) && (dy < ylength && dy > -other.ylength))return true;
		return false;
	}
};

