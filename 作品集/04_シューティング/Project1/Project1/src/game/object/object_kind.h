#pragma once

//ƒvƒŒ[ƒ„[‚ÌŒÀŠE—Ê(í‚É1)
#define PlayerCapacity	1
//’e‚ÌŒÀŠE—Ê
#define BulletCapacity	5
//“G‚ÌŒÀŠE—Ê
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
