#pragma once
#include "stdafx.h"

//�l�b�g���[�N�N���X
class networking {
public:
	IPDATA Ip;
	// �l�b�g���[�N�n���h��
	int NetUDPHandle; 
	void sender(IPDATA Ip,int send) {
		printfDx("Now sending...");
		// �t�c�o�ʐM�p�̃\�P�b�g�n���h�����쐬
		NetUDPHandle = MakeUDPSocket(-1);
			NetWorkSendUDP(NetUDPHandle, Ip, 9850, &send, 15);
			WaitKey();
		// �L�[���͑҂�

		// �t�c�o�\�P�b�g�n���h���̍폜
		DeleteUDPSocket(NetUDPHandle);
	}
	void receiver() {
		int receive;
		// �t�c�o�ʐM�p�̃\�P�b�g�n���h�����쐬
		NetUDPHandle = MakeUDPSocket(9850);
		
		// ������̎�M��҂�
		while (CheckNetWorkRecvUDP(NetUDPHandle) == FALSE)
		{
			// �E�C���h�E�Y���b�Z�[�W����
			if (ProcessMessage() < 0) break;
		}
			NetWorkRecvUDP(NetUDPHandle, NULL, NULL, &receive, 256, FALSE);
			printfDx("%dOK", receive);
			// �L�[���͑҂�
			WaitKey();
		
		NetWorkRecvUDP(NetUDPHandle, NULL, NULL, &receive, 256, FALSE);
		printfDx("%dOK", receive);
		// �L�[���͑҂�
		WaitKey();

		// �t�c�o�\�P�b�g�n���h���̍폜
		DeleteUDPSocket(NetUDPHandle);
	}
};