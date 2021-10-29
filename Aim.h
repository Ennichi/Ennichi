#pragma once
#include "stdafx.h"
#include "Poi.h"

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
};
