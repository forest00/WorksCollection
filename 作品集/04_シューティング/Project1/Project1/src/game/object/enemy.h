#pragma once
#include "primitive/vector2d.h"

//�G�̈ړ����x
#define EnemyMoveSpeed	0.3

//�G���o��
extern int SetEnemy(Vector2D pos);

//�G�������ꂽ�Ƃ��ɌĂ΂��
extern void EnemyCleared(void *kind_info);

//�G�X�V
extern void UpdateEnemy(int object_index);

//�G�̐��𓾂�
extern size_t GetEnemyNum();
