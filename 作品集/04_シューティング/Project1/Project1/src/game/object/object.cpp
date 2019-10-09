#include "object.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
//for debug
#include "primitive/debug.h"

//オブジェクト
static Object *all_obj = nullptr;

//オブジェクトの数
static size_t object_kind_num[(int)(ObjectKind_Max)];
static size_t object_num = 0;

//すべてのオブジェクトのメモリ確保
static int AllocateObjectMemory()
{
	if (all_obj != nullptr)
	{
		return -1;
	}
	all_obj = (Object *)malloc(sizeof(Object) * ObjectCapacity);
	if (all_obj == nullptr)
	{
		return -1;
	}
	for (int i = 0; i < ObjectCapacity; i++)
	{
		all_obj[i].kind = ObjectKind_None;
	}
	return 0;
}

//すべてのオブジェクトのメモリ解放
static int DeallocateObjectMemory()
{
	if (all_obj == nullptr)
	{
		return -1;
	}
	free(all_obj);
	all_obj = nullptr;
	return 0;
}

//オブジェクトの数をリセット
static void CountReset()
{
	for (int i = 0 ; i < (int)(ObjectKind_Max) ; i++)
	{
		object_kind_num[i] = 0;
	}
	object_num = 0;
}

//オブジェクトが空か判定
static bool IsNoneObject(const Object *obj)
{
	return obj->kind == ObjectKind_None;
}

//空のオブジェクトを探す
static Object *FindNoneObject()
{
	for (int i = 0; i < ObjectCapacity; i++)
	{
		Object *obj = GetObject(i);
		if (IsNoneObject(obj))
		{
			return obj;
		}
	}
	return nullptr;
}

extern int InitObject()
{
	//メモリ確保&カウントリセット
	if (AllocateObjectMemory() != 0)
	{
		return -1;
	}
	CountReset();
	return 0;
}

extern int EndObject()
{
	//メモリ解放のみ
	if (DeallocateObjectMemory() != 0)
	{
		return -1;
	}
	return 0;
}

extern Object *GetObject(int index)
{
	//インデックスに対応したオブジェクトを返す
	return &all_obj[index];
}

extern Object *BeginObject()
{
	//先頭のオブジェクトを返す
	return GetObject(0);
}

extern Object *OutObject()
{
	return GetObject(ObjectCapacity);
}

extern Object *NextObject(Object *obj)
{
	return obj + 1;
}

extern Object *SetObject(ObjectKind kind, Vector2D pos, LooksID looks_id, void *kind_info)
{
	//限界量を超えていたら何もしない
	if (object_kind_num[(int)(kind)] >= GetCapacity(kind))
	{
		return nullptr;
	}
	//空きを探す
	Object *obj = FindNoneObject();
	//空きがなければ何もしない
	if (obj == nullptr)
	{
		return nullptr;
	}
	//セット
	obj->kind = kind;
	obj->pos = pos;
	obj->looks_id = looks_id;
	obj->kind_info = kind_info;
	//カウントアップ
	object_num++;
	object_kind_num[(int)(kind)]++;
	return obj;
}

extern int ClearObject(Object *obj)
{
	//ヌルチェック
	if (obj == nullptr)
	{
		return -1;
	}
	//空のオブジェクトチェック
	if (IsNoneObject(obj))
	{
		return -1;
	}
	//カウントダウン
	object_kind_num[(int)(obj->kind)]--;
	object_num--;
	//種類ごとに後処理する
	switch (obj->kind)
	{
	case ObjectKind_None:
		break;
	case ObjectKind_Player:
		PlayerCleared(obj->kind_info);
		break;
	case ObjectKind_Bullet:
		BulletCleared(obj->kind_info);
		break;
	case ObjectKind_Enemy:
		EnemyCleared(obj->kind_info);
		break;
	case ObjectKind_Max:
		break;
	default:
		break;
	}
	//空のオブジェクトにする
	obj->kind = ObjectKind_None;
	return 0;
}

extern void ClearAllObject()
{
	//全部のオブジェクトを消す
	for (int i = 0; i < ObjectCapacity; i++)
	{
		Object *obj = GetObject(i);
		ClearObject(obj);
	}
}

extern void UpdateObject(const Keyboard *key)
{
	//全部のオブジェクトに対して
	for (int i = 0; i < ObjectCapacity; i++)
	{
		//種類ごとに更新処理
		switch (all_obj[i].kind)
		{
		case ObjectKind_None:
			break;
		case ObjectKind_Player:
			UpdatePlayer(i, key);
			break;
		case ObjectKind_Bullet:
			UpdateBullet(i);
			break;
		case ObjectKind_Enemy:
			UpdateEnemy(i);
			break;
		case ObjectKind_Max:
			break;
		default:
			break;
		}
	}
}

extern void DrawObject()
{
	//for debug
	//デバッグ情報削除
	ClearDebug();
	//デバッグ情報登録
	RegisterDebug("Object", GetObjectNum());
	RegisterDebug("Player", GetPlayerNum());
	RegisterDebug("Bullet", GetBulletNum());
	RegisterDebug("Enemy ", GetEnemyNum());
	//全部のオブジェクトに対して
	for (int i = 0; i < ObjectCapacity; i++)
	{
		//見た目を取得
		Object *obj = GetObject(i);
		if (IsNoneObject(obj) == false)
		{
			const Looks *looks = GetLooks(obj->looks_id);
			//描画
			DrawLooks(looks, obj->pos);
		}
	}
}

extern bool IsHitObjectRect(const Object *obj, Rect hit_rect)
{
	//空のオブジェクトなら失敗
	if (IsNoneObject(obj))
	{
		return false;
	}
	//矩形取得
	const Looks *looks = GetLooks(obj->looks_id);
	Rect obj_rect = CalcLooksRect(looks, obj->pos);
	//判定
	return IsHitRect(obj_rect, hit_rect);
}

extern Object *HitObject(const Object *obj, Object *offset, bool ignore_self)
{
	//オフセットを適用
	Object *current = offset;
	//ヌルチェック
	if (current == nullptr)
	{
		current = BeginObject();
	}
	//矩形取得
	const Looks *looks = GetLooks(obj->looks_id);
	Rect obj_rect = CalcLooksRect(looks, obj->pos);
	//末尾到達チェック
	while (current != OutObject())
	{
		//自身を無視する
		if (ignore_self && (current == obj))
		{
			current = NextObject(current);
		}
		//衝突チェック
		if (IsHitObjectRect(current, obj_rect))
		{
			return current;
		}
		//次の相手へ
		current = NextObject(current);
	}
	return current;
}

extern size_t GetObjectKindNum(ObjectKind kind)
{
	//種類ごとの総数を返す
	return object_kind_num[(int)(kind)];
}

extern size_t GetObjectNum()
{
	//総数を返す
	return object_num;
}
