#include "mode.h"
#include "title.h"
#include "main_play.h"

extern int InitGameMode(GameMode mode)
{
	switch (mode)
	{
	case GameMode_None:
		return -1;
		break;
	case GameMode_Title:
		return InitTitle();
		break;
	case GameMode_MainPlay:
		return InitMainPlay();
		break;
	case GameMode_Ended:
		return -1;
		break;
	case GameMode_Max:
		return -1;
		break;
	default:
		return -1;
		break;
	}
}

extern void EndGameMode(GameMode mode)
{
	switch (mode)
	{
	case GameMode_None:
		break;
	case GameMode_Title:
		EndTitle();
		break;
	case GameMode_MainPlay:
		EndMainPlay();
		break;
	case GameMode_Ended:
		break;
	case GameMode_Max:
		break;
	default:
		break;
	}
}

extern GameMode UpdateGameMode(GameMode mode, const Keyboard *key)
{
	switch (mode)
	{
	case GameMode_None:
		return mode;
		break;
	case GameMode_Title:
		return UpdateTitle(mode, key);
		break;
	case GameMode_MainPlay:
		return UpdateMainPlay(mode, key);
		break;
	case GameMode_Ended:
		return mode;
		break;
	case GameMode_Max:
		return mode;
		break;
	default:
		return mode;
		break;
	}
}

extern void DrawGameMode(GameMode mode)
{
	switch (mode)
	{
	case GameMode_None:
		break;
	case GameMode_Title:
		DrawTitle();
		break;
	case GameMode_MainPlay:
		DrawMainPlay();
		break;
	case GameMode_Ended:
		break;
	case GameMode_Max:
		break;
	default:
		break;
	}
}
