#pragma once

//�v���[���[�̌��E��(���1)
#define PlayerCapacity	1
//�e�̌��E��
#define BulletCapacity	5
//�G�̌��E��
#define EnemyCapacity	10

enum ObjectKind
{
	ObjectKind_None,
	ObjectKind_Player,
	ObjectKind_Bullet,
	ObjectKind_Enemy,
	ObjectKind_Max,
};

extern size_t GetCapacity(ObjectKind kind);
