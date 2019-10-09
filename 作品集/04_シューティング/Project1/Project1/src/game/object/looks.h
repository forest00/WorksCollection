#pragma once
#include "primitive/vector2d.h"
#include "primitive/rect.h"

//��s�̌��E
#define LooksLineMax	256
//�t�@�C���̐�
#define LooksFileNum	3
//�t�@�C���ꗗ��
#define LooksFilename_Player	"res/looks/player.txt"
#define LooksFilename_Enemy		"res/looks/enemy.txt"
#define LooksFilename_Bullet	"res/looks/bullet.txt"	
//�t�@�C���ꗗ��

enum LooksID
{
	LooksID_None,
	LooksID_Player,
	LooksID_Enemy,
	LooksID_Bullet,
	LooksID_Max,
};

struct Looks
{
	char **body;
	int *length;
	int max_length;
	int lines;
};

//������
extern void InitLooks();
//�I��
extern void EndLooks();
//�擾
extern const Looks *GetLooks(LooksID id);
//��`�v�Z
extern Rect CalcLooksRect(const Looks *looks, Vector2D pos);
//�X�N���[���ɕ`��
extern void DrawLooks(const Looks *looks, Vector2D pos);
