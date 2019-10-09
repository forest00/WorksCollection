#pragma once
#include "object/primitive/keyboard.h" 

//���[�h��`
enum GameMode
{
	GameMode_None,
	GameMode_Title,
	GameMode_MainPlay,
	GameMode_Ended,
	GameMode_Max,
};

//���[�h��������
extern int InitGameMode(GameMode mode);

//���[�h���I��
extern void EndGameMode(GameMode mode);

//���[�h���X�V
extern GameMode UpdateGameMode(GameMode mode, const Keyboard *key);

//���[�h��`��
extern void DrawGameMode(GameMode mode);
