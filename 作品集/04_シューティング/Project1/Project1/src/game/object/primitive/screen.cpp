//親ヘッダー
#include "screen.h"
//puts
#include <stdio.h>
//malloc, free
#include <stdlib.h>
//テキストカーソル移動用
#include <Windows.h>

static char *screen = nullptr;

static HANDLE stdout_handle;

static CONSOLE_SCREEN_BUFFER_INFO screen_info;

static void MySetConsoleCursorPosition(int x, int y)
{
	//static HANDLE stdout_handle = nullptr;
	//if (stdout_handle == nullptr)
	//{
	//	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//}
	COORD new_pos;
	new_pos.X = (short)(x);
	new_pos.Y = (short)(y);
	SetConsoleCursorPosition(stdout_handle, new_pos);
}
static void WriteScreenUnsafe(int x, int y, char c)
{
	int index = ToScreenIndex(x, y);
	screen[index] = c;
}
extern int InitScreen()
{
	if (screen != nullptr)
	{
		return -1;
	}
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(stdout_handle, &screen_info);
	screen = (char *)malloc(sizeof(char) * (ScreenWidth() + 1) * ScreenHeight());
	if (screen == nullptr)
	{
		return -1;
	}
	ClearScreen();
	return 0;
}
extern int EndScreen()
{
	if (screen == nullptr)
	{
		return -1;
	}
	free(screen);
	screen = nullptr;
	return 0;
}
extern void ClearScreen()
{
	for (int y = 0; y < ScreenHeight(); y++)
	{
		for (int x = 0; x < ScreenWidth(); x++)
		{
			WriteScreenUnsafe(x, y, ' ');
		}
		WriteScreenUnsafe(ScreenWidth(), y, '\n');
	}
	WriteScreenUnsafe(ScreenWidth(), ScreenHeight() - 1, '\0');
	MySetConsoleCursorPosition(0, 0);
}
extern int ScreenWidth()
{
	return screen_info.srWindow.Right - 1;
}
extern int ScreenHeight()
{
	return screen_info.srWindow.Bottom;
}
extern int WriteScreen(int x, int y, char c)
{
	if (OutOfScreen(x, y))
	{
		return -1;
	}
	WriteScreenUnsafe(x, y, c);
	return 0;
}
extern int WriteScreen(int x, int y, const char *s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		WriteScreen(x + i, y, s[i]);
	}
	return 0;
}
extern void DrawScreen()
{
	puts(screen);
}
extern int ToScreenIndex(int x, int y)
{
	return y * (ScreenWidth() + 1) + x;
}
extern int ToScreenCoord(int index, int *y)
{
	*y = index / (ScreenWidth() + 1);
	return index % (ScreenWidth() + 1);
}
extern bool OutOfScreen(int x, int y)
{
	return
		x < 0 || x >= ScreenWidth() ||
		y < 0 || y >= ScreenHeight();
}
extern bool OutOfScreen(Rect rect)
{
	return
		rect.left >= ScreenWidth() ||
		rect.top >= ScreenHeight() ||
		rect.right < 0 ||
		rect.bottom < 0;
}
extern void Wait(int millisec)
{
	Sleep(millisec);
}
