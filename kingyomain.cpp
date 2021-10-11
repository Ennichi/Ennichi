#include "main.h"
#include "stdafx.h"

void kingyomain(int font,int bgm,int effect) {
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60;//fps
	int score = 0;	//ゲームのスコア
	LONGLONG nowtime, prevtime;//現在時間

	std::random_device seed;//乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<int> handle{};
	makeImageHandle(handle, "./asset/image/goldfish_open.png", "./asset/image/goldfish_close.png");

	std::vector<int> button_handle{};
	makeImageHandle(button_handle, "./asset/image/button2.png", "./asset/image/button1.png");

	std::vector<int> poi_handle{};
	makeImageHandle(poi_handle, "./asset/image/poi.png");

	int px, py;
	int click_event, button_type, cx, cy, log_type;	
	Button button1(400, 240, false, button_handle);
	Goldfish *fish1=new Goldfish(300, 300, 0, true, handle); //金魚
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
	Timer timer(3600);
	Timer timer2(2400);
	//bgmを読み込む
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	int back_img = LoadGraph("./asset/image/background.png");
	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//エラーが起きたらループをぬける

		if (windowFlag == 0) {  // メニューウィンドウ
			SetMainWindowText("金魚すくい(タイトル)");	//windowテキスト
			
			DrawStringToHandle(500, 120, "金魚ゲーム", GetColor(120,120,120), font);
			button1.draw();	//ゲームスタート
			button1.next(px, py);

			if (button1.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//ボタン用
			}
		}
		else if (windowFlag == 1) { // ゲーム中のウィンドウ
			SetMainWindowText("金魚すくい(ゲーム中)");	//windowテキスト
			DrawGraph(0, 0, back_img, TRUE);
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
				SetMainWindowText("スコア表示");	//windowテキスト
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