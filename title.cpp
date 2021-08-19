#include "main.h"

int FontHandle;
void titlemain() {
	SetMainWindowText("縁日ゲーム(タイトル)");

	int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	DrawStringToHandle(250, 240 - 32, "タイトルです\nZキーで金魚を選択です", GetColor(20 + rand() % 256, 20 + rand() % 256, 20 + rand() % 256), CreateFontToHandle("メイリオ", 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE));
	while (1) {
		if (ProcessMessage() == -1) {
			break;	//エラー
		}
		PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if ((PADInput & PAD_INPUT_1) != 0) {
			ClearDrawScreen();
			kingyomain(); 
		}
		else if ((PADInput & PAD_INPUT_RIGHT) != 0) { // 矢印右キーを押す
			ClearDrawScreen();
			game_temp();
		}
		else if ((PADInput & PAD_INPUT_LEFT) != 0) { // 矢印左キーを押す
			ClearDrawScreen();
			shootingmain();
		}
	}
}