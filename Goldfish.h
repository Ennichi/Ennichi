#pragma once
#include "stdafx.h"
#include "Poi.h"
#include "functions.h"
#define ANGLE0 (0)

//�����̓�����
enum class MOV_OPTION
{
	CIRCLE = 0,
	WAVE
};


//���C���N���X
class Goldfish : public Obj
{
private:
	/* �O���A�N�Z�X�s�� */
	double speed = 0.0;
	double xerr = 0.0;
	double yerr = 0.0;
	double __wave_info_x = 0.0;
	double __wave_info_angle = 0.0;
	unsigned int difficulty = 10;

	//�N���X�O�ł͎��s���Ȃ�
	//���g�ƃ|�C���r,�]��
	//~1000�̒l��Ԃ�
	int triggeredLevel(const Poi& poi)const&
	{
		int midx = x, midy = y;
		Goldfish::center(midx, midy, xlength, ylength);
		double d = 1000.0 * dist_elipse(midx, midy, poi.center()[0], poi.center()[1], xlength, ylength);
		return 1000 - (int)(d * static_cast<double>(difficulty));
	}

public:
	/* �A�N�Z�X�\ */

	//difficulty�̃Z�b�^�[(�s���Ȓl���͖h�~)
	void setDifficulty(unsigned int dif)
	{
		if (dif == 0)return;
		difficulty = dif;
	}

	//difficulty�̃Q�b�^�[
	int getDifficulty()
	{
		return difficulty;
	}

	//���̃t���[���ɍX�V(������)
	//�����������Ă������(x,y)��i�߂�
	inline void Next()
	{
		double delx = speed * std::cos(angle + ANGLE0) + xerr;
		double dely = speed * std::sin(angle + ANGLE0) + yerr;
		x += std::lround(delx);
		y += std::lround(dely);
		xerr = delx - std::lround(delx);//x���W�̊ۂߌ덷(�����[�v�ő���)
		yerr = dely - std::lround(dely);//y���W�̊ۂߌ덷(�����[�v�ő���)
	}
	
	/*�����̓�����ݒ�(������)
	* 
	**** Next()�̑O�Ɏ��s���Ă�������(���t���[�����s) ****
	* MOV_OPTION::CIRCLE
	* | �~�ʂɉ����ē���
	* | p1...���a, p2...1.0�Ȃ�angle+90�̕����ɒ��S�A-1.0�Ȃ�angle-90�̕����ɒ��S
	* MOV_OPTION::WAVE
	* | �g�^�ɓ���
	* | p1...�U��, p2...�g��
	*/
	inline void SetMovement(MOV_OPTION option, double p1, double p2)
	{
		switch (option)
		{
			//�~�ʂ̋O���œ���
		case MOV_OPTION::CIRCLE:
			if (!(p2 == 1.0 || p2 == -1.0))
				throw new std::out_of_range("�~�`�ړ��̂Ƃ���p2��1.0��-1.0�ɂ��Ă�������");
			angle += p2 * (DX_PI / 2.0 - acos(speed / (p1 * 2.0)));
			__wave_info_x = 0.0;
			break;

			//�����g�̋O���œ���
		case MOV_OPTION::WAVE:
			if (__wave_info_x == 0.0)__wave_info_angle = angle;
			double slope = -p1 * (2.0 * DX_PI / p2) * std::sin((2.0 * DX_PI / p2) * __wave_info_x);
			__wave_info_x += speed / (std::sqrt(slope * slope + 1));
			angle = __wave_info_angle + std::atan(slope);
			break;
		}
	}

	//�R���X�g���N�^(������)
	Goldfish(
		int x,
		int	y,
		bool can_collision,
		const std::vector<int>& image_handle
	) : Obj(x,y,can_collision,image_handle)
	{}

	//�R���X�g���N�^(������)
	Goldfish(
		int x,
		int	y,
		double angle,
		bool can_collision,
		const std::vector<int>& image_handle
	) : Obj(x,y,angle,can_collision,image_handle)
	{}


	//�X�s�[�h�����߂�
	double setSpeed(double spmin, double spmax)
	{
		if (spmin > spmax || spmin < 0)
			throw new std::out_of_range("spmin��spmax�͂Ƃ��ɐ��Ō�҂̕����傫���Ȃ���΂Ȃ�Ȃ�");
		std::random_device seed;
		std::mt19937_64 mt(seed());
		std::uniform_real_distribution<> randsp(spmin, spmax);
		speed = randsp(mt);//����
		return speed;
	}


