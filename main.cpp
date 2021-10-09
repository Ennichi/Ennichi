#include "main.h"    
#include "stdafx.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ChangeWindowMode(TRUE); //�E�B���h�E���[�h�ɂ���
    SetWindowIconID(333);	//�A�C�R���\��
    SetDoubleStartValidFlag(TRUE);
    int dispx = GetSystemMetrics(SM_CXSCREEN);  //�E�B���h�E��X���W�̎擾
    int dispy = GetSystemMetrics(SM_CYSCREEN);  //�E�B���h�E��Y���W�̎擾
    //�f�B�X�v���C��1280x720�ȉ��̎�
    if (dispx <= 1280 || dispy <= 720) {
        MessageBox(NULL, "�𑜓x���Ⴂ��\n�V�������j�^�����Ă�", "Error Info", MB_ICONHAND);
        exit(1);
    }
    if (std::filesystem::exists("./asset") == 0) {
        MessageBox(NULL, "�f�ރt�H���_��������Ȃ���\n�f�ނ�Github�̃����[�X����_�E�����[�h���Ă�", "Error Info", MB_ICONHAND);
        ShellExecute(NULL, "open", "https://github.com/Ennichi/Ennichi", NULL, NULL, SW_SHOW);
        exit(1);
    }
    SetGraphMode(1280, 720, 32);    //��ʃT�C�Y��1280x720,32bit�J���[�ɂ���
    SetBackgroundColor(255, 255, 255);
    //�G���[���o����I��
    if (DxLib_Init() == -1) {
        return -1;
    }
    titlemain();    //�^�C�g����ʂ��Ăяo��
    //�I���
    DxLib_End();

    return 0;
}
