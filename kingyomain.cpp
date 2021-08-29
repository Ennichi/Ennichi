#include "main.h"
#include "stdafx.h"

void kingyomain() {
	int windowFlag = 0;  // ���݂̃E�B���h�E���Ǘ�����t���O
	int FramePerSecond = 60;//fps
	LONGLONG nowtime, pretime;//���ݎ���

	std::vector<const char*> path = { "./image/goldfish_open.png", "./image/goldfish_close.png" };
	std::vector<const char*> buttonpath = { "./image/button2.png", "./image/button1.png" };
	std::vector<const char*> poi_path = { "./image/poi.png" };

	std::random_device seed;//����������
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	int px, py;
	int click_event, button_type, cx, cy, log_type;

	Button button1(250, 240, false, buttonpath);
	Button button2(250, 240 + 115 + 30, false, buttonpath);
	Button button3(250, 240 + 115 * 2 + 30 * 2, false, buttonpath);

	Goldfish *fish1=new Goldfish(0, 0, DX_PI, true, path); //����
	Goldfish fish2(100, 0, DX_PI * 3.0 / 4.0, true, path);
	Poi first(500, 500, true, poi_path);//�|�C
	Goldfish fish3 = *fish1;

	fish1->setDifficulty(50);
	KeyInput input(KEY_INPUT_Z);

	fish1->setSpeed(0.5, 1.0);//�X�s�[�h�ݒ�

	int font = CreateFontToHandle("���C���I", 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

	pretime = GetNowHiPerformanceCount();
	int wantsend = 0;

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
			button3.draw();	//�ߋ��̐���
			button1.next(px, py);
			button2.next(px, py);
			button3.next(px, py);
			if (button1.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//�{�^���p
			}
			else if (button2.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 2;	//�{�^���p
			}
			else if (button3.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 3;	//�{�^���p
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
			fish3.draw();;
			first.point_change();
			first.draw();
			if (fish1 != NULL && input.GetKeyDown(KEY_INPUT_Z) == 1 && fish1->isCought(first, mt, dice))
			{
				printfDx("�߂܂���");
				delete fish1;
				fish1 = NULL;
			}
		}
		else if (windowFlag == 2) {	//�`���[�g���A��
			SetMainWindowText("����������(�`���[�g���A���̓{�^���ŃL�����Z���ł��܂�)");	//window�e�L�X�g
			PlayMovie("./image/tutorial.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);	//�`���[�g���A�����Đ�����(�{�^���L�����Z������)
			windowFlag = 0;
		}
		else if (windowFlag == 3) {

		}
		else {  // �Q�[���̏I��
			return;
		}
		ScreenFlip();

		/* �t���[�����[�g�̏��� */
		nowtime = GetNowHiPerformanceCount();
		while (nowtime - pretime < 1000000 / FramePerSecond)
		{
			nowtime = GetNowHiPerformanceCount();
		}
		pretime = nowtime;
	}
}