#include "game.h"
#include "mode.h"
#include "object/primitive/screen.h"
#include "object/primitive/keyboard.h"
#include "object/looks.h"
#include <stdlib.h>

static Keyboard *key = nullptr;
static GameMode now_mode = GameMode_None;
static GameMode init_mode = GameMode_Title;

static int ChengeMode(GameMode next_mode)
{
	//�ύX�̕K�v���Ȃ��ꍇ
	if (next_mode == now_mode)
	{
		return 0;
	}
	//���[�h�I��
	EndGameMode(now_mode);
	//���̃��[�h��
	now_mode = next_mode;
	//���[�h������
	return InitGameMode(now_mode);
}

static GameMode GetNowMode()
{
	return now_mode;
}

extern int InitGame()
{
	//�X�N���[��������
	InitScreen();
	//�L�����̌����ړǂݍ���
	InitLooks();
	//�L�[���͏�����
	key = (Keyboard *)(malloc(sizeof(Keyboard)));
	if (key == nullptr)
	{
		return -1;
	}
	CheckKey(key);
	//�Q�[�����[�h������
	if (ChengeMode(init_mode) != 0)
	{
		return -1;
	}
	return 0;
}

extern void EndGame()
{
	EndGameMode(GetNowMode());
	EndLooks();
	EndScreen();
}

extern bool ContinueGame()
{
	//���ԑ҂�
	Wait(64);
	//���łɏI�����Ă��邩�`�F�b�N
	if (GetNowMode() == GameMode_Ended)
	{
		return false;
	}
	//�G�X�P�[�v�L�[���̓`�F�b�N
	return key->push[EscapeKey()] == false;
}

extern void UpdateGame()
{
	//�L�[�{�[�h�`�F�b�N
	CheckKey(key);
	//���݂̃��[�h���X�V
	GameMode next_mode = UpdateGameMode(GetNowMode(), key);
	//���̃��[�h��
	if (ChengeMode(next_mode) != 0)
	{
		ChengeMode(GameMode_Ended);
	}
}

extern void DrawGame()
{
	//�X�N���[���N���A
	ClearScreen();
	//���݂̃��[�h��`��
	DrawGameMode(GetNowMode());
	//�X�N���[���`��
	DrawScreen();
}
