#include "main.h"
 void game_temp() {

	/* ������ */
	unsigned long long frames = 0;
	int windowFlag = 0;  // ���݂̃E�B���h�E���Ǘ�����t���O
	int px, py;
	int click_event, button_type, cx, cy, log_type;

	std::random_device seed;//����������
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> dice(1, 1000);

	std::vector<const char*> path = { "./image/kingyo.png", };
	std::vector<const char*> poi_path = { "./image/poi_kirinuki.png" };
	std::vector<const char*> button_path = { "./image/button1.png", "./image/button2.png" };

	Goldfish fish1(0, 0, 0.0, true, path);
	Goldfish fish2(100, 0, DX_PI *3.0/4.0, true, path);
	Poi first(500, 500, true, poi_path);
	Button button(300, 300, false, button_path);

	fish1.setSpeed(1.0, 2.0);
	fish2.setSpeed(1.0, 10.0);
	
	Timer timer;
	KeyInput input = { KEY_INPUT_0, KEY_INPUT_1,KEY_INPUT_Z };

	/* �Q�[�����[�v */
	while (1) {
		input();

	 	SetDrawScreen(DX_SCREEN_BACK);  // �\����ʂ𗠂�
		ClearDrawScreen();  // ��ʑS�̂��N���A
		GetMousePoint(&px, &py);
		click_event = GetMouseInputLog2(&button_type, &cx, &cy, &log_type);
		if (ProcessMessage() == -1) break;	//�G���[���N�����烋�[�v���ʂ���
		if (windowFlag == 0) {  // ���j���[�E�B���h�E

			timer.sets(50);
			if (timer() == 0)
			{
				windowFlag = 1;
			}
			fish1.Next();
			fish2.SetMovement(MOV_OPTION::WAVE, 20.0, 300.0);
			fish2.Next();
			first.point_change();

			if (input.GetKeyDown(KEY_INPUT_Z) == 1 && fish2.isCought(first, mt, dice))printfDx("�߂܂���");
			if (button.isPushedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 1;
			}
			button.next(px,py);

			if (input.GetKeyDown(KEY_INPUT_1))
			{
				printfDx("1");
			}
			if (input.GetKeyDown(KEY_INPUT_0))
			{
				printfDx("0");
			}
			fish1.draw();
			fish2.draw();
			first.draw();
			button.draw();
		}
		else if (windowFlag == 1) { // �Q�[�����̃E�B���h�E
			button.draw();
			button.next(px, py);
			if (button.isReleasedLeft(click_event, button_type, cx, cy, log_type)) {
				windowFlag = 0;
			}
		}
		else {  // �Q�[���̏I��
			return;
		}
		frames++;
		ScreenFlip();
	}
}