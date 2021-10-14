#include "main.h"
#include "stdafx.h"

void kingyomain(int font,int bgm,int effect) {
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
	makeImageHandle(button_handle, "./asset/image/button2.png", "./asset/image/button1.png");

	std::vector<int> poi_handle{};
	makeImageHandle(poi_handle, "./asset/image/poi.png");

	int px, py;
	int click_event, button_type, cx, cy, log_type;	
	Button button1(400, 240, false, button_handle);
	Goldfish *fish1=new Goldfish(300, 300, 0, true, handle); //����
	Goldfish fish2(100, 0, DX_PI * 3.0 / 4.0, true, handle);
	Poi first(500, 500, true, poi_handle);//�|�C
	Goldfish fish3 = *fish1;

	ObjGroup<Goldfish> fish4;

	fish1->setDifficulty(10);
	fish1->animsp = 30;
	KeyInput input(KEY_INPUT_Z);

	fish1->setSpeed(0.5, 1.0);//�X�s�[�h�ݒ�

	//fish4.addcpy(*fish1, 10);
	fish4.addcpy(*fish1, *fish1, *fish1);
	for (int i = 0; i < 3; ++i)
	{
		fish4[i].angle -= (double)i * 0.2;
	}
	fish4.destroy(1);
	prevtime = GetNowHiPerformanceCount();
	int clock = GetNowCount();	//���ݎ����̎擾
	Timer timer(3600);
	Timer timer2(2400);
	//bgm��ǂݍ���
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
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
			button1.draw();	//�Q�[���X�^�[�g
			button1.next(px, py);

			if (button1.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//�{�^���p
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
		else {  // �Q�[���̏I��
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