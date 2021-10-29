#include "../../main.h"
#include "../../stdafx.h"

//https://docs.microsoft.com/ja-jp/cpp/code-quality/annotating-function-parameters-and-return-values?view=msvc-160&viewFallbackFrom=vs-2019
int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd)
{
    //デバッグ時はこれをTRUEにすること
    SetOutApplicationLogValidFlag(FALSE); //ログファイルの出力をやめる
    SetWindowUserCloseEnableFlag(FALSE);  //バツボタンを無効にする
    SetDoubleStartValidFlag(TRUE);        //DXライブラリの多重起動を許可する
    SetMultiThreadFlag(TRUE);
    ChangeWindowMode(TRUE);     //ウィンドウモードにする
    SetGraphMode(500, 500, 32); //画面サイズを500x500,32bitカラーにする
    SetWindowStyleMode(7);
    SetWindowSizeChangeEnableFlag(TRUE); //ウィンドウのサイズを変更できるようにする
    char StrBuf[256];           // データバッファ
    int NetUDPHandle;           // ネットワークハンドル
    const std::string show_point = "ポイントをゲットしました";
    SetBackgroundColor(129, 161, 193); //背景を水色にする
    if (DxLib_Init() == -1)            // ＤＸライブラリ初期化処理
    {
        return -1; // エラーが起きたら直ちに終了
    }
    SetAlwaysRunFlag(TRUE); //フォーカスが外れた時も動作を続ける

    int FontHandle = CreateFontToHandle("メイリオ", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
    // ＵＤＰ通信用のソケットハンドルを作成
    NetUDPHandle = MakeUDPSocket(9850);
    // 受信待ちだよと表示
    DrawFormatStringToHandle(0, 0, GetColor(163, 190, 140), FontHandle, "縁日ゲーム");

    while (1)
    {
        if (ProcessMessage() == -1)
        {
            break; // エラーが起きたらループを抜ける
        }
        SetDrawScreen(DX_SCREEN_BACK); // 表示画面を裏に
        ClearDrawScreen();             // 画面全体をクリア

        // 文字列の受信を待つ
        while (CheckNetWorkRecvUDP(NetUDPHandle) == FALSE)
        {
            //バツボタンが押されたら終了する
            if (GetWindowUserCloseFlag(TRUE))
            {
                // ＵＤＰソケットハンドルの削除
                DeleteUDPSocket(NetUDPHandle);
                DeleteFontToHandle(FontHandle);

                DxLib_End(); // ＤＸライブラリ使用の終了処理
                return 0;
            }
            if (ProcessMessage() == -1)
            {
                break; // エラーが起きたらループを抜ける
            }
        }
        // 受信
        NetWorkRecvUDP(NetUDPHandle, NULL, NULL, StrBuf, 256, FALSE);
        std::string point = StrBuf;
        std::string show_point_up = point + show_point;
        // 受信した文字列を画面に描画
        DrawString(100, 100, show_point_up.c_str(), GetColor(163, 190, 140));
        //バツボタンが押されたら終了する
        if (GetWindowUserCloseFlag(TRUE))
        {
            // ＵＤＰソケットハンドルの削除
            DeleteUDPSocket(NetUDPHandle);
            DeleteFontToHandle(FontHandle);

            DxLib_End(); // ＤＸライブラリ使用の終了処理
            return 0;
        }
        ScreenFlip();
    }
    return 0;
}
