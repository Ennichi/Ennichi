#include "main.h"
#include "stdafx.h"

void titlemain() {
	SetMainWindowText("�����Q�[��(�^�C�g��)");
	AddFontResourceEx("./asset/font/fonts/otf/Mplus1-Regular.otf", FR_PRIVATE, NULL);	//�t�H���g��ǂݍ���
	int FontHandle = CreateFontToHandle("Mplus1-Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	int PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	int bgm = LoadSoundMem("./asset/bgm/maou_minzoku9.ogg");	//����������09
	int effect = LoadSoundMem("./asset/effect/system49.ogg");	//�V�X�e����
	DrawStringToHandle(250, 240 - 32, "Z�L�[�ŋ�����V�ׂ܂�\nX�L�[�Ŏ˓I���ł��܂�", GetColor(0,0,0),FontHandle);
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	IPDATA Ip;
	Ip.d1 = 192; Ip.d2 = 168; Ip.d3 = 0; Ip.d4 = 16;
	networking send;
	while (1) {
		if (ProcessMessage() == -1) {
			break;	//�G���[
		}
		PADInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if ((PADInput & PAD_INPUT_1) != 0) {
			ClearDrawScreen();
			PlaySoundMem(effect, DX_PLAYTYPE_BACK);
			kingyomain(FontHandle,bgm,effect); 
		}
		else if ((PADInput & PAD_INPUT_RIGHT) != 0) { // ���E�L�[������
			ClearDrawScreen();
			printfDx("���M��");
			send.sender(Ip, 10301031);
		}
		else if ((PADInput & PAD_INPUT_LEFT) != 0) { // ��󍶃L�[������
			ClearDrawScreen();
			printfDx("��M��");
			send.receiver();
		}
	}
}