#pragma once
#include "stdafx.h"
#include "Poi.h"
#include "functions.h"
//金魚の正面はここで合わせる
#define ANGLE0 (- DX_PI / 2.0)

//金魚の動き方
enum class MOV_OPTION : char
{
	LINEAR = 0,
	CIRCLE,
	WAVE
};


//メインクラス
class Goldfish : public Obj
{
private:
	/* 外部アクセス不可 */
	double speed = 0.0;
	double xerr = 0.0;
	double yerr = 0.0;

	double __wave_info_x = 0.0;
	double __wave_info_angle = 0.0;
	MOV_OPTION moveOption = MOV_OPTION::LINEAR;
	double move_paramater1 = 0.0;
	double move_paramater2 = 0.0;

	unsigned int difficulty = 10;


	//クラス外では実行しない
	//自身とポイを比較,評価
	//~1000の値を返す
	int triggeredLevel(const Poi& poi)const&
	{
		int midx = x, midy = y;
		Goldfish::center(midx, midy, xlength, ylength);
		double d = 1000.0 * dist_elipse(midx, midy, poi.center()[0], poi.center()[1], xlength, ylength);
		return 1000 - (int)(d * static_cast<double>(difficulty));
	}

public:
	/* アクセス可能 */

	//difficultyのセッター(不正な値入力防止)
	void setDifficulty(unsigned int dif)noexcept
	{
		if (dif == 0)return;
		difficulty = dif;
	}

	//difficultyのゲッター
	int getDifficulty()noexcept
	{
		return difficulty;
	}

	//次のフレームに更新(検討中)
	//金魚が向いてる向きに(x,y)を進める
	void Next()
	{
		if (x > 1080) {
			x = 100;
		}
		else if (x < 100) {
			x = 1080;
		}
		else if (y > 500) {
			y = 100;
		}
		else if (y < 100) {
			y = 500;
		}
		double delx = speed * std::cos(angle + ANGLE0) + xerr;
		double dely = speed * std::sin(angle + ANGLE0) + yerr;
		x += std::lround(delx);
		y += std::lround(dely);
		xerr = delx - std::lround(delx);//x座標の丸め誤差(次ループで足す)
		yerr = dely - std::lround(dely);//y座標の丸め誤差(次ループで足す)
		switch (moveOption)
		{
			//円弧の軌道で動く
		case MOV_OPTION::CIRCLE:
			angle += move_paramater2 * (DX_PI / 2.0 - acos(speed / (move_paramater1 * 2.0)));
			__wave_info_x = 0.0;
			break;

			//正弦波の軌道で動く
		case MOV_OPTION::WAVE: {
			if (__wave_info_x == 0.0)__wave_info_angle = angle;
			double slope = -move_paramater1 * (2.0 * DX_PI / move_paramater2) * std::sin((2.0 * DX_PI / move_paramater2) * __wave_info_x);
			__wave_info_x += speed / (std::sqrt(slope * slope + 1));
			angle = __wave_info_angle + std::atan(slope);
			break;
		}
			//直線だとbreak(-Wswitch)
		case MOV_OPTION::LINEAR:
			break;
		}
	}

	/*金魚の動きを設定(未完成)
	*
	**** 一回のみ実行 ****
	* MOV_OPTION::CIRCLE
	* | 円弧に沿って動く
	* | p1...半径, p2...1.0ならangle+90の方向に中心、-1.0ならangle-90の方向に中心
	* MOV_OPTION::WAVE
	* | 波型に動く
	* | p1...振幅, p2...波長
	*/
	void SetMovement(MOV_OPTION option, double p1, double p2)
	{
		constexpr double EPS = 0.000001;
		if (option == MOV_OPTION::CIRCLE && abs(p2) < EPS)throw new std::out_of_range("二つ目のパラメータの値が小さすぎます");
		moveOption = option;
		move_paramater1 = p1;
		move_paramater2 = p2 > 0.0 ? 1.0 : -1.0;
	}

	//コンストラクタ
	Goldfish(
		int x,
		int	y,
		bool can_collision,
		const std::vector<int>& image_handle,
		unsigned int anim = 0
	) : Obj(x, y, can_collision, image_handle, anim)
	{}

	//コンストラクタ
	Goldfish(
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<int>& image_handle,
		unsigned int anim = 0
	) : Obj(x, y, angle, can_collision, image_handle, anim)
	{}

	Goldfish(const Goldfish&) = default;
	Goldfish(Goldfish&&) = default;

