#include "../../main.h"    
#include "../../stdafx.h"

//https://docs.microsoft.com/ja-jp/cpp/code-quality/annotating-function-parameters-and-return-values?view=msvc-160&viewFallbackFrom=vs-2019
int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_  HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd){
    //デバッグ時はこれをTRUEにすること
    SetOutApplicationLogValidFlag(FALSE);   //ログファイルの出力をやめる
    SetAlwaysRunFlag(TRUE); //フォーカスが外れた時も動作を続ける
    SetDoubleStartValidFlag(TRUE);  //DXライブラリの多重起動を許可する
    ChangeWindowMode(TRUE); //ウィンドウモードにする
    SetGraphMode(500, 500, 32);    //画面サイズを1280x720,32bitカラーにする
    char StrBuf[256] ;    // データバッファ
    int NetUDPHandle ;        // ネットワークハンドル

    if( DxLib_Init() == -1 )    // ＤＸライブラリ初期化処理
    {
         return -1;    // エラーが起きたら直ちに終了
    }

    // ＵＤＰ通信用のソケットハンドルを作成
    NetUDPHandle = MakeUDPSocket( 9850);
    // 受信待ちだよと表示
    DrawString( 0, 0, "縁日ゲーム", GetColor( 255,255,255 ) );
	while(1){
    	// 文字列の受信を待つ
    	while( CheckNetWorkRecvUDP( NetUDPHandle ) == FALSE )
   	 	{
        	// ウインドウズメッセージ処理
        	if( ProcessMessage() < 0 ) break;
    	}
    	   // 受信
    	NetWorkRecvUDP( NetUDPHandle, NULL, NULL, StrBuf, 256, FALSE );

    	// 受信した文字列を画面に描画
   		ClearDrawScreen();
    DrawString( 0, 0, StrBuf, GetColor( 255,255,255 ) );
	}
    // ＵＤＰソケットハンドルの削除
    DeleteUDPSocket(NetUDPHandle); 

    DxLib_End();    // ＤＸライブラリ使用の終了処理
    return 0;
}
