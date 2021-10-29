#include "main.h"
#include "stdafx.h"


int syatekimain(int font, int bgm, int effect, int calling_check) {
	int windowFlag = 0;  // 現在のウィンドウを管理するフラグ
	int FramePerSecond = 60;//fps
	int score;	//ゲームのスコア
	LONGLONG nowtime, prevtime;//現在時間
	int hit_flag = 0;//景品を当てたフラグ

	std::random_device seed;//乱数生成器
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice;

	int shot = LoadSoundMem("./asset/effect/break.ogg");
	int error = LoadSoundMem("./asset/effect/error.ogg");
	int hazure = LoadSoundMem("./asset/effect/hazure.ogg");


	std::vector<int> button_handle{};//ボタン
	makeImageHandle(button_handle, "./asset/image/start.png", "./asset/image/start.png");
	std::vector<int> gun_handle{};//aim
	makeImageHandle(gun_handle, "./asset/image/aim.png");
	std::vector<int> keihin_handle{};//
	makeImageHandle(keihin_handle, "./asset/image/mato1.png", "./asset/image/mato2.png", "./asset/image/mato3.png", "./asset/image/break.png");
	std::vector<int> button_back_handle{}; // 戻るボタン
	makeImageHandle(button_back_handle, "./asset/image/back.png", "./asset/image/back.png"); // 戻るボタン

	unsigned int keihin_num = 10; //景品の数
	Goldfish keihin(900, 400, true, keihin_handle);
	Aim gun(900, 320, true, gun_handle);
	ObjGroup<Goldfish> keihin_group; //景品
	std::vector<int> keihin_frames(keihin_num); //景品の経過フレームに関する変数
	keihin_group.addcpy(keihin, keihin_num);
	std::string username;
	InputText input_username(500, 300, 40);
	Ranking shateki_ranking("./asset/result/shateki_result.csv");
	Ranking kingyo_ranking("./asset/result/kingyo_result.csv");
	std::vector<std::string> tmp;

	//タイトル画面のボタンの配置
	int px, py;
	int click_event, button_type, cx, cy, log_type;
	Button button_start(300, 500, button_handle);
	Button button_result(550, 500, button_handle);	//設定ボタン
	Button button_gotokingyo(750, 500, button_handle);	//射的ゲームへ行くボタン
	Button button_back(520, 400, button_back_handle); // 戻るボタン

	KeyInput input({ KEY_INPUT_Z, KEY_INPUT_RIGHT, KEY_INPUT_LEFT });

	prevtime = GetNowHiPerformanceCount();
	Timer timer(900);
	Timer timer2(240);
	Timer taiki_timer(180);
	Timer stan(60);
	bool able_shoot = true;//リロード完了
	int back_img = LoadGraph("./asset/image/syateki_back.jpg");	//ゲーム中の背景
	int back_black = LoadGraph("./asset/image/black_toumei.png");
	int title_img = LoadGraph("./asset/image/syateki_title.jpg");

	int count_Font_big = CreateFontToHandle("PixelMplus10 Regular", 400, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int count_Font_mid = CreateFontToHandle("PixelMplus10 Regular", 200, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int count_Font_small = CreateFontToHandle("PixelMplus10 Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	
	/* 初期化処理 */
	shateki_ranking.readAll();
	kingyo_ranking.readAll();
	input_username.NmlStr = GetColor(255, 255, 255);
	input_username.NmlCur = GetColor(255, 255, 255);
	if (calling_check == 0) PlaySoundFile("./asset/bgm/maou_minzoku9.ogg", DX_PLAYTYPE_LOOP); // bgmを読み込む

	/* ゲームループ */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // 表示画面を裏に
		ClearDrawScreen();  // 画面全体をクリア

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);
		input();

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
				score = 0;
				timer.reset();
				timer2.reset();
				taiki_timer.reset();
				stan.end();
				able_shoot = true;
				windowFlag = 4;	//ユーザー名入力
			}
			if (button_result.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 2;	//結果表示
			}
			if (button_gotokingyo.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				return 0; //金魚すくいゲームへ遷移
			}
		}
		else if (windowFlag == 1) { // ゲーム中のウィンドウ
			SetMainWindowText("射的ゲーム(ゲーム中)");	//windowテキスト
			DrawGraph(0, 0, back_img, FALSE);

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
				if (able_shoot)
				{
					hit_flag = 0;
					for (int i = 0; i < (int)keihin_num; i++) {
						if (keihin_group[i].isCought(gun) && keihin_group[i].state < 3) {
							PlaySoundMem(shot, DX_PLAYTYPE_BACK);
							keihin_group[i].state = 3;
							score++;
							hit_flag = 1;
						}
					}
					stan.reset();
					able_shoot = false;
					if (hit_flag) PlaySoundMem(shot, DX_PLAYTYPE_BACK);
					else PlaySoundMem(hazure, DX_PLAYTYPE_BACK);
				}
				else
				{
					PlaySoundMem(error, DX_PLAYTYPE_BACK);
				}
			}

			if (stan() == 0)able_shoot = true;
			//60秒たったら終了
			if (timer() == 0) {

				windowFlag = 3; // 60秒たったら終了しスコア表示へ
				shateki_ranking.insert(username, score);
			}
			else {
        
				DrawFormatStringToHandle(500, 50, GetColor(120, 120, 120), count_Font_small, "%d", timer() / 60);
				DrawFormatStringToHandle(1100, 550, GetColor(120, 120, 120), count_Font_small, "%d", score);
				if (stan() > 0 && stan() <= 60) {
					DrawStringToHandle(100, 200, "リロード", GetColor(0, 0, 255), count_Font_mid);

				}
			}
			timer.update();
			stan.update();
		}
		else if (windowFlag == 2)
		{
			/* 金魚すくいのランキング */
			DrawGraph(0, 0, title_img, TRUE);
			DrawGraph(0, 0, back_black, TRUE);
			DrawFormatStringToHandle(400, 10, GetColor(255, 255, 0), font, "金魚すくいランキングTOP15");
			for (unsigned int i = 0; i < 15 && i < kingyo_ranking.column_num(); ++i)
			{
				tmp = kingyo_ranking.readAt(i);
				DrawStringToHandle(200, 40 * (i + 1) + 10, (std::to_string(i + 1) + "位").c_str(), GetColor(255, 255, 255), font);
				DrawStringToHandle(550, 40 * (i + 1) + 10, (tmp[0] + " 様").c_str(), GetColor(255, 255, 255), font);
				DrawStringToHandle(900, 40 * (i + 1) + 10, (tmp[1] + "点").c_str(), GetColor(255, 255, 255), font);
			}
			if (click_event != -1)
			{
				windowFlag = 0;
			}
			if (input.GetKeyDown(KEY_INPUT_LEFT))
			{
				windowFlag = 6;
			}
		}
		else if (windowFlag == 6)
		{
			/* 射的ゲームのランキング */
			DrawGraph(0, 0, title_img, TRUE);
			DrawGraph(0, 0, back_black, TRUE);
			DrawFormatStringToHandle(400, 10, GetColor(255, 255, 0), font, "射的ランキングTOP15");
			for (unsigned int i = 0; i < 15 && i < shateki_ranking.column_num(); ++i)
			{
				tmp = shateki_ranking.readAt(i);
				DrawStringToHandle(200, 40 * (i + 1) + 10, (std::to_string(i + 1) + "位").c_str(), GetColor(255, 255, 255), font);
				DrawStringToHandle(550, 40 * (i + 1) + 10, (tmp[0] + " 様").c_str(), GetColor(255, 255, 255), font);
				DrawStringToHandle(900, 40 * (i + 1) + 10, (tmp[1] + "点").c_str(), GetColor(255, 255, 255), font);
			}
			if (click_event != -1)
			{
				windowFlag = 0;
			}
			if (input.GetKeyDown(KEY_INPUT_RIGHT))
			{
				windowFlag = 2;
			}
		}
		else if (windowFlag == 3) {
			DrawGraph(0, 0, back_img, TRUE);
			DrawGraph(0, 0, back_black, TRUE);
			SetMainWindowText("結果");	//windowテキスト
			DrawBoxAA(320, 180, 320 * 3, 180 * 3, GetColor(218, 165, 32), true);
			button_back.draw();
			DrawFormatStringToHandle(520, 200, GetColor(120, 120, 120), font, (username + "の結果").c_str());
			DrawFormatStringToHandle(520, 350, GetColor(120, 120, 120), font, "スコア： %d", score);
			DrawFormatStringToHandle(590, 425, GetColor(120, 120, 120), font, "戻る");
			if (score > 0) {
				DrawFormatStringToHandle(520, 250, GetColor(120, 120, 120), font, "%d個打ち抜けました!", score);
			}
			else {
				DrawFormatStringToHandle(520, 250, GetColor(120, 120, 120), font, "残念!");
			}
			if (button_back.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				PlaySoundMem(effect, DX_PLAYTYPE_BACK);
				windowFlag = 0; // スタート画面へ戻る
			}
			timer2.update();
		}
		else if (windowFlag == 4)
		{
			/* ユーザー名入力 */
			DrawGraph(0, 0, title_img, TRUE);
			DrawGraph(0, 0, back_black, TRUE);
			input_username.draw();
			if (input_username.entered())
			{
				windowFlag = 5;
				input_username.text(username);
			}
		}
		else if (windowFlag == 5) {
			DrawGraph(0, 0, back_img, FALSE);

			DrawGraph(0, 0, back_black, TRUE);


			if (taiki_timer() == 0) windowFlag = 1; //ゲームへ行く
			if(taiki_timer() < 10)  DrawStringToHandle(200, 200, "Start!",GetColor(255, 0, 0),count_Font_big);
      
			else DrawFormatStringToHandle(500, 250, GetColor(255, 0, 0), count_Font_big, "%d", taiki_timer() / 60 + 1);

			DrawStringToHandle(100, 100, "注意:弾のリロ-ド中は打てません", GetColor(255,255,0 ), count_Font_small);

			taiki_timer.update();
		}
		else if (windowFlag == 10) {  // ゲームの終了
			calling_check = 1;
			return 1;
		}
		else {
			return 1;
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
	return 1;
}