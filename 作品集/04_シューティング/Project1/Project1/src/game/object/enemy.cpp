#include "enemy.h"
#include "object.h"
#include "primitive/screen.h"

extern int SetEnemy(Vector2D pos)
{
	Object *obj = SetObject(ObjectKind_Enemy, pos, LooksID_Enemy, nullptr);
	if (obj == nullptr)
	{
		return -1;
	}
	//既に存在している他の敵との重なりを解消
	int hit_count = 0;
	int hit_count_max = 32;
	Vector2D dir = Vector2D_UniformNormal();
	if (dir.x < 0)
	{
		dir.x *= -1.0;
	}
	while (true)
	{
		//衝突チェック
		Object *hit = HitObject(obj, BeginObject(), true);
		if (hit == OutObject())
		{
			break;
		}
		//衝突相手が自分と同じ種別(敵キャラ)なら
		if (hit->kind == obj->kind)
		{
			//位置を修正
			obj->pos = Add(obj->pos, dir);
			//カウントアップ
			hit_count++;
			//カウントが一定以上になったら終わり
			if (hit_count >= hit_count_max)
			{
				break;
			}
		}
	}
	return 0;
}

extern void EnemyCleared(void *kind_info)
{
	(void)(kind_info);
}

extern void UpdateEnemy(int object_index)
{
	Object *obj = GetObject(object_index);
	const Looks *looks = GetLooks(obj->looks_id);
	obj->pos.x -= EnemyMoveSpeed;
	Rect looks_rect = CalcLooksRect(looks, obj->pos);
	//左端に行ったら消える
	if (OutOfScreen(looks_rect))
	{
		if (obj->pos.x < ScreenWidth())
		{
			ClearObject(obj);
			return;
		}
	}
	Object *current = BeginObject();
	while (true)
	{
		//衝突チェック
		Object *hit = HitObject(obj, current, true);
		if (hit == OutObject())
		{
			break;
		}
		//衝突相手が弾なら
		if (hit->kind == ObjectKind_Bullet)
		{
			//消える
			ClearObject(obj);
			//相手も消す
			ClearObject(hit);
			return;
		}
		//次の相手へ
		current = NextObject(hit);
	}
}

extern size_t GetEnemyNum()
{
	return GetObjectKindNum(ObjectKind_Enemy);
}
