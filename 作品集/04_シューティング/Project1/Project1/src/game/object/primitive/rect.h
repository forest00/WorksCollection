#pragma once

struct Rect
{
	double left;
	double top;
	double right;
	double bottom;
};

extern bool IsHitRect(Rect a, Rect b);
