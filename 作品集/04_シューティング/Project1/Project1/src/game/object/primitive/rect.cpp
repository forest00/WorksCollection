#include "rect.h"

extern bool IsHitRect(Rect a, Rect b)
{
	return
		a.left < b.right &&
		a.right > b.left &&
		a.top < b.bottom &&
		a.bottom > b.top;
}
