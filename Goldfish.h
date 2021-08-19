#pragma once
#include "stdafx.h"
#include "Poi.h"
#include "functions.h"
#define ANGLE0 (-std::_Pi/2.0)

//金魚の動き方
enum class MOV_OPTION
{
	CIRCLE = 0,
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
	unsigned int difficulty = 10;

	//クラス外では実行しない
	//自身とポイを比較,評価
	//~1000の値を返す
	int triggeredLevel(const Poi& poi)const&
	{
		int midx = x, midy = y;
		Goldfish::center(midx, midy, xlength, ylength);
		double d = distance(midx, midy, poi.center());
		return 1000 - (int)(d * static_cast<double>(difficulty));
	}

public:
	/* アクセス可能 */

	//difficultyのセッター(不正な値入力防止)
	void setDifficulty(unsigned int dif)
	{
		if (dif == 0)return;
		difficulty = dif;
	}

	//difficultyのゲッター
	int getDifficulty()
	{
		return difficulty;
	}

	//次のフレームに更新(検討中)
	//金魚が向いてる向きに(x,y)を進める
	inline void Next()
	{
		double delx = speed * std::cos(angle + ANGLE0) + xerr;
		double dely = speed * std::sin(angle + ANGLE0) + yerr;
		x += std::lround(delx);
		y += std::lround(dely);
		xerr = delx - std::lround(delx);//x座標の丸め誤差(次ループで足す)
		yerr = dely - std::lround(dely);//y座標の丸め誤差(次ループで足す)
	}
	
	/*金魚の動きを設定(未完成)
	* 
	**** Next()の前に実行してください(毎フレーム実行) ****
	* MOV_OPTION::CIRCLE
	* | 円弧に沿って動く
	* | p1...半径, p2...1.0ならangle+90の方向に中心、-1.0ならangle-90の方向に中心
	* MOV_OPTION::WAVE
	* | 波型に動く
	* | p1...振幅, p2...波長
	*/
	inline void SetMovement(MOV_OPTION option, double p1, double p2)
	{
		switch (option)
		{
			//円弧の軌道で動く
		case MOV_OPTION::CIRCLE:
			if (!(p2 == 1.0 || p2 == -1.0))
				throw new std::out_of_range("円形移動のときはp2を1.0か-1.0にしてください");
			angle += p2 * (std::_Pi / 2.0 - acos(speed / (p1 * 2.0)));
			__wave_info_x = 0.0;
			break;

			//正弦波の軌道で動く
		case MOV_OPTION::WAVE:
			if (__wave_info_x == 0.0)__wave_info_angle = angle;
			double slope = -p1 * (2.0 * std::_Pi / p2) * std::sin((2.0 * std::_Pi / p2) * __wave_info_x);
			__wave_info_x += speed / (std::sqrt(slope * slope + 1));
			angle = __wave_info_angle + std::atan(slope);
			break;
		}
	}

	//コンストラクタ(検討中)
	Goldfish(
		int x,
		int	y,
		bool can_collision,
		const std::vector<const char*>& image_path
	) : Obj(x,y,can_collision,image_path)
	{}

	//コンストラクタ(検討中)
	Goldfish(
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<const char*>& image_path
	) : Obj(x,y,angle,can_collision,image_path)
	{}


	//スピードを決める
	double setSpeed(double spmin, double spmax)
	{
		if (spmin > spmax || spmin < 0)
			throw new std::out_of_range("spminとspmaxはともに正で後者の方が大きくなければならない");
		std::random_device seed;
		std::mt19937_64 mt(seed());
		std::uniform_real_distribution<> randsp(spmin, spmax);
		speed = randsp(mt);//乱数
		return speed;
	}


	//描画関数オーバーライド(検討中)
	void draw() override
	{
		Obj::draw();
	}

	//インスタンスから実行しない関数
	//金魚の中心を返すマクロ
	static inline void center(int& x, int& y, int xlength, int ylength)
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
		const std::uniform_int_distribution<int>& dice
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
		const std::uniform_int_distribution<int>& dice,
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
		const std::uniform_int_distribution<int>& dice
	)const&
	{
		if (dice.a() != 1 || dice.b() != 1000)throw new std::invalid_argument("乱数の範囲を1から1000に設定してください");
		return can_collision && triggeredLevel(poi) >= dice(mt);
	}
};
