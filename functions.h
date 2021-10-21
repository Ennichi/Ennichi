#pragma once
#include "stdafx.h"

/* 汎用関数などまとめ */

/*ベクトル構造体
* 使わなかったら消します
*/
struct Vector2
{
	double x = 0.0;
	double y = 0.0;

	//基底のコンストラクタ
	Vector2()
	{}

	//コンストラクタ
	Vector2(double x0, double y0) : x{x0}, y{y0}
	{}

	/*
	* コンストラクタ(イニシャライザーリスト)
	* 使い方
	* Vector2 a = {2.0, 3.0};
	*/
	Vector2(std::initializer_list<double> list
	) :x{ *list.begin() }, y{ *(list.begin() + 1) }
	{}

	/*
	* 代入演算子定義(イニシャライザーリスト)
	* 使い方
	* Vector2 a;
	* a = {3.0, 2.5}
	*/
	Vector2& operator=(std::initializer_list<double> list)
	{
		x = *list.begin();
		y = *(list.begin() + 1);
		return *this;
	}
};


inline double distance(double dx, double dy)
{
	return std::sqrt(dx * dx + dy * dy);
}

double dist_elipse(int x1, int y1, int x2, int y2, double magx, double magy);

bool file_exists(const std::string&);

template<typename... strArgs>
void makeImageHandle(std::vector<int>& vHandle, const strArgs... pathArgs)
{
	std::vector<const char*> path_v = { pathArgs... };
	unsigned int vsize = static_cast<unsigned int>(vHandle.size());
	if (vsize > path_v.size())
	{
		vHandle.erase(vHandle.begin() + path_v.size() - 1ull, vHandle.end());
	}
	std::vector<int>::iterator itr = vHandle.begin(), itrend = vHandle.end();
	for (auto tmp : path_v)
	{
		if (itr != itrend)
		{
			*itr = LoadGraph(tmp);
			if (*itr == -1)
			{
				throw new std::runtime_error("画像ファイル" + static_cast<std::string>(tmp) + "が読み込めませんでした");
			}
			++itr;
		}
		else
		{
			vHandle.push_back(LoadGraph(tmp));
			if (*(vHandle.end() - 1) == -1)
			{
				throw new std::runtime_error("画像ファイル" + static_cast<std::string>(tmp) + "が読み込めませんでした");
			}
		}
	}

}