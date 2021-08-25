#include "stdafx.h"
#include "functions.h"

double dist_elipse(int x1, int y1, int x2, int y2, double magx, double magy)
{
	x1 -= x2;
	y1 -= y2;
	magx = _CAST(x1) / magx;
	magy = _CAST(y1) / magy;
	return distance(magx, magy);
}