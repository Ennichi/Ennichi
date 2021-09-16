#include "main.h"
#include "stdafx.h"

void kingyomain() {
	int windowFlag = 0;  // ���݂̃E�B���h�E���Ǘ�����t���O
	int FramePerSecond = 60;//fps
	LONGLONG nowtime, prevtime;//���ݎ���

	std::vector<const char*> path = { "./image/goldfish_open.png", "./image/goldfish_close.png" };
	std::vector<const char*> buttonpath = { "./image/button2.png", "./image/button1.png" };
	std::vector<const char*> poi_path = { "./image/poi.png" };

	std::random_device seed;//����������
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<int> handle{};
	for (auto image_path : path)
	{
		handle.push_back(LoadGraph(image_path));
		if (*(handle.end() - 1) == -1)
		{
			MessageBox(NULL, "�摜�ǂݍ��ݎ��G���[", "Error Info", MB_ICONHAND);
			exit(1);
		}
	}

	std::vector<int> button_handle{};
	for (auto image_path : buttonpath)
	{
		button_handle.push_back(LoadGraph(image_path));
		if (*(button_handle.end() - 1) == -1)
		{
			MessageBox(NULL, "�摜�ǂݍ��ݎ��G���[", "Error Info", MB_ICONHAND);
			exit(1);
		}
	}

	std::vector<int> poi_handle{};
	for (auto image_path : poi_path)
	{
		poi_handle.push_back(LoadGraph(image_path));
		if (*(poi_handle.end() - 1) == -1)
		{
			MessageBox(NULL, "�摜�ǂݍ��ݎ��G���[", "Error Info", MB_ICONHAND);
			exit(1);
		}
	}

	int px, py;
	int click_event, button_type, cx, cy, log_type;

	Button button1(250, 240, false, button_handle);
	Button button2(250, 240 + 200, false, button_handle);

	Goldfish *fish1=new Goldfish(0, 0, DX_PI, true, handle); //����
	Goldfish fish2(100, 0, DX_PI * 3.0 / 4.0, true, handle);
	Poi first(500, 500, true, poi_handle);//�|�C
	Goldfish fish3 = *fish1;

	fish1->setDifficulty(10);
	fish1->animsp = 30;
	KeyInput input(KEY_INPUT_Z);

	fish1->setSpeed(0.5, 1.0);//�X�s�[�h�ݒ�

	int font = CreateFontToHandle("���C���I", 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE);


	prevtime = GetNowHiPerformanceCount();
	int clock = GetNowCount();	//���ݎ����̎擾
	int score=0;	//�Q�[���̃X�R�A
	/* �Q�[�����[�v */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // �\����ʂ𗠂�
		ClearDrawScreen();  // ��ʑS�̂��N���A


		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//�G���[���N�����烋�[�v���ʂ���

		if (windowFlag == 0) {  // ���j���[�E�B���h�E
			SetMainWindowText("����������(�^�C�g��)");	//window�e�L�X�g

			DrawStringToHandle(350, 120, "�����Q�[��", GetColor(120,120,120), font);
			button1.draw();	//�Q�[���X�^�[�g
			button2.draw();	//�`���[�g���A��
			button1.next(px, py);
			button2.next(px, py);
			if (button1.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//�{�^���p
			}
			else if (button2.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 2;	//�{�^���p
			}
		}
		else if (windowFlag == 1) { // �Q�[�����̃E�B���h�E
			SetMainWindowText("����������(�Q�[����)");	//window�e�L�X�g
			input();

			if (fish1 != NULL)
			{
				fish1->Next();
				fish1->draw();
			}
			fish3.Next();
			fish3.draw();
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
			if ((GetNowCount() - clock) >= 60000) {
				SetMainWindowText("�X�R�A�\����");	//window�e�L�X�g
				windowFlag = 0;
				DrawFormatString(500, 200, GetColor(120, 120, 120), "�X�R�A��%d�ł�\n�Q�[���𑱂���ɂ̓L�[�������Ă�������...", score, font);
			}
			DrawFormatString(350, 120, GetColor(120, 120, 120) ,"�Q�[���I���܂�%d", (GetNowCount() - clock), font);
		}
		else if (windowFlag == 2) {	//�`���[�g���A��
			SetMainWindowText("����������(�`���[�g���A���̓{�^���ŃL�����Z���ł��܂�)");	//window�e�L�X�g
			PlayMovie("./image/tutorial.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);	//�`���[�g���A�����Đ�����(�{�^���L�����Z������)
			windowFlag = 0;
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