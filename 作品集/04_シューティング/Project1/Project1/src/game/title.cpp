#include "title.h"
#include "object/primitive/screen.h"
#include <time.h>

static clock_t init_time = 0;

extern int InitTitle()
{
	init_time = clock();
	return 0;
}
extern void EndTitle()
{

}
extern GameMode UpdateTitle(GameMode mode, const Keyboard *key)
{
	if (key->push[' '])
	{
		return GameMode_MainPlay;
	}
	return mode;
}
extern void DrawTitle()
{
	WriteScreen(5, 5, "Title");
	clock_t separate = (clock_t)(CLOCKS_PER_SEC * 0.5);
	if (((clock() - init_time) % separate) > (separate / 4))
	{
		WriteScreen(7, 7, "Press [space key]");
	}
}
