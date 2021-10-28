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

	std::vector<int> button_handle{};//ボタン
	makeImageHandle(button_handle, "./asset/image/uncheck.png", "./asset/image/checked.png");
	std::vector<int> gun_handle{};//aim
	makeImageHandle(gun_handle, "./asset/image/aim.png");	//TODO:画像を置換する
	std::vector<int> keihin_handle{};//
	makeImageHandle(keihin_handle, "./asset/image/mato.png");

	unsigned int keihin_num = 10; //景品の数
	Goldfish keihin(900, 400, true, keihin_handle);
	Aim gun(900, 380, true, gun_handle);
	ObjGroup<Goldfish> keihin_group; //景品
	keihin_group.addcpy(keihin, keihin_num);
	for (unsigned int i = 0; i < keihin_num; i++) {
		if (i < 5) {//上段
			keihin_group[i].x = 300 + 100 * i;
			keihin_group[i].y = 400;
		}
		else {//下段
			keihin_group[i].x = 300 + 100 * (i - 5);
			keihin_group[i].y = 600;
		}
	}

	//タイトル画面のボタンの配置
	int px, py;
	int click_event, button_type, cx, cy, log_type;
	Button button_start(300, 500,button_handle);	//STARTボタン
	StringObj start_obj(350, 550, "スタート", GetColor(120, 120, 120), font);
	Button button_result(500, 500,button_handle);	//設定ボタン
	StringObj result_obj(550, 550, "結果", GetColor(120, 120, 120), font);
	Button button_gotokingyo(700, 500,button_handle);	//射的ゲームへ行くボタン
	StringObj gotokingyo_obj(700, 550, "金魚すくい", GetColor(120, 120, 120), font);


	KeyInput input(KEY_INPUT_Z);

	prevtime = GetNowHiPerformanceCount();
	Timer timer(900);
	Timer timer2(240);
	int back_img = LoadGraph("./asset/image/syateki_back.jpg");	//ゲーム中の背景
	int count_Font = CreateFontToHandle("Mplus1-Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

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
			gun.next();
			
			gun.draw();
			keihin_group.draw();

			if (input.GetKeyDown(KEY_INPUT_Z)) {
				/* zキーが押された */
				SetMainWindowText("射的ゲーム(タイトル)");	//windowテキスト
				std::vector<int> index_management;
				for (int i = 0; i < (int)keihin_num; i++) {
					if (keihin_group[i].isCought(gun, mt, dice)) {
						index_management.push_back(i);
					}
				}
				for (int i = (int)index_management.size() - 1; i >= 0; i--) {
					keihin_group.destroy(index_management[i]);
					keihin_num--;
					score++;
					
				}
				

			}

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
				DrawFormatStringToHandle(500, 50, GetColor(120, 120, 120), count_Font, "残り%d秒", timer() / 60);
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