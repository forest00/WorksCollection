#pragma once
#include "primitive/vector2d.h"
#include "primitive/keyboard.h"

#define PlayerMoveSpeed	1.0

//�v���[���[���o��
extern int SetPlayer(Vector2D pos);

//�v���[���[�������ꂽ�Ƃ��ɌĂ΂��
extern void PlayerCleared(void *kind_info);

//�v���[���[�X�V
extern void UpdatePlayer(int object_index, const Keyboard *key);

//�v���[���[�̐��𓾂�
extern size_t GetPlayerNum();
