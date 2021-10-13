#include "main.h"
#include "stdafx.h"

void syatekimain(int font, int bgm, int effect, int calling_check) {
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60;//fps
	int score = 0;	//ゲームのスコア
	LONGLONG nowtime, prevtime;//現在時間

	std::random_device seed;//乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<int> button_handle{};
	makeImageHandle(button_handle, "./asset/image/uncheck.png", "./asset/image/checked.png");

	std::vector<int> gun_handle{};
	makeImageHandle(gun_handle, "./asset/image/syateki.png");	//TODO:画像を置換する

	int px, py;
	int click_event, button_type, cx, cy, log_type;
	Button button_start(100, 200, false, button_handle);	//STARTボタン
	StringObj start_obj(150, 250, "スタート", GetColor(120, 120, 120), font);
	Button button_result(200, 350, false, button_handle);	//設定ボタン
	StringObj result_obj(250, 400, "結果", GetColor(120, 120, 120), font);
	Button button_gotokingyo(1000, 600, false, button_handle);	//射的ゲームへ行くボタン
	StringObj gotokingyo_obj(1025, 650, "金魚すくい", GetColor(120, 120, 120), font);
	Gun gun_syateki(100, 600, false, gun_handle);
	KeyInput input(KEY_INPUT_Z);

	prevtime = GetNowHiPerformanceCount();
	int clock = GetNowCount();	//現在時刻の取得
	Timer timer(3600);
	Timer timer2(2400);
	int back_img = LoadGraph("./asset/image/background.png");	//TODO:背景画像を射的の屋台にする
	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//エラーが起きたらループをぬける

		if (windowFlag == 0) {  // メニューウィンドウ
			SetMainWindowText("射的ゲーム(タイトル)");	//windowテキスト

			DrawStringToHandle(500, 120, "射的ゲーム[WIP]", GetColor(120, 120, 120), font);
			button_start.draw();	//ゲームスタート
			button_start.next(px, py);
			start_obj.draw();
			button_result.draw();	//結果画面
			button_result.next(px, py);
			result_obj.draw();
			button_gotokingyo.draw();		//射的ゲームへ
			button_gotokingyo.next(px, py);
			gotokingyo_obj.draw();
			if (button_start.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//金魚すくいスタート
			}
			if (button_result.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 2;	//結果表示
			}
			if (button_gotokingyo.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 10;	//金魚すくいゲームへ遷移
			}
		}
		else if (windowFlag == 1) { // ゲーム中のウィンドウ
			SetMainWindowText("射的ゲーム(ゲーム中)");	//windowテキスト
			DrawGraph(0, 0, back_img, TRUE);

			input();
			gun_syateki.gunnt_change();
			gun_syateki.draw();
			//60秒たったら終了
			if (timer() == 0) {
				SetMainWindowText("スコア表示");	//windowテキスト
				if (score > 0) {
					DrawFormatString(500, 200, GetColor(120, 120, 120), "%d打ち抜けました!", score, font);
				}
				else {
					DrawFormatString(500, 200, GetColor(120, 120, 120), "残念!",font);
				}
				if (timer2() == 0) {
					windowFlag = 0;
				}
				timer2.update();
			}
			else {
				DrawFormatString(1200, 0, GetColor(120, 120, 120), "残り%d秒", timer() / 60, font);
			}
			timer.update();
		}
		else if (windowFlag == 3) {
			SetMainWindowText("結果");	//windowテキスト
		}
		else if (windowFlag == 10) {  // ゲームの終了
			calling_check=1;
			kingyomain(font, bgm, effect,calling_check);
		}
		else {
			return;
		}
		ScreenFlip();

		/* フレームレートの処理 */
		nowtime = GetNowHiPerformanceCount();
		while (nowtime - prevtime < 1000000 / FramePerSecond)
		{
			nowtime = GetNowHiPerformanceCount();
		}
		prevtime = nowtime;
	}
}