#include "main.h"    
#include "stdafx.h"

//https://docs.microsoft.com/ja-jp/cpp/code-quality/annotating-function-parameters-and-return-values?view=msvc-160&viewFallbackFrom=vs-2019
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd){
    int dispx = GetSystemMetrics(SM_CXSCREEN);  //�E�B���h�E��X���W�̎擾
    int dispy = GetSystemMetrics(SM_CYSCREEN);  //�E�B���h�E��Y���W�̎擾

    //�f�o�b�O���͂����TRUE�ɂ��邱��
    SetOutApplicationLogValidFlag(FALSE);   //���O�t�@�C���̏o�͂���߂�
    SetAlwaysRunFlag(TRUE); //�t�H�[�J�X���O�ꂽ��������𑱂���
    SetDoubleStartValidFlag(TRUE);  //DX���C�u�����̑��d�N����������
    ChangeWindowMode(TRUE); //�E�B���h�E���[�h�ɂ���
    SetWindowIconID(333);	//�A�C�R���\��
    SetGraphMode(1280, 720, 32);    //��ʃT�C�Y��1280x720,32bit�J���[�ɂ���
    SetBackgroundColor(255, 255, 255);  //�w�i�𔒐F�ɂ���
    SetWindowStyleMode(6);
    SetWindowSizeChangeEnableFlag(TRUE);    //�E�B���h�E�̃T�C�Y��ύX�ł���悤�ɂ���
    //�G���[���o����I��
    if (DxLib_Init() == -1) {
        return -1;
    }
    int bgm = LoadSoundMem("./asset/bgm/maou_minzoku9.ogg");	//����������09
    int effect = LoadSoundMem("./asset/effect/system49.ogg");	//�V�X�e����
    int FontHandle = CreateFontToHandle("Mplus1-Regular", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);    //�t�H���g�n���h�����쐬����

    AddFontResourceEx("./asset/font/fonts/otf/Mplus1-Regular.otf", FR_PRIVATE, NULL);	//�t�H���g��ǂݍ���
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
    int calling_check = 0;
    //bgm��ǂݍ���
    kingyomain(FontHandle, bgm, effect,calling_check);
    //�I���
    DxLib_End();
    return 0;
}
