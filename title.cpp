#include "main.h"
#include "stdafx.h"

void titlemain() {
	SetMainWindowText("�����Q�[��(�^�C�g��)");
	int FontHandle;
	FontHandle = CreateFontToHandle(
		NULL,
		40,
		3,
		DX_FONTTYPE_ANTIALIASING_EDGE_4X4
	);
	int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	DrawStringToHandle(250, 240 - 32, "�^�C�g���ł�\nZ�L�[�ŋ�����I���ł�", GetColor(0,120,120),FontHandle);
	while (1) {
		if (ProcessMessage() == -1) {
			break;	//�G���[
		}
		PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if ((PADInput & PAD_INPUT_1) != 0) {
			ClearDrawScreen();
			kingyomain(); 
		}
		else if ((PADInput & PAD_INPUT_RIGHT) != 0) { // ���E�L�[������
			ClearDrawScreen();
			game_temp();
		}
		else if ((PADInput & PAD_INPUT_LEFT) != 0) { // ��󍶃L�[������
			ClearDrawScreen();
			shootingmain();
		}
	}
}