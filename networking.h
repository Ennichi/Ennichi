#pragma once
#include "stdafx.h"

//ネットワーククラス
class networking {
public:
	IPDATA Ip;
	// ネットワークハンドル
	int NetUDPHandle; 
	void sender(IPDATA Ip,int send) {
		printfDx("Now sending...");
		// ＵＤＰ通信用のソケットハンドルを作成
		NetUDPHandle = MakeUDPSocket(-1);
			NetWorkSendUDP(NetUDPHandle, Ip, 9850, &send, 15);
			WaitKey();
		// キー入力待ち

		// ＵＤＰソケットハンドルの削除
		DeleteUDPSocket(NetUDPHandle);
	}
	void receiver() {
		int receive;
		// ＵＤＰ通信用のソケットハンドルを作成
		NetUDPHandle = MakeUDPSocket(9850);
		
		// 文字列の受信を待つ
		while (CheckNetWorkRecvUDP(NetUDPHandle) == FALSE)
		{
			// ウインドウズメッセージ処理
			if (ProcessMessage() < 0) break;
		}
			NetWorkRecvUDP(NetUDPHandle, NULL, NULL, &receive, 256, FALSE);
			printfDx("%dOK", receive);
			// キー入力待ち
			WaitKey();
		
		NetWorkRecvUDP(NetUDPHandle, NULL, NULL, &receive, 256, FALSE);
		printfDx("%dOK", receive);
		// キー入力待ち
		WaitKey();

		// ＵＤＰソケットハンドルの削除
		DeleteUDPSocket(NetUDPHandle);
	}
};