#include "object_kind.h"

static size_t capacity[ObjectKind_Max] = {
	0,
	PlayerCapacity,
	BulletCapacity,
	EnemyCapacity,
};

extern size_t GetCapacity(ObjectKind kind)
{
	return capacity[(int)(kind)];
}