#include "main.h"

void shootingmain() {
	/* 初期化 */
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	std::vector<const char*> file_path = { "./score_shoooting.csv" };
	std::vector<const char*> gun_path = { "./image/gun.png" };
	Gun first(0, 250, true, gun_path);
	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア
		if (ProcessMessage() == -1) break;	//エラーが起きたらループをぬける

		if (windowFlag == 0) {  // メニューウィンドウ

		}
		else if (windowFlag == 1) { // ゲーム中のウィンドウ
			while (ProcessMessage() == 0) {
				first.gunnt_change();
				first.draw();
				ScreenFlip();

			}
		}
		else if (windowFlag == 2) {//結果表示のウィンドウ

		}
		else {  // ゲームの終了
			return;
		}
		ScreenFlip();
	}
}