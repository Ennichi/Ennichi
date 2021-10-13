#include "main.h"
#include "stdafx.h"

void kingyomain(int font,int bgm,int effect, int calling_check) {
	int windowFlag = 0;  // ���݂̃E�B���h�E���Ǘ�����t���O
	int FramePerSecond = 60;//fps
	int score = 0;	//�Q�[���̃X�R�A
	LONGLONG nowtime, prevtime;//���ݎ���

	std::random_device seed;//����������
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<int> handle{};
	makeImageHandle(handle, "./asset/image/goldfish_open.png", "./asset/image/goldfish_close.png");

	std::vector<int> button_handle{};
	makeImageHandle(button_handle, "./asset/image/uncheck.png", "./asset/image/checked.png");

	std::vector<int> poi_handle{};
	makeImageHandle(poi_handle, "./asset/image/poi.png");

	int px, py;
	int click_event, button_type, cx, cy, log_type;	
	Button button_start(100, 200, false, button_handle);	//START�{�^��
	StringObj start_obj(150, 250, "START", GetColor(120, 120, 120), font);
	Button button_result(200, 350, false, button_handle);	//�ݒ�{�^��
	StringObj result_obj(250, 400, "result", GetColor(120, 120, 120), font);
	Button button_gotosyateki(1000, 600, false, button_handle);	//�˓I�Q�[���֍s���{�^��
	StringObj gotosyateki_obj(1025, 650, "SYATEKI", GetColor(120, 120, 120), font);

	Goldfish *fish1=new Goldfish(300, 300, 0, true, handle); //����
	Goldfish fish2(100, 0, DX_PI * 3.0 / 4.0, true, handle);
	Poi first(500, 500, true, poi_handle);//�|�C
	Goldfish fish3 = *fish1;

	ObjGroup<Goldfish> fish4;

	fish1->setDifficulty(10);
	fish1->animsp = 30;
	KeyInput input(KEY_INPUT_Z);

	fish1->setSpeed(0.5, 1.0);//�X�s�[�h�ݒ�

	fish4.addcpy(*fish1, 10);

	for (int i = 0; i < 10; ++i)
	{
		fish4[i].angle -= (double)i * 0.2;
	}
	prevtime = GetNowHiPerformanceCount();
	int clock = GetNowCount();	//���ݎ����̎擾
	Timer timer(3600);
	Timer timer2(2400);
	//bgm��ǂݍ���
	if (calling_check == 0) {
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	}
	int back_img = LoadGraph("./asset/image/background.png");
	/* �Q�[�����[�v */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // �\����ʂ𗠂�
		ClearDrawScreen();  // ��ʑS�̂��N���A

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//�G���[���N�����烋�[�v���ʂ���

		if (windowFlag == 0) {  // ���j���[�E�B���h�E
			SetMainWindowText("����������(�^�C�g��)");	//window�e�L�X�g
			
			DrawStringToHandle(500, 120, "�����Q�[��", GetColor(120,120,120), font);
			button_start.draw();	//�Q�[���X�^�[�g
			button_start.next(px, py);
			start_obj.draw();
			button_result.draw();	//���ʉ��
			button_result.next(px, py);
			result_obj.draw();
			button_gotosyateki.draw();		//�˓I�Q�[����
			button_gotosyateki.next(px, py);
			gotosyateki_obj.draw();
			if (button_start.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//�����������X�^�[�g
			}
			if (button_result.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 2;	//���ʕ\��
			}
			if (button_gotosyateki.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 10;	//�˓I�Q�[���֑J��
			}
		}
		else if (windowFlag == 1) { // �Q�[�����̃E�B���h�E
			SetMainWindowText("����������(�Q�[����)");	//window�e�L�X�g
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
				printfDx("�߂܂���");
				delete fish1;
				fish1 = NULL;
				score++;
			}

			//60�b��������I��
			if (timer() == 0) {
				SetMainWindowText("�X�R�A�\��");	//window�e�L�X�g
				DrawFormatString(500, 200, GetColor(120, 120, 120), "�X�R�A��%d�ł�", score, font);
				if (timer2() == 0) {
					windowFlag = 0;
				}
				timer2.update();
			}
			else {
				DrawFormatString(1200, 0, GetColor(120, 120, 120), "�c��%d�b", timer() / 60, font);
			}
			timer.update();
		}
		else if (windowFlag == 2) {
			SetMainWindowText("����");	//window�e�L�X�g
		}
		else if(windowFlag==10) {  // �Q�[���̏I��
			syatekimain(font, bgm, effect,calling_check);
		}
		else {
			return;
		}
		ScreenFlip();

		/* �t���[�����[�g�̏��� */
		nowtime = GetNowHiPerformanceCount();
		while (nowtime - prevtime < 1000000 / FramePerSecond)
		{
			nowtime = GetNowHiPerformanceCount();
		}
		prevtime = nowtime;
	}
}