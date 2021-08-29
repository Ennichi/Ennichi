#include "main.h"
#include "stdafx.h"

void kingyomain() {
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60;//fps
	LONGLONG nowtime, pretime;//現在時間

	std::vector<const char*> path = { "./image/goldfish_open.png", "./image/goldfish_close.png" };
	std::vector<const char*> buttonpath = { "./image/button2.png", "./image/button1.png" };
	std::vector<const char*> poi_path = { "./image/poi.png" };

	std::random_device seed;//乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	int px, py;
	int click_event, button_type, cx, cy, log_type;

	Button button1(250, 240, false, buttonpath);
	Button button2(250, 240 + 115 + 30, false, buttonpath);
	Button button3(250, 240 + 115 * 2 + 30 * 2, false, buttonpath);

	Goldfish *fish1=new Goldfish(0, 0, DX_PI, true, path); //金魚
	Goldfish fish2(100, 0, DX_PI * 3.0 / 4.0, true, path);
	Poi first(500, 500, true, poi_path);//ポイ
	Goldfish fish3 = *fish1;

	fish1->setDifficulty(50);
	KeyInput input(KEY_INPUT_Z);

	fish1->setSpeed(0.5, 1.0);//スピード設定

	int font = CreateFontToHandle("メイリオ", 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

	pretime = GetNowHiPerformanceCount();
	int wantsend = 0;

	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア
		

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//エラーが起きたらループをぬける

		if (windowFlag == 0) {  // メニューウィンドウ
			SetMainWindowText("金魚すくい(タイトル)");	//windowテキスト

			DrawStringToHandle(350, 120, "金魚ゲーム", GetColor(120,120,120), font);
			button1.draw();	//ゲームスタート
			button2.draw();	//チュートリアル
			button3.draw();	//過去の成績
			button1.next(px, py);
			button2.next(px, py);
			button3.next(px, py);
			if (button1.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//ボタン用
			}
			else if (button2.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 2;	//ボタン用
			}
			else if (button3.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 3;	//ボタン用
			}
		}
		else if (windowFlag == 1) { // ゲーム中のウィンドウ
			SetMainWindowText("金魚すくい(ゲーム中)");	//windowテキスト
			input();
			if (fish1 != NULL)
			{
				fish1->Next();
				fish1->draw();
			}
			fish3.Next();
			fish3.draw();;
			first.point_change();
			first.draw();
			if (fish1 != NULL && input.GetKeyDown(KEY_INPUT_Z) == 1 && fish1->isCought(first, mt, dice))
			{
				printfDx("捕まった");
				delete fish1;
				fish1 = NULL;
			}
		}
		else if (windowFlag == 2) {	//チュートリアル
			SetMainWindowText("金魚すくい(チュートリアルはボタンでキャンセルできます)");	//windowテキスト
			PlayMovie("./image/tutorial.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);	//チュートリアルを再生する(ボタンキャンセルあり)
			windowFlag = 0;
		}
		else if (windowFlag == 3) {

		}
		else {  // ゲームの終了
			return;
		}
		ScreenFlip();

		/* フレームレートの処理 */
		nowtime = GetNowHiPerformanceCount();
		while (nowtime - pretime < 1000000 / FramePerSecond)
		{
			nowtime = GetNowHiPerformanceCount();
		}
		pretime = nowtime;
	}
}