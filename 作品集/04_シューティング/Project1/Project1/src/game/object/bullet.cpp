#include "bullet.h"
#include "object.h"
#include "primitive/screen.h"

struct AddInfo
{
	Vector2D velocity;
};

extern int SetBullet(Vector2D pos, Vector2D velocity)
{
	Object *obj = SetObject(ObjectKind_Bullet, pos, LooksID_Bullet, malloc(sizeof(AddInfo)));
	if (obj == nullptr)
	{
		return -1;
	}
	AddInfo *info = (AddInfo *)(obj->kind_info);
	info->velocity = velocity;
	return 0;
}

extern void BulletCleared(void *kind_info)
{
	free(kind_info);
}

extern void UpdateBullet(int object_index)
{
	Object *obj = GetObject(object_index);
	const Looks *looks = GetLooks(obj->looks_id);
	AddInfo *info = (AddInfo *)(obj->kind_info);
	obj->pos = Add(obj->pos, info->velocity);
	Rect looks_rect = CalcLooksRect(looks, obj->pos);
	if (OutOfScreen(looks_rect))
	{
		ClearObject(obj);
	}
}

extern size_t GetBulletNum()
{
	return GetObjectKindNum(ObjectKind_Bullet);
}
