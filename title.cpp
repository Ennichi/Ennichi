#include "main.h"
#include "stdafx.h"

void titlemain() {
	SetMainWindowText("縁日ゲーム(タイトル)");
	int FontHandle = CreateFontToHandle(NULL,40,3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	int bgm = LoadSoundMem("./asset/bgm/maou_minzoku9.mp3");	//魔王魂民族09
	DrawStringToHandle(250, 240 - 32, "タイトルです\nZキーで金魚を選択です", GetColor(0,120,120),FontHandle);
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	while (1) {
		if (ProcessMessage() == -1) {
			break;	//エラー
		}
		PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if ((PADInput & PAD_INPUT_1) != 0) {
			ClearDrawScreen();
			DeleteSoundMem(bgm);	//これをなくすとゲーム中に流れるが、Deleteは必要 
			kingyomain(); 
		}
		else if ((PADInput & PAD_INPUT_RIGHT) != 0) { // 矢印右キーを押す
			ClearDrawScreen();
			DeleteSoundMem(bgm);
			game_temp();
		}
		else if ((PADInput & PAD_INPUT_LEFT) != 0) { // 矢印左キーを押す
			ClearDrawScreen();
			DeleteSoundMem(bgm);
			shootingmain();
		}
	}
}