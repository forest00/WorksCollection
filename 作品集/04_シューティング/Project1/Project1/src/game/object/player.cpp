#include "player.h"
#include "player_ref.h"
#include "object.h"
#include "bullet.h"
#include "primitive/screen.h"

extern int SetPlayer(Vector2D pos)
{
	Object *obj = SetObject(ObjectKind_Player, pos, LooksID_Player, nullptr);
	if (obj == nullptr)
	{
		return -1;
	}	
	SetPlayerRef(obj);
	return 0;
}

extern void PlayerCleared(void *kind_info)
{
	(void)(kind_info);
}

extern void UpdatePlayer(int object_index, const Keyboard * key)
{
	Object *obj = GetObject(object_index);
	const Looks *looks = GetLooks(obj->looks_id);
	Vector2D movevec = Vector2D_Literal(0.0, 0.0);
	if (key->push['a'] || key->push[ArrowLeft()])
	{
		movevec.x -= PlayerMoveSpeed * 2.0;
	}
	if (key->push['d'] || key->push[ArrowRight()])
	{
		movevec.x += PlayerMoveSpeed * 2.0;
	}
	if (key->push['w'] || key->push[ArrowUp()])
	{
		movevec.y -= PlayerMoveSpeed * 1.0;
	}
	if (key->push['s'] || key->push[ArrowDown()])
	{
		movevec.y += PlayerMoveSpeed * 1.0;
	}
	if (key->push[' '])
	{
		SetBullet(
			Add(obj->pos, Vector2D_Literal(looks->max_length, 1)),
			Vector2D_Literal(2, 0)
		);
	}
	obj->pos = Add(obj->pos, movevec);
	if (obj->pos.x < 0)
	{
		obj->pos.x = 0;
	}
	if (obj->pos.y < 0)
	{
		obj->pos.y = 0;
	}
	if (obj->pos.x >= ((double)(ScreenWidth()) - (double)(looks->max_length)))
	{
		obj->pos.x = ((double)(ScreenWidth())- (double)(looks->max_length));
	}
	if (obj->pos.y >= ((double)(ScreenHeight())- (double)(looks->lines)))
	{
		obj->pos.y = ((double)(ScreenHeight())- (double)(looks->lines));
	}
}

extern size_t GetPlayerNum()
{
	return GetObjectKindNum(ObjectKind_Player);
}
