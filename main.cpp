#include "main.h"    

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ChangeWindowMode(TRUE); //ウィンドウモードにする
    SetWindowIconID(333);	//アイコン表示
    int dispx = GetSystemMetrics(SM_CXSCREEN);  //ウィンドウのX座標の取得
    int dispy = GetSystemMetrics(SM_CYSCREEN);  //ウィンドウのY座標の取得
    //ディスプレイの1280x720以下の時
    if (dispx <= 1280 || dispy <= 720) {
        MessageBox(NULL, "解像度が低いよ\n新しいモニタ買ってね", "Error Info", MB_ICONHAND);
        exit(1);
    }
    if (std::filesystem::exists("./image") == 0) {
        MessageBox(NULL, "画像フォルダが見つからないよ\n画像をGithubのリリースからダウンロードしてね", "Error Info", MB_ICONHAND);
        ShellExecute(NULL, "open", "https://github.com/Ennichi/Ennichi", NULL, NULL, SW_SHOW);
        exit(1);
    }
    SetGraphMode(1280, 720, 32);    //画面サイズを1280x720,32bitカラーにする
    SetBackgroundColor(255, 255, 255);
    //エラーが出たら終了
    if (DxLib_Init() == -1) {
        return -1;
    }
    titlemain();    //タイトル画面を呼び出す
    //終わり
    DxLib_End();

    return 0;
}
