#pragma once
#include <Windows.h>

extern int initializeGame(HWND hWnd);
extern int finalizeGame();
extern int updateGame();
extern int drawGame(HWND hWnd, HDC hdc);
