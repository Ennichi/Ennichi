#include "main.h"
#include "stdafx.h"

void shootingmain() {
	/* ������ */
	int windowFlag = 0;  // ���݂̃E�B���h�E���Ǘ�����t���O
	std::vector<const char*> file_path = { "./score_shoooting.csv" };
	std::vector<const char*> gun_path = { "./image/gun.png" };
	std::vector<int> gun_handle{LoadGraph(*(gun_path.begin()))};
	if (*(gun_handle.begin()) == -1)
	{
		MessageBox(NULL, "�摜�ǂݍ��ݎ��G���[", "Error Info", MB_ICONHAND);
		exit(1);
	}
	Gun first(0, 250, true, gun_handle);
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