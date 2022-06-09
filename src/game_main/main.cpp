#pragma once
#include "../header/stdafx.h"
#include "../header/main.hpp"

//https://docs.microsoft.com/ja-jp/cpp/code-quality/annotating-function-parameters-and-return-values?view=msvc-160&viewFallbackFrom=vs-2019
int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_  HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd){
    //デバッグ時はこれをTRUEにすること
    SetOutApplicationLogValidFlag(FALSE);   //ログファイルの出力をやめる
    SetAlwaysRunFlag(TRUE); //フォーカスが外れた時も動作を続ける
    ChangeWindowMode(TRUE); //ウィンドウモードにする
    SetWindowIconID(333);	//アイコン表示
    SetGraphMode(1280, 720, 32);    //画面サイズを1280x720,32bitカラーにする
    //エラーが出たら終了
    if (DxLib_Init() == -1) {
        return -1;
    }
    LoadGraphScreen(0, 0, "./asset/image/syateki_title.jpg", FALSE);
    int bgm = -1;
    int effect = LoadSoundMem("./asset/effect/system49.ogg");	//システム音
    AddFontResourceEx("./asset/fonts/PixelMplus10-Regular.ttf", FR_PRIVATE, NULL);	//フォントを読み込む
    int FontHandle = CreateFontToHandle("PixelMplus10 Regular", 30, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);    //フォントハンドルを作成する
    int calling_check = 0;
    int flag = 1; // どちらのゲームを呼び出すか管理するフラグ(0: 金魚, 1: 射的)
    while (ProcessMessage() == 0) {
        if (flag == 0) flag = kingyomain(FontHandle, bgm, effect, calling_check);
        else flag = syatekimain(FontHandle, bgm, effect,calling_check);
    }
    //終わり
    DxLib_End();
    return 0;
}
