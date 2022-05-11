#pragma once
#include "../../../header/stdafx.h"

class Aim : public Poi {
public:
	Aim(int x,
		int	y,
		bool can_collision,
		const std::vector<int>& image_handle
	) : Poi(x, y, can_collision, image_handle) {}
	std::vector<int> center() const& {
		return {x + xlength / 2, (y + ylength / 2)};
		// return {0, 0};
	}

	void next() {//aim�̓���(�����n�_�Ƃ��锽���v���
		if (y == 320 && x > 280) x -= 10;//��
		if (x == 280 && y < 560) y +=  10;//�E
		if (y == 560 && x < 900) x += 10;//��
		if (x == 900 && y > 320)y -= 10;//����


		return;
	}
};
