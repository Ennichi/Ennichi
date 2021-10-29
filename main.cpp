#include "main.h"    
#include "stdafx.h"

//https://docs.microsoft.com/ja-jp/cpp/code-quality/annotating-function-parameters-and-return-values?view=msvc-160&viewFallbackFrom=vs-2019
int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_  HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd){
    int dispx = GetSystemMetrics(SM_CXSCREEN);  //ウィンドウのX座標の取得
    int dispy = GetSystemMetrics(SM_CYSCREEN);  //ウィンドウのY座標の取得
    //デバッグ時はこれをTRUEにすること
    SetOutApplicationLogValidFlag(FALSE);   //ログファイルの出力をやめる
    SetAlwaysRunFlag(TRUE); //フォーカスが外れた時も動作を続ける
    SetDoubleStartValidFlag(TRUE);  //DXライブラリの多重起動を許可する
    ChangeWindowMode(TRUE); //ウィンドウモードにする
    SetWindowIconID(333);	//アイコン表示
    SetGraphMode(1280, 720, 32);    //画面サイズを1280x720,32bitカラーにする
    SetBackgroundColor(255, 255, 255);  //背景を白色にする
    SetWindowStyleMode(7);
    SetWindowSizeChangeEnableFlag(TRUE);    //ウィンドウのサイズを変更できるようにする
    //エラーが出たら終了
    if (DxLib_Init() == -1) {
        return -1;
    }
    int bgm = -1;
    int effect = LoadSoundMem("./asset/effect/system49.ogg");	//システム音


    AddFontResourceEx("./asset/fonts/PixelMplus10-Regular.ttf", FR_PRIVATE, NULL);	//フォントを読み込む
    int FontHandle = CreateFontToHandle("PixelMplus10 Regular", 30, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);    //フォントハンドルを作成する

    //ディスプレイの1280x720以下の時
    if (dispx <= 1280 || dispy <= 720) {
        MessageBox(NULL, "解像度が低いよ\n新しいモニタ買ってね", "Error Info", MB_ICONHAND);
        exit(1);
    }
    if (std::filesystem::exists("./asset") == 0) {
        MessageBox(NULL, "素材フォルダが見つからないよ\n素材をGithubのリリースからダウンロードしてね", "Error Info", MB_ICONHAND);
        ShellExecute(NULL, "open", "https://github.com/Ennichi/Ennichi", NULL, NULL, SW_SHOW);
        exit(1);
    }
    int calling_check = 0;
    //bgmを読み込む
    syatekimain(FontHandle, bgm, effect,calling_check);
    //終わり
    DxLib_End();
    return 0;
}
