#include "main.h"
#include "stdafx.h"
#define pi 3.14159265358979

void kingyomain(int font,int bgm,int effect, int calling_check) {
	/* ゲーム全体を管理する変数 */
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60;//fps
	LONGLONG nowtime, prevtime = GetNowHiPerformanceCount();
	int score = 0; //ゲームのスコア
	unsigned int kingyo_num = 5;// 金魚数指定用変数(Objgroup.addcopy用)
	unsigned int telescope_num = 1;//出目金数指定用変数(Objgroup.addcopy用)
	std::random_device seed;//乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);
	int px, py; // マウスポインタの座標
	int click_event, button_type, cx, cy, log_type;	// マウスポインタのイベント管理用変数
	KeyInput z_push(KEY_INPUT_Z); // zキーが押されたかどうかを管理する変数
	Timer timer60sec(1800); // ゲームの制限時間
	Timer timer80sec(2400); // 結果 -> タイトルまでに使うタイマー

	/* ゲーム用データの読み込み */
	int count_Font = CreateFontToHandle("Mplus1-Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int back_img = LoadGraph("./asset/image/background.png"); // 背景画像
	int title_img = LoadGraph("./asset/image/title.png");
	std::vector<int> button_handle{}; /* ボタンのデータ */
	std::vector<int> kingyo_handle{}; /* 金魚の画像データ */
	std::vector<int> telescope_handle{}; /* 出目金の画像データ */
	std::vector<int> poi_handle{}; /* ポイの画像データ */
	makeImageHandle(button_handle, "./asset/image/start.png", "./asset/image/start.png");
	makeImageHandle(kingyo_handle, "./asset/image/kingyo.png", "./asset/image/kingyo_left.png","./asset/image/kingyo_right.png");
	makeImageHandle(telescope_handle, "./asset/image/Telescope.png", "./asset/image/Telescope_left.png", "./asset/image/Telescope_right.png");
	makeImageHandle(poi_handle, "./asset/image/poi.png");

	Goldfish kingyo(500, 500, pi/2,true, kingyo_handle); //最初のコピー元金魚
	kingyo.SetMovement(MOV_OPTION::WAVE, 100.0, 10.0);
	Goldfish telescope(500, 400, true, telescope_handle); //最初のコピー元出目金

	Button button_start(400, 300, false, button_handle);	//金魚掬いのSTARTボタン
	Poi poi(100, 100, true, poi_handle);//ポイの宣言
	ObjGroup<Goldfish> kingyo_group;
	ObjGroup<Goldfish> telescope_group;
	kingyo_group.addcpy(kingyo, kingyo_num);
	telescope_group.addcpy(telescope, 1);

	//bgmを読み込む
	if (calling_check == 0) {
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	}

	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア

		z_push(); //zキーが押されたかどうか(捕獲)

		GetMousePoint(&px, &py);//マウスポインタ(ポイ)の位置ゲット
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//エラーが起きたらループをぬける

		if (windowFlag == 0) {  // メニューウィンドウ
			SetMainWindowText("金魚すくい(タイトル)");	//windowテキスト
			DrawGraph(0, 0, title_img, TRUE);

			button_start.draw();	//ゲームスタート
			button_start.next(px, py);
			if (button_start.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//金魚すくいスタート
			}
		}
		else if (windowFlag == 1) { // ゲーム中のウィンドウ
			SetMainWindowText("金魚すくい(ゲーム中)");	//windowテキスト
			DrawGraph(0, 0, back_img, TRUE);

			kingyo_group.draw();
			kingyo_group.Next();
			telescope_group.draw();
			telescope_group.Next();
			
			//60秒たったら終了
			if (timer60sec() == 0) {
				SetMainWindowText("スコア表示");	//windowテキスト
				DrawFormatString(500, 200, GetColor(120, 120, 120), "スコアは%dです", score, font);
				if (timer80sec() == 0) {
					windowFlag = 0;
				}
				timer80sec.update();
			}
			else {
				DrawFormatStringToHandle(520, 60, GetColor(120, 120, 120), count_Font, "のこり%d秒", timer60sec() / 60);
			}
			timer60sec.update();
		}
		else if (windowFlag == 2) {
			SetMainWindowText("結果");	//windowテキスト
		}
		else if(windowFlag==10) {	//射的ゲームへ
			syatekimain(font, bgm, effect,calling_check);
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