	//�`��֐��I�[�o�[���C�h(������)
	void draw() override
	{
		Obj::draw();
	}

	//�C���X�^���X������s���Ȃ��֐�
	//�����̒��S��Ԃ��}�N��
	static inline void center(int& x, int& y, int xlength, int ylength)
	{
		x += xlength / 2;
		y += ylength / 2;
	}


	/*
	* ������߂܂��邱�Ƃ��ł������Ԃ��֐�
	* ������:�����̃C���X�^���X�̔z��
	* ������:�|�C�̃C���X�^���X
	* �ł���Α���3�̊֐��̂ǂꂩ���g���Ă�������(�������͏������삪�x������)
	*/
	static std::vector<bool> isCought(const std::vector<Goldfish>& goldfish, const Poi& poi)
	{
		std::random_device seed;
		std::mt19937_64 mt(seed());
		std::uniform_int_distribution<> dice(1, 1000);
		return Goldfish::isCought(goldfish, poi, mt, dice);
	}


	/*
	* ������߂܂��邱�Ƃ��ł������Ԃ��֐�
	* ������:�����̃C���X�^���X�̔z��
	* ������:�|�C�̃C���X�^���X
	* ��O����:������{�̂̃C���X�^���X
	* ��l����:���U��l���z�N���X(����)�̃C���X�^���X(������̈ꕔ)
	* std::uniform_int_distribution<>�̃C���X�^���X�������̓R���X�g���N�^�̈�����1,1000�ɂ��Ă�������
	*/
	static std::vector<bool> isCought(
		const std::vector<Goldfish>& goldfish, 
		const Poi& poi,
		std::mt19937_64& mt,
		std::uniform_int_distribution<int>& dice
	)
	{
		std::vector<bool> result;
		if (dice.a() != 1 || dice.b() != 1000)throw new std::invalid_argument("�����͈̔͂�1����1000�ɐݒ肵�Ă�������");
		for (unsigned int i = 0; i < goldfish.size(); ++i)
		{
			//can_collision��false��������false,poi��goldfish�̋����ɂ���ĕϓ�����m����true��false�����܂�
			result.push_back(goldfish[i].can_collision && goldfish[i].triggeredLevel(poi) >= dice(mt));
		}
		return result;
	}


	/*
	* ������߂܂��邱�Ƃ��ł������Ԃ��֐�
	* ������:�����̃C���X�^���X�̔z��
	* ������:�|�C�̃C���X�^���X
	* ��O����:������{�̂̃C���X�^���X
	* ��l����:���U��l���z�N���X(����)�̃C���X�^���X(������̈ꕔ)
	* ��܈���:�߂�l���i�[�����Q��
	* �߂�l:�߂܂��������̐�
	* std::uniform_int_distribution<>�̃C���X�^���X�������̓R���X�g���N�^�̈�����1,1000�ɂ��Ă�������
	*/
	static int isCought(
		const std::vector<Goldfish>& goldfish,
		const Poi& poi,
		std::mt19937_64& mt,
		std::uniform_int_distribution<int>& dice,
		std::vector<bool>& result
	)
	{
		int cought_count = 0;
		if (dice.a() != 1 || dice.b() != 1000)throw new std::invalid_argument("�����͈̔͂�1����1000�ɐݒ肵�Ă�������");
		for (unsigned int i = 0; i < goldfish.size(); ++i)
		{
			//can_collision��false��������false,poi��goldfish�̋����ɂ���ĕϓ�����m����true��false�����܂�
			result.push_back(goldfish[i].can_collision && goldfish[i].triggeredLevel(poi) >= dice(mt));
			if (*result.end())cought_count++;
		}
		return cought_count;
	}
	

	//�ʃp�^�[��
	bool isCought(
		const Poi& poi,
		std::mt19937_64& mt,
		std::uniform_int_distribution<int>& dice
	)const&
	{
		if (dice.a() != 1 || dice.b() != 1000)throw new std::invalid_argument("�����͈̔͂�1����1000�ɐݒ肵�Ă�������");
		return can_collision && (triggeredLevel(poi) >= dice(mt));
	}
};
