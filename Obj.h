#pragma once
#include "stdafx.h"


class Obj
{
public:
	/* メンバ変数 */
	int x, y; // オブジェクトの座標
	int xlength=0, ylength=0; // x, y方向の長さ
	int state = 0; // 描画する画像の、imageにおける添え字

	double angle = 0.0; // 画像の回転

	bool can_collision; // 他のオブジェクトと衝突するかどうか

	std::vector<int> images; // ロードした画像(ハンドル)のリスト


	
	/* メンバ関数 */
	Obj( // コンストラクタ
		int x,
		int	y,
		bool can_collision,
		const std::vector<const char*>& image_path
	) : x(x), y(y), can_collision(can_collision)
	{
		/* image_pathで指定された画像を読み込み, imagesに保存 */
		for (int i = 0; i < (int)image_path.size(); i++) {
			int t = LoadGraph(image_path[i]); // 指定画像を読み込む
			if (t == -1) {
				exit(1);
			}
			images.push_back(t); // imagesに追加
			GetGraphSize(images[0], &xlength, &ylength);
		}
	}

	Obj( // コンストラクタ(角度指定あり)
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<const char*>& image_path
	) : x(x), y(y), angle(angle), can_collision(can_collision)
	{
		/* image_pathで指定された画像を読み込み, imagesに保存 */
		for (int i = 0; i < (int)image_path.size(); i++) {
			int t = LoadGraph(image_path[i], 1); // 指定画像を読み込む
			if (t == -1) {
				exit(1);
			}
			images.push_back(t); // imagesに追加
			GetGraphSize(images[0], &xlength, &ylength);
		}
	}


	virtual ~Obj() {
		for (int i = 0; i < (int)images.size(); i++) {
			DeleteGraph(images[i]); // 読み込んだ画像をメモリから削除
		}
	}

	virtual void draw()
	{
		/* オブジェクトを画面に反映する */
		if (DrawRotaGraph(x + xlength / 2, y + ylength / 2, 1.0, angle, images[state], 1) == -1) {
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

