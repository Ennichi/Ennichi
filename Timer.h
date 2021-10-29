#pragma once
#define max(a, b) (((a) > (b)) ? (a) : (b))
#include "stdafx.h"

//非同期式タイマー
class nSyncTimer
{
	using LL = long long;

private:
	/* 外部参照不可 */
	unsigned short fps = 60;
	LL configured_time = 0;
	LL start_time = 0;
	bool isSet = false;

public:
	/* 外部参照可 */

	//デフォルトコンストラクタ
	nSyncTimer(){}

	//fps設定コンストラクタ
	nSyncTimer(unsigned short FramePerSecond) : fps{FramePerSecond} {}

	//単位フレームでタイマーセット
	void setf(unsigned int frames)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = (1000000ll * (LL)frames) / fps;
		isSet = true;
	}

	//単位ミリ秒でタイマーセット
	void setms(unsigned int ms)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = 1000ll * (LL)ms;
		isSet = true;
	}

	//単位秒でタイマーセット
	void sets(double seconds)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = (LL)(1000000.0 * seconds);
		isSet = true;
	}

	//単位マイクロ秒でタイマーセット
	void set(LL microseconds)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = microseconds;
		isSet = true;
	}

	//一回セットしたらこれを実行するまでもう一度使えない
	void reset()
	{
		isSet = false;
		configured_time = 0;
		start_time = 0;
	}

	//タイマーの名前()で残り時間を取得(タイマー終了時0)
	LL operator()()const&
	{
		LL temp = configured_time + start_time - GetNowHiPerformanceCount();
		return max(temp, 0);
	}
};

//同期式タイマー
class Timer
{
	using uLL = unsigned long long;

private:
	/* 外部参照不可 */
	uLL __configured = 0;
	uLL __remaining = 0;

public:

	//コンストラクタ
	Timer()
	{}

	Timer(uLL frames):__configured{frames}, __remaining{frames}
	{}

	//時間を設定(フレーム)
	void set(uLL frames)noexcept
	{
		__configured = frames;
	}

	//時間を設定(秒)
	void sets(uLL sec)
	{
		__configured = sec * 60;
	}

	//タイマーをリセットする
	void reset()noexcept
	{
		__remaining = __configured;
	}

	//毎フレーム実行
	//これを実行している間タイマーが作動する
	void update()noexcept
	{
		if (__remaining != 0)
		{
			--__remaining;
		}
	}

	//残りフレーム
	uLL operator()()noexcept
	{
		return __remaining;
	}
};