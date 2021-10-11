#pragma once
#include "stdafx.h"
#include "Obj.h"

class InputText : public Obj
{
	using ULL = unsigned long long;
private:
	/* 外部アクセス不可 */
	char* __textptr;
	int inputHandle = 0;
	int fontHandle;
	unsigned int maxlen;
	bool CancelValidFlag;
	bool SingleCharOnlyFlag;
	bool NumCharOnlyFlag;

public:
	/* アクセス可能 */
	int NmlStr = 0xFF000000;//入力文字列の色
	int NmlCur = 0xFF000000;//IME非使用時のカーソルの色
	int IMEStrBack = 0xFFFFFF00;//IME使用時の入力文字列の周りの色
	int IMECur = 0xFF000000;//IME使用時のカーソルの色
	int IMELine = 0xFFFFFFFF;//IME使用時の変換文字列の下線の色
	int IMESelectStr = 0xFF000000;//IME使用時の選択対象の変換候補文字列の色
	int IMESelectStrBack = 0xFFFF0000;//IME使用時の選択対象の変換候補文字列の周りの色
	int IMEConvWinStr = 0xFFFFFFFF;//IME使用時の変換候補ウインドウ内の文字列の色


	//コンストラクタ
	InputText(int x0, int y0, unsigned int maxlen0, int font = -1) :Obj(x0, y0), CancelValidFlag{false}, SingleCharOnlyFlag{false}, NumCharOnlyFlag{false}, maxlen{maxlen0}, fontHandle{font}
	{
		__textptr = new char[static_cast<ULL>(maxlen) + 1ull];
		inputHandle = MakeKeyInput(maxlen, FALSE, FALSE, FALSE);
		if (inputHandle == -1)throw new std::runtime_error("テキスト入力オブジェクト作成失敗");
		SetActiveKeyInput(inputHandle);
		SetKeyInputCursorBrinkFlag(TRUE);
	}

	//コンストラクタ
	InputText(
		int x0,
		int y0,
		unsigned int maxlen0,
		bool CancelValidFlag0,
		bool SingleCharOnlyFlag0,
		bool NumCharOnlyFlag0,
		int font = -1
	) : Obj(x0, y0), CancelValidFlag{CancelValidFlag0}, SingleCharOnlyFlag{SingleCharOnlyFlag0}, NumCharOnlyFlag{NumCharOnlyFlag0},maxlen{maxlen0}, fontHandle{font}
	{
		__textptr = new char[static_cast<ULL>(maxlen) + 1ull];
		inputHandle = MakeKeyInput(maxlen, CancelValidFlag, SingleCharOnlyFlag, NumCharOnlyFlag);
		if (inputHandle == -1)throw new std::runtime_error("テキスト入力オブジェクト作成失敗");
		SetKeyInputCursorBrinkFlag(TRUE);
	}

	//コピーコンストラクタ
	InputText(const InputText& base) : Obj(base),CancelValidFlag{base.CancelValidFlag}, SingleCharOnlyFlag{base.SingleCharOnlyFlag}, NumCharOnlyFlag{base.NumCharOnlyFlag}, maxlen{base.maxlen}, fontHandle{base.fontHandle}
	{
		__textptr = new char[maxlen];
		inputHandle = MakeKeyInput(maxlen, CancelValidFlag, SingleCharOnlyFlag, NumCharOnlyFlag);
		if (inputHandle == -1)throw new std::runtime_error("テキスト入力オブジェクト作成失敗");
	}

	//デストラクタ
	virtual ~InputText()
	{
		DeleteKeyInput(inputHandle);
		delete[] __textptr;
	}

	//描画
	void draw() override
	{
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, NmlStr);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, NmlCur);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_STR_BACK, IMEStrBack);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_CURSOR, IMECur);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_LINE, IMELine);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_SELECT_STR, IMESelectStr);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_SELECT_STR_BACK, IMESelectStrBack);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR, IMEConvWinStr);
		SetKeyInputStringFont(fontHandle);
		DrawKeyInputString(x, y, inputHandle);
		GetKeyInputString(__textptr, inputHandle);

	}

	//テキスト取得
	void text(std::string& str)const&
	{
		str = __textptr;
	}

	//テキスト取得
	void text(char* str)const&
	{
		GetKeyInputString(str, inputHandle);
	}

	//有効化
	void set()const& noexcept
	{
		SetActiveKeyInput(inputHandle);
		SetKeyInputString("", inputHandle);
	}
};