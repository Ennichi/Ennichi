#pragma once
#define max(a, b) (((a) > (b)) ? (a) : (b))
#include "stdafx.h"

//�񓯊����^�C�}�[
class nSyncTimer
{
	using LL = long long;

private:
	/* �O���Q�ƕs�� */
	unsigned short fps = 60;
	LL configured_time = 0;
	LL start_time = 0;
	bool isSet = false;

public:
	/* �O���Q�Ɖ� */

	//�f�t�H���g�R���X�g���N�^
	nSyncTimer(){}

	//fps�ݒ�R���X�g���N�^
	nSyncTimer(unsigned short FramePerSecond) : fps{FramePerSecond} {}

	//�P�ʃt���[���Ń^�C�}�[�Z�b�g
	void setf(unsigned int frames)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = (1000000ll * (LL)frames) / fps;
		isSet = true;
	}

	//�P�ʃ~���b�Ń^�C�}�[�Z�b�g
	void setms(unsigned int ms)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = 1000ll * (LL)ms;
		isSet = true;
	}

	//�P�ʕb�Ń^�C�}�[�Z�b�g
	void sets(double seconds)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = (LL)(1000000.0 * seconds);
		isSet = true;
	}

	//�P�ʃ}�C�N���b�Ń^�C�}�[�Z�b�g
	void set(LL microseconds)
	{
		if (isSet)return;
		start_time = GetNowHiPerformanceCount();
		configured_time = microseconds;
		isSet = true;
	}

	//���Z�b�g�����炱������s����܂ł�����x�g���Ȃ�
	void reset()
	{
		isSet = false;
		configured_time = 0;
		start_time = 0;
	}

	//�^�C�}�[�̖��O()�Ŏc�莞�Ԃ��擾(�^�C�}�[�I����0)
	LL operator()()const&
	{
		LL temp = configured_time + start_time - GetNowHiPerformanceCount();
		return max(temp, 0);
	}
};

//�������^�C�}�[
class Timer
{
	using uLL = unsigned long long;

private:
	/* �O���Q�ƕs�� */

	uLL __remaining = 0;
	uLL __configured = 0;

public:

	//�R���X�g���N�^
	Timer()
	{}

	Timer(uLL frames):__configured{frames}, __remaining{frames}
	{}

	//���Ԃ�ݒ�(�t���[��)
	void set(uLL frames)
	{
		__configured = frames;
	}

	//���Ԃ�ݒ�(�b)
	void sets(uLL sec)
	{
		__configured = sec * 60;
	}

	//�^�C�}�[�����Z�b�g����
	void reset()
	{
		__remaining = __configured;
	}

	//���t���[�����s
	//��������s���Ă���ԃ^�C�}�[���쓮����
	void update()
	{
		if (__remaining != 0)
		{
			--__remaining;
		}
	}

	//�c��t���[��
	uLL operator()()
	{
		return __remaining;
	}
};