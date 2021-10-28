﻿#include "main.h"
#include "stdafx.h"
#define pi 3.14159265358979

void kingyomain(int font, int bgm, int effect, int calling_check) {
	/* ゲームの基本データ */
	int windowFlag = 0; // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60; //fps
	LONGLONG nowtime, prevtime = GetNowHiPerformanceCount(); // fps管理用変数
	int count_play = 1; // プレイヤーカウンタ
	int cought_kingyo = 0; // 捕まえた金魚の数
	int cought_telescope = 0; // 捕まえた出目金の数
	std::random_device seed; // 乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);
	std::uniform_int_distribution<> rand_four(0, 3);//0~3を返す乱数
	std::uniform_int_distribution<> rand_posx(101, 1080);//x座標を決める乱数
	std::uniform_int_distribution<> rand_posy(101, 500);//y座標を決める乱数
	int px, py; // マウスポインタの座標
	int click_event, button_type, cx, cy, log_type;	// マウスポインタのイベント管理用変数
	KeyInput z_push(KEY_INPUT_Z); // zキーが押されたかどうかを管理する変数
	Timer timer60sec(1800); // ゲームの制限時間
	Timer timer80sec(2400); // 結果 -> タイトルまでに使うタイマー
	size_t kingyo_num = 5; // 金魚の数
	size_t telescope_num = 100; //出目金の数
	unsigned char poi_num_remaining = 5;// 残りポイの数
	bool poi_destroy; // ポイが破れるフラグ
	int kingyo_score = 2; // 金魚一匹捕まえたときのスコア
	int telescope_score = 3; // 出目金一匹捕まえたときのスコア
	const std::string buff1 = "金魚すくい! あなたは";
	const std::string buff2 = "人目のプレーヤーです";
	
	/* ゲームで使用するデータの読み込み */
	int count_Font = CreateFontToHandle("Mplus1-Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8); // フォントデータ
	int back_img = LoadGraph("./asset/image/background.png"); // 背景画像
	int title_img = LoadGraph("./asset/image/title.png"); // タイトル画面
	std::vector<int> button_handle{}; // ボタンのデータ
	std::vector<int> button_back_handle{}; // 戻るボタン
	std::vector<int> kingyo_handle{}; // 金魚の画像データ
	std::vector<int> telescope_handle{}; // 出目金の画像データ
	std::vector<int> poi_handle{}; // ポイの画像データ
	makeImageHandle(button_handle, "./asset/image/start.png", "./asset/image/start.png"); // ハンドルの読み込み
	makeImageHandle(button_back_handle, "./asset/image/back.png", "./asset/image/back.png"); // 戻るボタン
	makeImageHandle(kingyo_handle, "./asset/image/kingyo.png", "./asset/image/kingyo_left.png", "./asset/image/kingyo.png", "./asset/image/kingyo_right.png");
	makeImageHandle(telescope_handle, "./asset/image/Telescope.png", "./asset/image/Telescope_left.png", "./asset/image/Telescope.png", "./asset/image/Telescope_right.png");
	makeImageHandle(poi_handle, "./asset/image/poi.png", "./asset/image/Telescope.png");
	Goldfish kingyo(500, 500, pi/2,true, kingyo_handle); // コピー元金魚
	Goldfish telescope(500, 400, true, telescope_handle); // コピー元出目金
	Obj poiFake(1200, 10, false, poi_handle);// 当たり判定の無いポイ
	poiFake.setScale(0.25);

	Button button_start(400, 300, button_handle); // 金魚掬いのSTARTボタン
	Button button_back(520, 400, button_back_handle); // 戻るボタン
	Poi poi(100, 100, true, poi_handle); // ポイ
	ObjGroup<Goldfish> kingyo_group; // 金魚のグループ
	ObjGroup<Goldfish> telescope_group; // 出目金のグループ
	ObjGroup<Obj> remaining_poi; //ポイの残り個数表示
	kingyo_group.addcpy(kingyo, (unsigned int)kingyo_num); // グループ初期化
	telescope_group.addcpy(telescope, (unsigned int)telescope_num);
	remaining_poi.addcpy(poiFake, (unsigned int)poi_num_remaining);

	/* ゲーム開始前の初期化処理 */
	if (calling_check == 0) PlaySoundMem(bgm, DX_PLAYTYPE_LOOP); // bgmを読み込む


	/* ゲームループ */
	while (1) {
		/* ゲームループ毎の初期化処理 */
		SetDrawScreen(DX_SCREEN_BACK); // 表示画面を裏に
		ClearDrawScreen(); // 画面全体をクリア
		z_push(); // zキーが押されたかどうか(捕獲)
		GetMousePoint(&px, &py); // マウスポインタ(ポイ)の位置ゲット
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type); // マウスのクリックがあったか

		if (ProcessMessage() == -1) break; //エラーが起きたらループをぬける

		if (windowFlag == 0) {
			/* タイトル画面の処理 */
			/* 画面表示 */
			SetMainWindowText("金魚すくい(タイトル)"); //windowテキスト
			DrawGraph(0, 0, title_img, TRUE);
			button_start.draw(); // スタートボタンの表示

			/* 次状態の管理 */
			if (button_start.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				PlaySoundMem(effect, DX_PLAYTYPE_BACK);
				windowFlag = 1;	//金魚すくいスタート

				for (unsigned int i = 0; i < kingyo_num; i++) {
					/* 金魚グループに関する初期化 */
					kingyo_group[i].setSpeed(1.0, 3.0); // 金魚のスピードを設定
					kingyo_group[i].setDifficulty(1);
					kingyo_group[i].animsp = 30; // アニメーションの設定
					kingyo_group[i].spawn_position(dice(mt) % 980 + 100, dice(mt) % 400 + 100);//範囲内に収まるように補正
					if (i % 2 == 0) kingyo_group[i].SetMovement(MOV_OPTION::CIRCLE, 300, 1.0);
					if (i % 3 == 0)kingyo_group[i].SetMovement(MOV_OPTION::WAVE, 100, 100);
				}
				for (unsigned int i = 0; i < telescope_num; i++) {
					/* 出目金グループに関する初期化 */
					telescope_group[i].setSpeed(1.0, 3.0); // 出目金のスピードを設定
					telescope_group[i].setDifficulty(1);
					telescope_group[i].animsp = 30; // アニメーションの設定
					if (i % 2 == 0) telescope_group[i].SetMovement(MOV_OPTION::CIRCLE, 300, 1.0);
					if (i % 3 == 0) telescope_group[i].SetMovement(MOV_OPTION::WAVE, 100, 100);
				}
				for (unsigned char i = 0; i < poi_num_remaining; ++i)
				{
					/* 残りポイ数表示用オブジェクトグループ */
					remaining_poi[i].x -= 40 * i; // ポイの位置をずらす
				}

				timer60sec.reset();
				cought_kingyo = 0;
				cought_telescope = 0;
				count_play++;
			}
			button_start.next(px, py);
		}
		else if (windowFlag == 1) {
			/* ゲーム中の処理 */
			/* 画面の描画 */
			std::string buff3 = buff1 + std::to_string(count_play) + buff2;
			SetMainWindowText(buff3.c_str());	//windowテキスト
			DrawGraph(0, 0, back_img, TRUE); // 背景表示
			poi.draw();
			kingyo_group.draw();
			telescope_group.draw();
			for (unsigned char i = 0; i < poi_num_remaining; ++i)remaining_poi[i].draw();

			/* 次状態の管理 */
			if (timer60sec() == 0 || poi_num_remaining == 0) windowFlag = 2; // 60秒たったら終了しスコア表示へ
			else DrawFormatStringToHandle(520, 60, GetColor(120, 120, 120), count_Font, "のこり%d秒", timer60sec() / 60); // 残り時間表示
			if (z_push.GetKeyDown(KEY_INPUT_Z)) {
				/* zキーが押された */
				poi_destroy = true;
				for (int i = 0; i < (int)kingyo_group.size(); i++) {
					if (kingyo_group[i].isCought(poi, mt, dice)) {
						switch (rand_four(mt))
						{
						case 0:
							kingyo_group[i].spawn_position(1080, rand_posy(mt));
							break;
						case 1:
							kingyo_group[i].spawn_position(101, rand_posy(mt));
							break;
						case 2:
							kingyo_group[i].spawn_position(rand_posx(mt), 500);
							break;
						case 3:
							kingyo_group[i].spawn_position(rand_posx(mt), 101);
							break;
						default:
							break;
						}
						cought_kingyo++;
						poi_destroy = false;
					}
				}
				for (int i = 0; i < (int)telescope_group.size(); i++) {
					if (telescope_group[i].isCought(poi, mt, dice)) {
						switch (rand_four(mt))
						{
						case 0:
							telescope_group[i].spawn_position(1080, rand_posy(mt));
							break;
						case 1:
							telescope_group[i].spawn_position(101, rand_posy(mt));
							break;
						case 2:
							telescope_group[i].spawn_position(rand_posx(mt), 500);
							break;
						case 3:
							telescope_group[i].spawn_position(rand_posx(mt), 101);
							break;
						default:
							break;
						}
						cought_telescope++;
						poi_destroy = false;
					}
				}
				if (poi_destroy)
				{
					poi_num_remaining--;
				}
			}
			poi.point_change();
			kingyo_group.Next(); // オブジェクトの見た目の遷移
			telescope_group.Next();
			timer60sec.update(); // タイマー更新
		}
		else if (windowFlag == 2) {
			/* スコア表示 */
			/* 画面の描画 */
			SetMainWindowText("スコア表示");	//windowテキスト
			DrawGraph(0, 0, back_img, TRUE);
			DrawBoxAA(320, 180, 320 * 3, 180 * 3, GetColor(218, 165, 32), true);
			button_back.draw();
			DrawFormatStringToHandle(520, 60, GetColor(120, 120, 120), font, "%d匹捕まえたよ", cought_kingyo + cought_telescope);
			DrawFormatStringToHandle(520, 200, GetColor(120, 120, 120), font, "金魚 × %d匹", cought_kingyo);
			DrawFormatStringToHandle(520, 250, GetColor(120, 120, 120), font, "出目金 × %d匹", cought_telescope);
			DrawFormatStringToHandle(520, 300, GetColor(120, 120, 120), font, "スコア： %d", cought_kingyo * kingyo_score + cought_telescope * telescope_score);
			DrawFormatStringToHandle(590, 425, GetColor(120, 120, 120), font, "戻る");
			
			/* 次状態の管理 */
			if (button_back.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				PlaySoundMem(effect, DX_PLAYTYPE_BACK);
				windowFlag = 0; // スタート画面へ戻る
			}
			if (timer80sec() == 0) { // スコア表示時間を過ぎたら
				windowFlag = 0; // タイトル画面へ
				timer60sec.reset(); // タイマーのリセット
				timer80sec.reset();
				count_play++; // プレイ回数を増やす
				PlaySoundMem(effect, DX_PLAYTYPE_BACK); // 効果音
			}
			button_back.next(px, py);
			timer80sec.update(); // タイマーの更新
		}
		else if (windowFlag == 3) {
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