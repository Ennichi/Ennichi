﻿#include "main.h"
#include "stdafx.h"

void kingyomain(int font, int bgm, int effect, int calling_check) {
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60;//fps
	int score = 0;	//ゲームのスコア
	LONGLONG nowtime, prevtime;//現在時間

	std::random_device seed;//乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<int> handle{};
	makeImageHandle(handle, "./asset/image/kingyo.png", "./asset/image/kingyo_left.png", "./asset/image/kingyo_right.png");
	handle.insert(std::next(handle.begin(), 2), handle.at(0));

	std::vector<int> telescope_handle{};
	makeImageHandle(telescope_handle, "./asset/image/Telescope.png", "./asset/image/Telescope_left.png", "./asset/image/Telescope_right.png");
	telescope_handle.insert(std::next(telescope_handle.begin(), 2), telescope_handle.at(0));

	std::vector<int> button_handle{};
	makeImageHandle(button_handle, "./asset/image/start.png", "./asset/image/start.png");

	std::vector<int> poi_handle{};
	makeImageHandle(poi_handle, "./asset/image/poi.png");

	int px, py;
	int click_event, button_type, cx, cy, log_type;
	Button button_start(400, 300, false, button_handle);	//STARTボタン

	Goldfish* fish1 = new Goldfish(300, 300, 0, true, handle); //金魚
	Goldfish fish2(100, 0, DX_PI * 3.0 / 4.0, true, handle);
	Poi first(500, 500, true, poi_handle);//ポイ
	Goldfish fish3 = *fish1;

	Goldfish* telescope_fish1 = new Goldfish(400, 500, 0, true, telescope_handle); //出目金
	Goldfish telescope_fish3 = *telescope_fish1;

	ObjGroup<Goldfish> fish4;

	fish1->setDifficulty(10);
	fish1->animsp = 30;

	KeyInput input(KEY_INPUT_Z);

	fish1->setSpeed(0.5, 1.0);//スピード設定

	//fish4.addcpy(*fish1, 10);
	fish4.addcpy(*fish1, *fish1, *fish1);
	for (int i = 0; i < 3; ++i)
	{
		fish4[i].angle -= (double)i * 0.2;
	}
	fish4.destroy(1);
	prevtime = GetNowHiPerformanceCount();
	int clock = GetNowCount();	//現在時刻の取得
	Timer timer(1800);
	Timer timer2(180);
	//bgmを読み込む
	if (calling_check == 0) {
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	}

	int back_img = LoadGraph("./asset/image/background.png");
	int title_img = LoadGraph("./asset/image/title.png");

	int count_Font = CreateFontToHandle("Mplus1-Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	unsigned int count_play = 1;	//プレーヤーのカウント
	const std::string buff1 = "金魚すくい! あなたは";
	const std::string buff2 = "人目のプレーヤーです";
	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//エラーが起きたらループをぬける

		if (windowFlag == 0) {  // メニューウィンドウ
			SetMainWindowText("金魚すくい(タイトル)");	//windowテキスト
			DrawGraph(0, 0, title_img, TRUE);

			button_start.draw();	//ゲームスタート
			button_start.next(px, py);
			if (button_start.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				PlaySoundMem(effect, DX_PLAYTYPE_BACK);
				windowFlag = 1;	//金魚すくいスタート
			}
		}
		else if (windowFlag == 1) { // ゲーム中のウィンドウ
			//プレーヤーの整数型をstringへキャストする
			std::string players(std::to_string(count_play));
			std::string buff3 = buff1 + players + buff2;
			SetMainWindowText(buff3.c_str());	//windowテキスト
			if (DrawGraph(0, 0, back_img, TRUE) == -1) {
				printfDx("Error");
			}
			input();

			if (fish1 != NULL)
			{
				fish1->Next();
				fish1->draw();
				if (input.GetKeyDown(KEY_INPUT_Z) == 1 && fish1->isCought(first, mt, dice)) {
					delete fish1;
					fish1 = NULL;
					score++;
				}
			}
			if (telescope_fish1 != NULL) {
				telescope_fish1->Next();
				telescope_fish1->draw();
				if (input.GetKeyDown(KEY_INPUT_Z) == 1 && telescope_fish1->isCought(first, mt, dice)) {
					delete telescope_fish1;
					telescope_fish1 = NULL;
					score++;
				}
			}
			fish3.Next();
			fish3.draw();
			fish4.Next();
			fish4.draw();
			first.point_change();
			first.draw();

			//60秒たったら終了
			if (timer() == 0) {
				SetMainWindowText("スコア表示");	//windowテキスト
				DrawFormatStringToHandle(520, 60, GetColor(120, 120, 120), font, "%d匹捕まえたよ", score);
				if (timer2() == 0) {
					windowFlag = 0;
					timer2.reset();
					timer.reset();
					count_play++;
					PlaySoundMem(effect, DX_PLAYTYPE_BACK);

				}
				timer2.update();

			}
			else {
				DrawFormatStringToHandle(520, 60, GetColor(120, 120, 120), count_Font, "のこり%d秒", timer() / 60);
			}
			timer.update();
		}
		else if (windowFlag == 2) {
			SetMainWindowText("結果");	//windowテキスト

		}
		else if (windowFlag == 10) {	//射的ゲームへ
			syatekimain(font, bgm, effect, calling_check);
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