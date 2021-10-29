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

	int shot = LoadSoundMem("./asset/effect/break.ogg");

	std::vector<int> button_handle{};//ボタン
	makeImageHandle(button_handle, "./asset/image/start.png", "./asset/image/start.png");
	std::vector<int> gun_handle{};//aim
	makeImageHandle(gun_handle, "./asset/image/aim.png");	//TODO:画像を置換する
	std::vector<int> keihin_handle{};//
	makeImageHandle(keihin_handle, "./asset/image/mato1.png", "./asset/image/mato2.png", "./asset/image/mato3.png", "./asset/image/break.png");

	unsigned int keihin_num = 10; //景品の数
	Goldfish keihin(900, 400, true, keihin_handle);
	Aim gun(900, 320, true, gun_handle);
	ObjGroup<Goldfish> keihin_group; //景品
	std::vector<int> keihin_frames(keihin_num); //景品の経過フレームに関する変数
	keihin_group.addcpy(keihin, keihin_num);


	//タイトル画面のボタンの配置
	int px, py;
	int click_event, button_type, cx, cy, log_type;
	Button button_start(300, 500, button_handle);
	Button button_result(550, 500, button_handle);	//設定ボタン
	Button button_gotokingyo(750, 500, button_handle);	//射的ゲームへ行くボタン


	KeyInput input(KEY_INPUT_Z);

	prevtime = GetNowHiPerformanceCount();
	Timer timer(900);
	Timer timer2(240);
	Timer taiki_timer(180);
	int back_img = LoadGraph("./asset/image/syateki_back.jpg");	//ゲーム中の背景
	int back_black = LoadGraph("./asset/image/black_toumei.png");
	int title_img = LoadGraph("./asset/image/syateki_title.jpg");

	int count_Font_big = CreateFontToHandle("Mplus1-Regular", 400, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int count_Font_small = CreateFontToHandle("Mplus1-Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	

	if (calling_check == 0) PlaySoundFile("./asset/bgm/maou_minzoku9.ogg", DX_PLAYTYPE_LOOP); // bgmを読み込む

	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//エラーが起きたらループをぬける

		if (windowFlag == 0) {  // メニューウィンドウ
			DrawGraph(0, 0, title_img, FALSE);
			SetMainWindowText("射的ゲーム(タイトル)");	//windowテキスト


			button_start.draw();	//ゲームスタート
			button_start.next(px, py);
			button_result.draw();	//結果画面
			button_result.next(px, py);
			button_gotokingyo.draw();		//射的ゲームへ
			button_gotokingyo.next(px, py);
			if (button_start.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				for (unsigned int i = 0; i < keihin_num; i++) {
					if (i < keihin_num / 2) {//上段
						keihin_group[i].state = dice(mt) % 3;
						keihin_group[i].x = 300 + 120 * i;
						keihin_group[i].y = 350;
					}
					else {//下段
						keihin_group[i].state = dice(mt) % 3;
						keihin_group[i].x = 300 + 120 * (i - 5);
						keihin_group[i].y = 580;
					}
					keihin_frames[i] = 4;
					keihin_group.Tag(i) = 0;
				}
				timer.reset();
				windowFlag = 5;	//射的ゲーム待機スタート
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
			DrawGraph(0, 0, back_img, FALSE);

			input();
			gun.next();

			gun.draw();
			keihin_group.draw(0);

			for (unsigned int i = 0; i < keihin_group.size(); ++i)
			{
				if (keihin_group[i].state == 3 && keihin_group.Tag(i) == 0)
				{
					keihin_frames[i]--;
					if (keihin_frames[i] <= 0)keihin_group.Tag(i) = 1;
				}
			}

			if (input.GetKeyDown(KEY_INPUT_Z)) {
				/* zキーが押された */
				SetMainWindowText("射的ゲーム(タイトル)");	//windowテキスト
				for (int i = 0; i < (int)keihin_num; i++) {
					if (keihin_group[i].isCought(gun, mt, dice) && keihin_group[i].state < 3) {
						PlaySoundMem(shot, DX_PLAYTYPE_BACK);
						keihin_group[i].state = 3;
						score++;
					}
				}
			}

			//60秒たったら終了
			if (timer() == 0) {
				SetMainWindowText("スコア表示");	//windowテキスト
				if (score > 0) {
					DrawFormatString(500, 200, GetColor(120, 120, 120), "%d打ち抜けました!", score, font);
				}
				else {
					DrawFormatString(500, 200, GetColor(120, 120, 120), "残念!", font);
				}
				if (timer2() == 0) {
					windowFlag = 0;
				}
				timer2.update();
			}
			else {
				DrawFormatStringToHandle(500, 50, GetColor(120, 120, 120), count_Font_small, "残り%d秒", timer() / 60);
				DrawFormatStringToHandle(1100, 550, GetColor(120, 120, 120), count_Font_small, "%d", score);
			}
			timer.update();
		}
		else if (windowFlag == 3) {
			SetMainWindowText("結果");	//windowテキスト
		}

		else if (windowFlag == 5) {
			DrawGraph(0, 0, back_img, FALSE);

			DrawGraph(0, 0, back_black, TRUE);

			if (taiki_timer() == 0) windowFlag = 1; //ゲームへ行く
			if(taiki_timer() < 10)  DrawStringToHandle(200, 200, "Start!",GetColor(255, 0, 0),count_Font_big);
			else DrawFormatStringToHandle(500, 250, GetColor(255, 0, 0), count_Font_big, "%d", taiki_timer() / 60 + 1);

			taiki_timer.update();
		}
		else if (windowFlag == 10) {  // ゲームの終了
			calling_check = 1;
			kingyomain(font, bgm, effect, calling_check);
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