	Goldfish& operator=(const Goldfish&) = default;
	Goldfish& operator=(Goldfish&&) = default;

	//スピードを決める
	double setSpeed(double spmin, double spmax)
	{
		if (spmin > spmax || spmin < 0)
			throw new std::out_of_range("spminとspmaxはともに正で後者の方が大きくなければならない");
		static std::random_device seed;
		static std::mt19937_64 mt(seed());
		static std::uniform_real_distribution<> randsp(spmin, spmax);
		speed = randsp(mt);//乱数
		return speed;
	}


	//インスタンスから実行しない関数
	//金魚の中心を返すマクロ
	static void center(int& x, int& y, int xlength, int ylength)noexcept
	{
		x += xlength / 2;
		y += ylength / 2;
	}


	/*
	* 金魚を捕まえることができたか返す関数
	* 第一引数:金魚のインスタンスの配列
	* 第二引数:ポイのインスタンス
	* できれば他の3つの関数のどれかを使ってください(こっちは少し動作が遅いかも)
	*/
	static std::vector<bool> isCought(const std::vector<Goldfish>& goldfish, const Poi& poi)
	{
		std::random_device seed;
		std::mt19937_64 mt(seed());
		std::uniform_int_distribution<> dice(1, 1000);
		return Goldfish::isCought(goldfish, poi, mt, dice);
	}


	/*
	* 金魚を捕まえることができたか返す関数
	* 第一引数:金魚のインスタンスの配列
	* 第二引数:ポイのインスタンス
	* 第三引数:乱数器本体のインスタンス
	* 第四引数:離散一様分布クラス(整数)のインスタンス(乱数器の一部)
	* std::uniform_int_distribution<>のインスタンス生成時はコンストラクタの引数を1,1000にしてください
	*/
	static std::vector<bool> isCought(
		const std::vector<Goldfish>& goldfish,
		const Poi& poi,
		std::mt19937_64& mt,
		std::uniform_int_distribution<int>& dice
	)
	{
		std::vector<bool> result;
		if (dice.a() != 1 || dice.b() != 1000)throw new std::invalid_argument("乱数の範囲を1から1000に設定してください");
		for (unsigned int i = 0; i < goldfish.size(); ++i)
		{
			//can_collisionがfalseだったらfalse,poiとgoldfishの距離によって変動する確率でtrueかfalseが決まる
			result.push_back(goldfish[i].can_collision && goldfish[i].triggeredLevel(poi) >= dice(mt));
		}
		return result;
	}


	/*
	* 金魚を捕まえることができたか返す関数
	* 第一引数:金魚のインスタンスの配列
	* 第二引数:ポイのインスタンス
	* 第三引数:乱数器本体のインスタンス
	* 第四引数:離散一様分布クラス(整数)のインスタンス(乱数器の一部)
	* 第五引数:戻り値が格納される参照
	* 戻り値:捕まった金魚の数
	* std::uniform_int_distribution<>のインスタンス生成時はコンストラクタの引数を1,1000にしてください
	*/
	static int isCought(
		const std::vector<Goldfish>& goldfish,
		const Poi& poi,
		std::mt19937_64& mt,
		std::uniform_int_distribution<int>& dice,
		std::vector<bool>& result
	)
	{
		int cought_count = 0;
		if (dice.a() != 1 || dice.b() != 1000)throw new std::invalid_argument("乱数の範囲を1から1000に設定してください");
		for (unsigned int i = 0; i < goldfish.size(); ++i)
		{
			//can_collisionがfalseだったらfalse,poiとgoldfishの距離によって変動する確率でtrueかfalseが決まる
			result.push_back(goldfish[i].can_collision && goldfish[i].triggeredLevel(poi) >= dice(mt));
			if (*result.end())cought_count++;
		}
		return cought_count;
	}


	//個別パターン
	bool isCought(
		const Poi& poi,
		std::mt19937_64& mt,
		std::uniform_int_distribution<int>& dice
	)const&
	{
		if (dice.a() != 1 || dice.b() != 1000)throw new std::invalid_argument("乱数の範囲を1から1000に設定してください");
		return can_collision && (triggeredLevel(poi) >= dice(mt));
	}

	//乱数生成器を引数から取り払ったバージョン
	bool isCought(const Poi& poi)const&
	{
		static std::random_device seed;
		static std::mt19937_64 mt(seed());
		static std::uniform_int_distribution<> dice(1, 1000);
		return can_collision && (triggeredLevel(poi) >= dice(mt));
	}
};
