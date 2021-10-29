#include "main.h"
#include "stdafx.h"

void deleteImageHandle(std::vector<int>& vHandle) {
	for (size_t i = 0; i < vHandle.size(); i++) {
		DeleteGraph(vHandle[i]);
	}
}

int kingyomain(int font, int bgm, int effect, int calling_check) {
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
	KeyInput key_getter({ KEY_INPUT_Z, KEY_INPUT_RIGHT, KEY_INPUT_LEFT, KEY_INPUT_DOWN, KEY_INPUT_UP }); // zキーが押されたかどうかを管理する変数
	Timer timer60sec(1800); // ゲームの制限時間
	Timer timer80sec(2400); // 結果 -> タイトルまでに使うタイマー
	size_t kingyo_num = 5; // 金魚の数
	size_t telescope_num = 2; //出目金の数
	unsigned char poi_num_remaining = 5;// 残りポイの数
	bool poi_destroy; // ポイが破れるフラグ
	int kingyo_score = 2; // 金魚一匹捕まえたときのスコア
	int telescope_score = 3; // 出目金一匹捕まえたときのスコア
	int total_score;
	const std::string buff1 = "金魚すくい! あなたは";
	const std::string buff2 = "人目のプレーヤーです";
	std::string username;

	/* ゲームで使用するデータの読み込み */
	int count_Font = CreateFontToHandle("PixelMplus10 Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8); // フォントデータ
	int sound_hazure = LoadSoundMem("./asset/effect/poi_break.ogg");
	int back_img = LoadGraph("./asset/image/background.png"); // 背景画像
	int title_img = LoadGraph("./asset/image/title.png"); // タイトル画面
	int back_black = LoadGraph("./asset/image/black_toumei.png");
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
	Goldfish kingyo(500, 500, DX_PI/2,true, kingyo_handle); // コピー元金魚
	Goldfish telescope(500, 400, true, telescope_handle); // コピー元出目金
	Obj poiFake(1200, 10, false, poi_handle);// 当たり判定の無いポイ
	Obj kingyoFake(0, 0);//アニメーション用金魚
	InputText input_username(500, 300, 40);// 文字入力オブジェクト
	Ranking kingyo_ranking("./asset/result/kingyo_result.csv");
	poiFake.setScale(0.5);

	Button button_start(400, 300, button_handle); // 金魚掬いのSTARTボタン
	Button button_back(520, 400, button_back_handle); // 戻るボタン
	Poi poi(100, 100, true, poi_handle); // ポイ
	ObjGroup<Goldfish> kingyo_group; // 金魚のグループ
	ObjGroup<Goldfish> telescope_group; // 出目金のグループ
	ObjGroup<Obj> remaining_poi; //ポイの残り個数表示
	ObjGroup<Obj> kingyo_anim; //アニメーション用金魚のグループ
	std::vector<Timer> kingyo_anim_timer;
	kingyo_group.addcpy(kingyo, (unsigned int)kingyo_num); // グループ初期化
	telescope_group.addcpy(telescope, (unsigned int)telescope_num);
	remaining_poi.addcpy(poiFake, (unsigned int)poi_num_remaining);

	/* ゲーム開始前の初期化処理 */
	// if (calling_check == 0) PlaySoundFile("./asset/bgm/maou_minzoku9.ogg", DX_PLAYTYPE_LOOP); // bgmを読み込む

	for (unsigned char i = 0; i < poi_num_remaining; ++i)
	{
		/* 残りポイ数表示用オブジェクトグループ */
		remaining_poi[i].x -= 50 * i; // ポイの位置をずらす
	}

	input_username.NmlStr = GetColor(255, 255, 255);
	input_username.NmlCur = GetColor(255, 255, 255);
	kingyo_ranking.readAll();
	/* ゲームループ */
	while (1) {
		/* ゲームループ毎の初期化処理 */
		SetDrawScreen(DX_SCREEN_BACK); // 表示画面を裏に
		ClearDrawScreen(); // 画面全体をクリア
		key_getter(); // キー押下状態取得
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
				windowFlag = 3;	//ユーザー名入力
				input_username.set();

				for (unsigned int i = 0; i < kingyo_num; i++) {
					/* 金魚グループに関する初期化 */
					kingyo_group[i].setSpeed(1.0, 3.0); // 金魚のスピードを設定
					kingyo_group[i].animsp = 30; // アニメーションの設定
					kingyo_group[i].spawn_position(dice(mt) % 980 + 100, dice(mt) % 400 + 100);//範囲内に収まるように補正
					if (i % 2 == 0) kingyo_group[i].SetMovement(MOV_OPTION::CIRCLE, 300, 1.0);
					if (i % 3 == 0)kingyo_group[i].SetMovement(MOV_OPTION::WAVE, 100, 100);
				}
				for (unsigned int i = 0; i < telescope_num; i++) {
					/* 出目金グループに関する初期化 */
					telescope_group[i].setSpeed(1.0, 3.0); // 出目金のスピードを設定
					telescope_group[i].animsp = 30; // アニメーションの設定
					if (i % 2 == 0) telescope_group[i].SetMovement(MOV_OPTION::CIRCLE, 300, 1.0);
					if (i % 3 == 0) telescope_group[i].SetMovement(MOV_OPTION::WAVE, 100, 100);
				}
				poi_num_remaining = 5;


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
			for (unsigned int i = 0; i < kingyo_anim.size(); ++i)
			{
				if (kingyo_anim_timer[i]() == 0)
				{
					kingyo_anim_timer.erase(kingyo_anim_timer.begin() + i);
					kingyo_anim.destroy(i);
					i--;
				}
			}
			kingyo_anim.draw();

			/* 次状態の管理 */
			if (timer60sec() == 0 || poi_num_remaining == 0)
			{
				windowFlag = 2; // 60秒たったら終了しスコア表示へ
				total_score = cought_kingyo * kingyo_score + cought_telescope * telescope_score;
				kingyo_ranking.insert(username, total_score);
			}
			else DrawFormatStringToHandle(520, 60, GetColor(120, 120, 120), count_Font, "のこり%d秒", timer60sec() / 60); // 残り時間表示
			if (key_getter.GetKeyDown(KEY_INPUT_Z)) {
				/* zキーが押された */
				Timer tmptimer(60);
				poi_destroy = true;//ポイが破れるかどうかのフラグ
				for (int i = 0; i < (int)kingyo_group.size(); i++) {
					if (kingyo_group[i].isCought(poi)) {
						kingyoFake = kingyo_group[i];//座標保存
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
						kingyoFake.animsp = 5;//アニメーションスピードを速くする
						kingyoFake.x -= static_cast<int>(kingyoFake.xlength * 0.1);//画像拡大による位置調整
						kingyoFake.y -= static_cast<int>(kingyoFake.ylength * 0.1);
						kingyoFake.setScale(1.2);//金魚を大きくする
						kingyo_anim.addcpy(kingyoFake);//金魚のアニメーションを描画するオブジェクトグループに追加
						kingyo_anim_timer.push_back(tmptimer);//1秒タイマーをセット
					}
				}
				for (int i = 0; i < (int)telescope_group.size(); i++) {
					if (telescope_group[i].isCought(poi)) {
						kingyoFake = telescope_group[i];
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
						PlaySoundMem(effect, DX_PLAYTYPE_BACK);
						poi_destroy = false;
						//kingyo_groupのときと同様の処理
						kingyoFake.animsp = 5;
						kingyoFake.x -= static_cast<int>(kingyoFake.xlength * 0.1);
						kingyoFake.y -= static_cast<int>(kingyoFake.ylength * 0.1);
						kingyoFake.setScale(1.2);
						kingyo_anim.addcpy(kingyoFake);
						kingyo_anim_timer.push_back(tmptimer);
					}
				}
				if (poi_destroy)
				{
					poi_num_remaining--;//ポイが破れる
				}
			}
			poi.point_change(key_getter);
			kingyo_group.Next(); // オブジェクトの見た目の遷移
			telescope_group.Next();
			timer60sec.update(); // タイマー更新
			for (unsigned int i = 0; i < kingyo_anim_timer.size(); ++i)kingyo_anim_timer[i].update();
		}
		else if (windowFlag == 2) {
			/* スコア表示 */
			/* 画面の描画 */
			SetMainWindowText("スコア表示");	//windowテキスト
			DrawGraph(0, 0, back_img, TRUE);
			DrawGraph(0, 0, back_black, TRUE);
			DrawBoxAA(320, 180, 320 * 3, 180 * 3, GetColor(218, 165, 32), true);
			button_back.draw();
			DrawFormatStringToHandle(520, 60, GetColor(120, 120, 120), font, "%d匹捕まえたよ", cought_kingyo + cought_telescope);
			DrawFormatStringToHandle(520, 200, GetColor(120, 120, 120), font, (username + "のスコア").c_str());
			DrawFormatStringToHandle(520, 250, GetColor(120, 120, 120), font, "金魚 × %d匹", cought_kingyo);
			DrawFormatStringToHandle(520, 300, GetColor(120, 120, 120), font, "出目金 × %d匹", cought_telescope);
			DrawFormatStringToHandle(520, 350, GetColor(120, 120, 120), font, "スコア： %d", total_score);
			DrawFormatStringToHandle(590, 425, GetColor(120, 120, 120), font, "戻る");
			
			/* 次状態の管理 */
			if (button_back.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				PlaySoundMem(effect, DX_PLAYTYPE_BACK);
				windowFlag = 10; // タイトル画面へ
			}
			if (timer80sec() == 0) { // スコア表示時間を過ぎたら
				PlaySoundMem(effect, DX_PLAYTYPE_BACK); // 効果音
				windowFlag = 10; // タイトル画面へ
			}
			button_back.next(px, py);
			timer80sec.update(); // タイマーの更新
		}
		else if (windowFlag == 3) {
			/* ユーザー名入力処理 */
			DrawGraph(0, 0, title_img, TRUE);
			DrawGraph(0, 0, back_black, TRUE);
			input_username.draw();
			if (input_username.entered())
			{
				windowFlag = 1;
				input_username.text(username);
			}
		}
		else if (windowFlag == 10) { // 金魚すくいへ
			DeleteFontToHandle(count_Font);
			DeleteGraph(back_img);
			DeleteGraph(title_img);
			DeleteGraph(back_black);
			deleteImageHandle(button_handle);
			deleteImageHandle(button_back_handle);
			deleteImageHandle(kingyo_handle);
			deleteImageHandle(telescope_handle);
			deleteImageHandle(poi_handle);
			return 0;
		}
		else if (windowFlag == 11) { // 射的ゲームへ
			DeleteFontToHandle(count_Font);
			DeleteGraph(back_img);
			DeleteGraph(title_img);
			DeleteGraph(back_black);
			deleteImageHandle(button_handle);
			deleteImageHandle(button_back_handle);
			deleteImageHandle(kingyo_handle);
			deleteImageHandle(telescope_handle);
			deleteImageHandle(poi_handle);
			DeleteFontToHandle(count_Font);
			return 1;
		}
		else {
			return 0;
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
	return 0;
}