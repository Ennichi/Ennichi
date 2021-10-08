#include "main.h"
#include "stdafx.h"

void kingyomain(int font,int bgm,int effect) {
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60;//fps
	LONGLONG nowtime, prevtime;//現在時間
	
	std::vector<const char*> path = { "./asset/image/goldfish_open.png", "./asset/image/goldfish_close.png" };
	std::vector<const char*> buttonpath = { "./asset/image/button2.png", "./asset/image/button1.png" };
	std::vector<const char*> poi_path = { "./asset/image/poi.png" };

	std::random_device seed;//乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<int> handle{};
	for (auto image_path : path)
	{
		handle.push_back(LoadGraph(image_path));
		if (*(handle.end() - 1) == -1)
		{
			MessageBox(NULL, "画像読み込み時エラー", "Error Info", MB_ICONHAND);
			exit(1);
		}
	}

	std::vector<int> button_handle{};
	for (auto image_path : buttonpath)
	{
		button_handle.push_back(LoadGraph(image_path));
		if (*(button_handle.end() - 1) == -1)
		{
			MessageBox(NULL, "画像読み込み時エラー", "Error Info", MB_ICONHAND);
			exit(1);
		}
	}

	std::vector<int> poi_handle{};
	for (auto image_path : poi_path)
	{
		poi_handle.push_back(LoadGraph(image_path));
		if (*(poi_handle.end() - 1) == -1)
		{
			MessageBox(NULL, "画像読み込み時エラー", "Error Info", MB_ICONHAND);
			exit(1);
		}
	}

	int px, py;
	int click_event, button_type, cx, cy, log_type;

	Button button1(250, 240, false, button_handle);
	Button button2(250, 240 + 200, false, button_handle);

	Goldfish *fish1=new Goldfish(0, 0, 0, true, handle); //金魚
	Goldfish fish2(100, 0, DX_PI * 3.0 / 4.0, true, handle);
	Poi first(500, 500, true, poi_handle);//ポイ
	Goldfish fish3 = *fish1;

	ObjGroup<Goldfish> fish4;

	fish1->setDifficulty(10);
	fish1->animsp = 30;
	KeyInput input(KEY_INPUT_Z);

	fish1->setSpeed(0.5, 1.0);//スピード設定

	fish4.addcpy(*fish1, 10);
	for (int i = 0; i < 10; ++i)
	{
		fish4[i].angle -= (double)i * 0.2;
	}
	prevtime = GetNowHiPerformanceCount();
	int clock = GetNowCount();	//現在時刻の取得
	int score=0;	//ゲームのスコア

	Timer timer(3600);
	Timer timer2(2400);
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
			button1.next(px, py);
			button2.next(px, py);
			if (button1.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//ボタン用
			}
			else if (button2.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 2;	//ボタン用
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
			fish3.draw();
			fish4.Next();
			fish4.draw();
			first.point_change();
			first.draw();
			if (fish1 != NULL && input.GetKeyDown(KEY_INPUT_Z) == 1 && fish1->isCought(first, mt, dice))
			{
				printfDx("捕まった");
				delete fish1;
				fish1 = NULL;
				score++;
			}

			//60秒たったら終了
			if (timer() == 0) {
				SetMainWindowText("スコア表示中");	//windowテキスト
				DrawFormatString(500, 200, GetColor(120, 120, 120), "スコアは%dです", score, font);
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
		else if (windowFlag == 2) {	//チュートリアル
			SetMainWindowText("金魚すくい(チュートリアルはボタンでキャンセルできます)");	//windowテキスト
			PlayMovie("./asset/video/tutorial.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);	//チュートリアルを再生する(ボタンキャンセルあり)
			windowFlag = 0;
		}
		else {  // ゲームの終了
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