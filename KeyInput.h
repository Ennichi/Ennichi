#pragma once
#include "stdafx.h"

class KeyInput
{
	using uchar = unsigned char;
private:
	/* 外部参照不可 */
	std::vector<uchar> Pressed;
	std::vector<uchar> prev_Pressed;
	std::vector<uchar> Target_key;
	char* __KeyState;
public:
	/* 外部参照可 */

	//デフォルトコンストラクタ(キー指定無し)
	KeyInput() : Pressed{}, prev_Pressed{}, Target_key{}
	{
		__KeyState = new char[256];
	}

	//コンストラクタ(キー一つ指定)
	KeyInput(uchar target) : Pressed{}, prev_Pressed{}, Target_key(1, target)
	{
		__KeyState = new char[256];
	}

	//コンストラクタ(イニシャライザーリスト)
	KeyInput(std::initializer_list<uchar> target
	) : Pressed{}, prev_Pressed{}, Target_key{}
	{
		__KeyState = new char[256];
		for (auto temp : target)
		{
			Target_key.push_back(temp);
		}
	}

	//コンストラクタ(vectorで渡す)(ほぼ使わない)
	KeyInput(const std::vector<uchar>& target) : Pressed{}, Target_key{target}
	{
		__KeyState = new char[256];
	}

	virtual ~KeyInput()
	{
		delete[] __KeyState;
	}

	//対象キーを追加
	void add(uchar KeyCode)
	{
		Target_key.push_back(KeyCode);
	}

	//キーが押されているか
	bool GetKey(uchar KeyCode)
	{
		for (auto __key : Pressed)
		{
			if (__key == KeyCode)return true;
		}
		return false;
	}

	//キーが押されたか
	bool GetKeyDown(uchar KeyCode)
	{
		for (auto __key : Pressed)
		{
			if (__key == KeyCode)
			{
				for (auto __prev_key : prev_Pressed)
				{
					if (__prev_key == KeyCode)return false;
				}
				return true;
			}
		}
		return false;
	}

	//キーが離されたか
	bool GetKeyUp(uchar KeyCode)
	{
		for (auto __key : Pressed)
		{
			if (__key == KeyCode)
			{
				return false;
			}
		}
		for (auto __prev_key : prev_Pressed)
		{
			if (__prev_key == KeyCode)
			{
				return true;
			}
		}
		return false;
	}

	//ループの最初に実行
	void operator()()
	{
		if (GetHitKeyStateAll(__KeyState) == -1)
			throw new std::runtime_error("キー押下状態取得失敗");
		prev_Pressed = Pressed;
		Pressed.clear();
		for (auto __target : Target_key)
		{
			if (__KeyState[__target] == 1)
			{
				Pressed.push_back(__target);
			}
		}
	}

};
