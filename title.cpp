#include "main.h"
#include "stdafx.h"

void titlemain() {
	SetMainWindowText("�����Q�[��(�^�C�g��)");
	int FontHandle = CreateFontToHandle(NULL,40,3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	int bgm = LoadSoundMem("./asset/bgm/maou_minzoku9.mp3");	//����������09
	DrawStringToHandle(250, 240 - 32, "�^�C�g���ł�\nZ�L�[�ŋ�����I���ł�", GetColor(0,120,120),FontHandle);
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	while (1) {
		if (ProcessMessage() == -1) {
			break;	//�G���[
		}
		PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if ((PADInput & PAD_INPUT_1) != 0) {
			ClearDrawScreen();
			DeleteSoundMem(bgm);	//������Ȃ����ƃQ�[�����ɗ���邪�ADelete�͕K�v 
			kingyomain(); 
		}
		else if ((PADInput & PAD_INPUT_RIGHT) != 0) { // ���E�L�[������
			ClearDrawScreen();
			DeleteSoundMem(bgm);
			game_temp();
		}
		else if ((PADInput & PAD_INPUT_LEFT) != 0) { // ��󍶃L�[������
			ClearDrawScreen();
			DeleteSoundMem(bgm);
			shootingmain();
		}
	}
}