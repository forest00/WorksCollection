#include <conio.h>
#include "keyboard.h"

extern void CheckKey(Keyboard *key)
{
	for (int i = 0; i < 256; i++)
	{
		key->push[i] = false;
	}
	while (_kbhit())
	{
		int ch = _getch();
		key->push[ch] = true;
	}
}

extern int ArrowUp()
{
	return (int)'H';//72
}

extern int ArrowDown()
{
	return (int)'P';//80
}

extern int ArrowRight()
{
	return (int)'M';//77
}

extern int ArrowLeft()
{
	return (int)'K';//75
}

extern int EscapeKey()
{
	return 27;
}
