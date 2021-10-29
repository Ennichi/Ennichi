#pragma once
#include "stdafx.h"


class Obj
{
protected:
	/* 派生クラスのみアクセス可能 */
	unsigned int __frames = 0;
	double scale = 1.0; // 画像の表示倍率

public:
	/* メンバ変数 */
	int x, y; // オブジェクトの座標
	double angle = 0.0; // 画像の回転

	bool can_collision; // 他のオブジェクトと衝突するかどうか
	std::vector<int> images; // ロードした画像(ハンドル)のリスト
	unsigned int animsp = 0;//アニメーションのコマ送りの速度(何フレームに一回の速度か)(0なら停止)

	int xlength=0, ylength=0; // x, y方向の長さ
	int state = 0; // 描画する画像の、imageにおける添え字

	/* メンバ関数 */
	Obj( // コンストラクタ
		int x,
		int	y,
		bool can_collision,
		const std::vector<int>& image_handle,
		unsigned int anim = 0
	) : x(x), y(y), can_collision(can_collision),images(image_handle), animsp(anim)
	{
		if(!images.empty())GetGraphSize(images[0], &xlength, &ylength);
	}

	Obj( // コンストラクタ(角度指定あり)
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<int>& image_handle,
		unsigned int anim = 0
	) : x(x), y(y), angle(angle), can_collision(can_collision),images(image_handle), animsp(anim)
	{
		if(!images.empty())GetGraphSize(images[0], &xlength, &ylength);
	}

	//画像無しコンストラクタ
	Obj(
		int x,
		int y
	) : x(x), y(y), can_collision(false), images()
	{}

	//コピーコンストラクタ自動生成
	Obj(const Obj&) = default;

	//ムーブコンストラクタ自動生成
	Obj(Obj&&) = default;

	Obj& operator=(const Obj&) = default;
	Obj& operator=(Obj&&) = default;

	virtual ~Obj() {

	}

	void setScale(double scale0)
	{
		scale = scale0;
		xlength = (int)(xlength * scale);
		ylength = (int)(ylength * scale);
	}

	double getScale()const& noexcept
	{
		return scale;
	}

	virtual void draw()
	{
		/* オブジェクトを画面に反映する */
		if (DrawRotaGraph(x + xlength / 2, y + ylength / 2, scale, angle, images[state], 1) == -1) {
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

	virtual void Next()
	{
	}

	virtual bool isCollision(const Obj& other)const& {
		if (!(can_collision && other.can_collision)) return false; // どちらかが衝突不可
		int dx = x - other.x, dy = y - other.y;//*thisの座標に対するotherの相対座標
		if ((dx < xlength && dx > -other.xlength) && (dy < ylength && dy > -other.ylength))return true;
		return false;
	}
};

template<class T>
class ObjGroup
{
private:
	std::vector<T> objects;
	std::vector<int> tags;

public:
	ObjGroup() : objects{}, tags{}
	{}

	template<typename... objArgs>
	void addcpy(objArgs&... object)
	{
		for (const T& tmp: std::initializer_list<T>{ object... })
		{
			objects.push_back(tmp);
			tags.push_back(0);
		}
	}

	//オブジェクトを指定の個数だけコピーしてグループに追加
	void addcpy(T& object, unsigned int number)
	{
		for (unsigned int i = 0; i < number; ++i)
		{
			objects.push_back(object);
			tags.push_back(0);
		}
	}

	//グループに所属している全てのオブジェクトを描画
	void draw()
	{
		for (T& tmp : objects)
		{
			tmp.draw();
		}
	}

	//指定のタグが付けられたオブジェクトのみを描画する
	void draw(int Tag)
	{
		for (unsigned int i = 0; i < tags.size(); ++i)
		{
			if (tags[i] == Tag)
			{
				objects[i].draw();
			}
		}
	}
	//グループに所属している全てのオブジェクトを次フレームの状態に更新
	void Next()
	{
		for (T& tmp : objects)
		{
			tmp.Next();
		}
	}

	unsigned int size()noexcept
	{
		return (unsigned int)(objects.size());
	}

	//オブジェクトを削除
	void destroy(std::size_t index)
	{
		objects.erase(objects.begin() + index);
		tags.erase(tags.begin() + index);
	}

	//指定のオブジェクトにタグを付ける
	int& Tag(std::size_t index)&
	{
		return tags[index];
	}

	//指定のオブジェクトのタグを取得
	const int& Tag(std::size_t index)const&
	{
		return tags[index];
	}

	//オブジェクトを参照
	T& operator[](std::size_t index)&
	{
		return objects[index];
	}
	const T& operator[](std::size_t index)const&
	{
		return objects[index];
	}
};