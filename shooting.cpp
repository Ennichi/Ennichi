#include "stdafx.h"
#include "main.h"

void shootingmain() {
	/* ������ */
	int windowFlag = 0;  // ���݂̃E�B���h�E���Ǘ�����t���O
	std::vector<const char*> file_path = { "./score_shoooting.csv" };
	std::vector<const char*> gun_path = { "./image/gun.png" };
	Gun first(0, 250, true, gun_path);
	/* �Q�[�����[�v */
	while (1) {
		SetDrawScreen(DX_SCREEN_BACK);  // �\����ʂ𗠂�
		ClearDrawScreen();  // ��ʑS�̂��N���A
		if (ProcessMessage() == -1) break;	//�G���[���N�����烋�[�v���ʂ���

		if (windowFlag == 0) {  // ���j���[�E�B���h�E

		}
		else if (windowFlag == 1) { // �Q�[�����̃E�B���h�E
			while (ProcessMessage() == 0) {
				first.gunnt_change();
				first.draw();
				ScreenFlip();

			}
		}
		else if (windowFlag == 2) {//���ʕ\���̃E�B���h�E

		}
		else {  // �Q�[���̏I��
			return;
		}
		ScreenFlip();
	}
}