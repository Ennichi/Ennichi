#include "main.h"
#include "stdafx.h"

void syatekimain(int font, int bgm, int effect, int calling_check) {
	int windowFlag = 0;  // ���݂̃E�B���h�E���Ǘ�����t���O
	int FramePerSecond = 60;//fps
	int score = 0;	//�Q�[���̃X�R�A
	LONGLONG nowtime, prevtime;//���ݎ���

	std::random_device seed;//����������
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<int> button_handle{};
	makeImageHandle(button_handle, "./asset/image/button2.png", "./asset/image/button1.png");	//TODO:�{�^�����u���ցv�ɕύX����

	std::vector<int> gun_handle{};
	makeImageHandle(gun_handle, "./asset/image/syateki.png");	//TODO:�摜��u������

	int px, py;
	int click_event, button_type, cx, cy, log_type;
	Button button1(400, 240, false, button_handle);	
	Button next_panel(800, 600, false, button_handle);
	Gun gun_syateki(100, 600, false, gun_handle);
	KeyInput input(KEY_INPUT_Z);

	prevtime = GetNowHiPerformanceCount();
	int clock = GetNowCount();	//���ݎ����̎擾
	Timer timer(3600);
	Timer timer2(2400);
	int back_img = LoadGraph("./asset/image/background.png");	//TODO:�w�i�摜���˓I�̉���ɂ���
	/* �Q�[�����[�v */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // �\����ʂ𗠂�
		ClearDrawScreen();  // ��ʑS�̂��N���A

		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);

		if (ProcessMessage() == -1) break;	//�G���[���N�����烋�[�v���ʂ���

		if (windowFlag == 0) {  // ���j���[�E�B���h�E
			SetMainWindowText("�˓I�Q�[��(�^�C�g��)");	//window�e�L�X�g

			DrawStringToHandle(500, 120, "�˓I�Q�[��[WIP]", GetColor(120, 120, 120), font);
			button1.draw();	//�Q�[���X�^�[�g
			button1.next(px, py);
			next_panel.draw();	//�����Q�[����
			next_panel.next(px, py);
			if (button1.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;	//�{�^���p
			}
			if (next_panel.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 10;	//�����Q�[���֑J��
			}
		}
		else if (windowFlag == 1) { // �Q�[�����̃E�B���h�E
			SetMainWindowText("�˓I�Q�[��(�Q�[����)");	//window�e�L�X�g
			DrawGraph(0, 0, back_img, TRUE);

			input();
			gun_syateki.gunnt_change();
			gun_syateki.draw();
			//60�b��������I��
			if (timer() == 0) {
				SetMainWindowText("�X�R�A�\��");	//window�e�L�X�g
				if (score > 0) {
					DrawFormatString(500, 200, GetColor(120, 120, 120), "%d�ł������܂���!", score, font);
				}
				else {
					DrawFormatString(500, 200, GetColor(120, 120, 120), "�c�O!",font);
				}
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
		else if (windowFlag == 10) {  // �Q�[���̏I��
			calling_check=1;
			kingyomain(font, bgm, effect,calling_check);
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