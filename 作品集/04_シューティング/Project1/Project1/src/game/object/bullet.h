#pragma once
#include "primitive/vector2d.h"

//�e���o��
extern int SetBullet(Vector2D pos, Vector2D velocity);

//�e�������ꂽ�Ƃ��ɌĂ΂��
extern void BulletCleared(void *kind_info);

//�e�X�V
extern void UpdateBullet(int object_index);

//�e�̐��𓾂�
extern size_t GetBulletNum();
