#pragma once
#include "stdafx.h"
#include "Poi.h"

class Aim : public Poi {
public:
	double scale = 1.0;
	Aim(int x,
		int	y,
		bool can_collision,
		const std::vector<int>& image_handle
	) : Poi(x, y, can_collision, image_handle) {}
	std::vector<int> center() const& {
		return {x + (int)(scale * xlength / 2), (y + (int)(scale * ylength / 2))};
		// return {0, 0};
	}
};
